#include "util/function/stft/STFT_Parallel.hpp"

#include "util/function/stft/detail/PDJE_Parallel_Runtime_Loader.hpp"
#include "util/function/stft/detail/OpenclBackend.hpp"
#include "util/function/stft/detail/SerialBackend.hpp"
#include "util/function/stft/detail/StftBackend.hpp"

#include <exception>
#include <memory>
#include <utility>
#include <vector>

namespace PDJE_PARALLEL::detail {

namespace {

StftArgs
MakeStftArgs(const std::vector<float> &inputVec,
             const int                 windowSizeEXP,
             const float               overlapRatio)
{
    StftArgs arglist;
    arglist.FullSize   = static_cast<unsigned int>(inputVec.size());
    arglist.windowSize = 1 << windowSizeEXP;
    arglist.qtConst = toQuot(arglist.FullSize, overlapRatio, arglist.windowSize);
    arglist.OFullSize = arglist.qtConst * arglist.windowSize;
    arglist.OHalfSize = arglist.OFullSize / 2;
    arglist.OMove =
        static_cast<unsigned int>(arglist.windowSize * (1.0f - overlapRatio));
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
    calculate(std::vector<float> &PCMdata,
              const WINDOW_LIST   target_window,
              const int           windowSizeEXP,
              const float         overlapRatio,
              POST_PROCESS        post_process)
    {
        if (PCMdata.empty() || overlapRatio < 0.0f || overlapRatio >= 1.0f ||
            windowSizeEXP < 6 || windowSizeEXP >= 31) {
            return {};
        }

        const auto gargs = MakeStftArgs(PCMdata, windowSizeEXP, overlapRatio);

        if (active_backend_ == BACKEND_T::OPENCL && opencl_backend_) {
            try {
                auto result = opencl_backend_->Execute(
                    PCMdata,
                    target_window,
                    post_process,
                    static_cast<unsigned int>(windowSizeEXP),
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
                                        target_window,
                                        post_process,
                                        static_cast<unsigned int>(windowSizeEXP),
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
STFT::calculate(std::vector<float> &PCMdata,
                const WINDOW_LIST   target_window,
                 const int           windowSizeEXP,
                 const float         overlapRatio,
                 POST_PROCESS        post_process)
{
    if (!impl_) {
        return {};
    }

    return impl_->calculate(
        PCMdata, target_window, windowSizeEXP, overlapRatio, post_process);
}

} // namespace PDJE_PARALLEL
