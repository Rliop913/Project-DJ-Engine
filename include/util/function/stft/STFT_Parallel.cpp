#include "util/function/stft/STFT_Parallel.hpp"

#include "util/function/stft/detail/PDJE_Parallel_Runtime_Loader.hpp"
#include "util/function/stft/detail/OpenclBackend.hpp"
#include "util/function/stft/detail/SerialBackend.hpp"
#include "util/function/stft/detail/StftBackend.hpp"

#include <algorithm>
#include <cstddef>
#include <exception>
#include <memory>
#include <optional>
#include <utility>
#include <vector>

namespace PDJE_PARALLEL::detail {

namespace {

bool
IsPowerOfTwo(const int value) noexcept
{
    if (value <= 0) {
        return false;
    }

    const unsigned int unsignedValue = static_cast<unsigned int>(value);
    return (unsignedValue & (unsignedValue - 1u)) == 0u;
}

unsigned int
WindowSizeExpFromFft(const int nFft) noexcept
{
    unsigned int exp   = 0u;
    unsigned int value = static_cast<unsigned int>(nFft);
    while (value > 1u) {
        value >>= 1u;
        ++exp;
    }

    return exp;
}

unsigned int
ExactFrameCount(const std::size_t fullSize,
                const unsigned int hopLength,
                const int          windowSize) noexcept
{
    if (hopLength == 0u || windowSize <= 0 ||
        fullSize < static_cast<std::size_t>(windowSize)) {
        return 0u;
    }

    return static_cast<unsigned int>(
               (fullSize - static_cast<std::size_t>(windowSize)) / hopLength) +
           1u;
}

bool
HasSupportedWindowSize(const int nFft) noexcept
{
    if (!IsPowerOfTwo(nFft)) {
        return false;
    }

    const unsigned int exp = WindowSizeExpFromFft(nFft);
    return exp >= 6u && exp < 31u;
}

bool
ValidateRequest(const std::vector<float> &PCMdata, const STFTRequest &request)
{
    if (PCMdata.empty() || request.sample_rate <= 0 || request.n_fft <= 0 ||
        !HasSupportedWindowSize(request.n_fft) || request.hop_length == 0u) {
        return false;
    }

    if (request.frame_policy == FRAME_POLICY::EXACT_WINDOWED &&
        PCMdata.size() < static_cast<std::size_t>(request.n_fft)) {
        return false;
    }

    if (!request.post_process.mel_scale) {
        return true;
    }

    if (!request.mel_filter_bank.has_value()) {
        return false;
    }

    const auto &melSpec = request.mel_filter_bank.value();
    if (melSpec.sample_rate != request.sample_rate ||
        melSpec.n_fft != request.n_fft) {
        return false;
    }

    return CheckMelVals(melSpec);
}

StftArgs
MakeStftArgs(const std::vector<float> &inputVec,
             const STFTRequest        &request,
             const std::optional<float> legacyOverlapRatio)
{
    StftArgs arglist;
    arglist.FullSize    = static_cast<unsigned int>(inputVec.size());
    arglist.sample_rate = request.sample_rate;
    arglist.windowSize  = request.n_fft;
    arglist.dc_remove   = request.dc_remove;
    arglist.mel_filter_bank =
        request.post_process.mel_scale ? request.mel_filter_bank : std::nullopt;

    if (legacyOverlapRatio.has_value()) {
        arglist.qtConst =
            toQuot(arglist.FullSize, legacyOverlapRatio.value(), arglist.windowSize);
        arglist.OMove = static_cast<unsigned int>(
            static_cast<float>(arglist.windowSize) *
            (1.0f - legacyOverlapRatio.value()));
    } else if (request.frame_policy == FRAME_POLICY::LEGACY_ZERO_PAD) {
        arglist.qtConst =
            static_cast<int>(arglist.FullSize / request.hop_length) + 1;
        arglist.OMove = request.hop_length;
    } else {
        arglist.qtConst = static_cast<int>(ExactFrameCount(
            inputVec.size(),
            request.hop_length,
            arglist.windowSize));
        arglist.OMove = request.hop_length;
    }

    if (arglist.qtConst <= 0) {
        return {};
    }

    arglist.OFullSize =
        static_cast<unsigned int>(arglist.qtConst) *
        static_cast<unsigned int>(arglist.windowSize);
    arglist.OHalfSize = arglist.OFullSize / 2u;
    return arglist;
}

} // namespace

class STFTImpl {
  public:
    STFTImpl() : serial_backend_(std::make_unique<SERIAL_STFT>())
    {
        active_backend_ = DetectPreferredBackend();

        if (active_backend_ != BACKEND_T::OPENCL) {
            return;
        }

        try {
            opencl_backend_ = std::make_unique<OPENCL_STFT>();
        } catch (const std::exception &) {
            opencl_backend_.reset();
            active_backend_ = BACKEND_T::SERIAL;
        }
    }

    STFTImpl(const STFTImpl &)            = delete;
    STFTImpl &operator=(const STFTImpl &) = delete;
    STFTImpl(STFTImpl &&)                 = delete;
    STFTImpl &operator=(STFTImpl &&)      = delete;

    BACKEND_T
    active_backend() const noexcept
    {
        return active_backend_;
    }

    StftResult
    calculate(std::vector<float>         &PCMdata,
              STFTRequest                 request,
              const std::optional<float> legacyOverlapRatio = std::nullopt)
    {
        request.post_process.check_values();
        if (!ValidateRequest(PCMdata, request)) {
            return {};
        }

        const unsigned int windowSizeExp = WindowSizeExpFromFft(request.n_fft);
        const auto         gargs =
            MakeStftArgs(PCMdata, request, legacyOverlapRatio);
        if (gargs.qtConst <= 0 || gargs.OFullSize == 0u) {
            return {};
        }

        if (active_backend_ == BACKEND_T::OPENCL && opencl_backend_) {
            try {
                auto result = opencl_backend_->Execute(PCMdata,
                                                       request.target_window,
                                                       request.post_process,
                                                       windowSizeExp,
                                                       gargs);
                if (!result.first.empty() || !result.second.empty()) {
                    return result;
                }
            } catch (const std::exception &) {
            }

            opencl_backend_.reset();
            active_backend_ = BACKEND_T::SERIAL;
        }

        if (!serial_backend_) {
            return {};
        }

        return serial_backend_->Execute(PCMdata,
                                        request.target_window,
                                        request.post_process,
                                        windowSizeExp,
                                        gargs);
    }

  private:
    std::unique_ptr<IStftBackend> serial_backend_;
    std::unique_ptr<IStftBackend> opencl_backend_;
    BACKEND_T                     active_backend_ = BACKEND_T::SERIAL;
};

} // namespace PDJE_PARALLEL::detail

namespace PDJE_PARALLEL {

STFT::STFT() : impl_(std::make_unique<detail::STFTImpl>())
{
}

STFT::~STFT() = default;

STFT::STFT(STFT &&) noexcept            = default;
STFT &
STFT::operator=(STFT &&) noexcept = default;

BACKEND_T
STFT::active_backend() const noexcept
{
    return impl_ ? impl_->active_backend() : BACKEND_T::SERIAL;
}

BACKEND_T
STFT::detect_available_backend() noexcept
{
    return detail::DetectPreferredBackend();
}

StftResult
STFT::calculate(std::vector<float> &PCMdata, const STFTRequest &request)
{
    if (!impl_) {
        return {};
    }

    return impl_->calculate(PCMdata, request);
}

StftResult
STFT::calculate(std::vector<float> &PCMdata,
                const WINDOW_LIST   target_window,
                const int           windowSizeEXP,
                const float         overlapRatio,
                POST_PROCESS        post_process)
{
    if (!impl_ || overlapRatio < 0.0f || overlapRatio >= 1.0f ||
        windowSizeEXP < 6 || windowSizeEXP >= 31) {
        return {};
    }

    post_process.check_values();

    STFTRequest request{
        .sample_rate = 48000,
        .n_fft = 1 << windowSizeEXP,
        .hop_length = std::max(
            1u,
            static_cast<unsigned int>(
                static_cast<float>(1 << windowSizeEXP) * (1.0f - overlapRatio))),
        .target_window = target_window,
        .post_process = post_process,
        .frame_policy = FRAME_POLICY::LEGACY_ZERO_PAD,
        .mel_filter_bank = post_process.mel_scale
                               ? std::optional<MelFilterBankSpec>(
                                      MelFilterBankSpec{ .sample_rate = 48000,
                                                         .n_fft =
                                                             1 << windowSizeEXP,
                                                         .n_mels = 80,
                                                         .f_min = 0.0f,
                                                         .f_max = -1.0f,
                                                         .mel_formula =
                                                             MelFormula::Slaney,
                                                         .norm =
                                                             MelNorm::Slaney })
                               : std::nullopt,
    };

    return impl_->calculate(PCMdata, request, overlapRatio);
}

} // namespace PDJE_PARALLEL
