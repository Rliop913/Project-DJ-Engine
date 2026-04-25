#pragma once

#include "util/function/stft/STFT_Parallel.hpp"
#include "util/function/stft/detail/PDJE_Parallel_Runtime_Loader.hpp"
#include "util/function/stft/detail/SerialBackend.hpp"
#include "util/function/stft/detail/StftBackend.hpp"

#include <exception>
#include <memory>
#include <optional>
#include <vector>

namespace PDJE_TEST::util {

inline PDJE_PARALLEL::detail::StftArgs
BuildStftArgs(const std::vector<float> &inputVec,
              const int                 windowSizeExp,
              const float               overlapRatio,
              std::optional<PDJE_PARALLEL::MelFilterBankSpec> melFilterBank = std::nullopt,
              const int                 sampleRate = 48000)
{
    PDJE_PARALLEL::detail::StftArgs arglist;
    arglist.FullSize   = static_cast<unsigned int>(inputVec.size());
    arglist.sample_rate = sampleRate;
    arglist.windowSize = 1 << windowSizeExp;
    arglist.qtConst =
        PDJE_PARALLEL::toQuot(arglist.FullSize, overlapRatio, arglist.windowSize);
    arglist.OFullSize = arglist.qtConst * arglist.windowSize;
    arglist.OHalfSize = arglist.OFullSize / 2;
    arglist.OMove =
        static_cast<unsigned int>(arglist.windowSize * (1.0f - overlapRatio));
    arglist.mel_filter_bank = std::move(melFilterBank);
    arglist.dc_remove       = true;
    return arglist;
}

inline unsigned int
WindowSizeExpFromFft(const int nFft)
{
    unsigned int exp   = 0u;
    unsigned int value = static_cast<unsigned int>(nFft);
    while (value > 1u) {
        value >>= 1u;
        ++exp;
    }

    return exp;
}

inline PDJE_PARALLEL::detail::StftArgs
BuildRequestArgs(const std::vector<float>       &inputVec,
                 const PDJE_PARALLEL::STFTRequest &request)
{
    PDJE_PARALLEL::detail::StftArgs arglist;
    arglist.FullSize        = static_cast<unsigned int>(inputVec.size());
    arglist.sample_rate     = request.sample_rate;
    arglist.windowSize      = request.n_fft;
    arglist.OMove           = request.hop_length;
    arglist.dc_remove       = request.dc_remove;
    arglist.mel_filter_bank = request.post_process.mel_scale
                                  ? request.mel_filter_bank
                                  : std::nullopt;

    if (request.hop_length == 0u || request.n_fft <= 0 ||
        inputVec.size() < static_cast<std::size_t>(request.n_fft)) {
        return {};
    }

    arglist.qtConst = static_cast<int>(
        ((inputVec.size() - static_cast<std::size_t>(request.n_fft)) /
         request.hop_length) +
        1u);
    arglist.OFullSize =
        static_cast<unsigned int>(arglist.qtConst) *
        static_cast<unsigned int>(arglist.windowSize);
    arglist.OHalfSize = arglist.OFullSize / 2u;
    return arglist;
}

inline PDJE_PARALLEL::MelFilterBankSpec
BuildLegacyMelSpec(const int windowSize)
{
    return PDJE_PARALLEL::MelFilterBankSpec{
        .sample_rate = 48000,
        .n_fft = windowSize,
        .n_mels = 80,
        .f_min = 0.0f,
        .f_max = -1.0f,
        .mel_formula = PDJE_PARALLEL::MelFormula::Slaney,
        .norm = PDJE_PARALLEL::MelNorm::Slaney,
    };
}

inline PDJE_PARALLEL::STFTRequest
BuildBeatThisRequest(PDJE_PARALLEL::POST_PROCESS postProcess = {})
{
    postProcess.check_values();
    return PDJE_PARALLEL::STFTRequest{
        .sample_rate = 22050,
        .n_fft = 1024,
        .hop_length = 441u,
        .target_window = PDJE_PARALLEL::WINDOW_LIST::HANNING,
        .post_process = postProcess,
        .frame_policy = PDJE_PARALLEL::FRAME_POLICY::EXACT_WINDOWED,
        .mel_filter_bank = postProcess.mel_scale
                               ? std::optional<PDJE_PARALLEL::MelFilterBankSpec>(
                                     PDJE_PARALLEL::MelFilterBankSpec{})
                               : std::nullopt,
    };
}

inline PDJE_PARALLEL::BACKEND_T
DetectAvailableBackend() noexcept
{
    return PDJE_PARALLEL::detail::DetectPreferredBackend();
}

class SerialStftHarness {
  public:
    PDJE_PARALLEL::StftResult
    calculate(std::vector<float>         &pcm,
              PDJE_PARALLEL::WINDOW_LIST  target_window =
                  PDJE_PARALLEL::WINDOW_LIST::HANNING,
              const int                   windowSizeExp = 10,
              const float                 overlapRatio  = 0.5f,
              PDJE_PARALLEL::POST_PROCESS post_process  = {})
    {
        post_process.check_values();
        if (pcm.empty() || overlapRatio < 0.0f || overlapRatio >= 1.0f ||
            windowSizeExp < 6 || windowSizeExp >= 31) {
            return {};
        }

        const auto args = BuildStftArgs(
            pcm,
            windowSizeExp,
            overlapRatio,
            post_process.mel_scale
                ? std::optional<PDJE_PARALLEL::MelFilterBankSpec>(
                      BuildLegacyMelSpec(1 << windowSizeExp))
                : std::nullopt);
        return backend_.Execute(pcm,
                                target_window,
                                post_process,
                                static_cast<unsigned int>(windowSizeExp),
                                args);
    }

    PDJE_PARALLEL::StftResult
    calculate(std::vector<float> &pcm, const PDJE_PARALLEL::STFTRequest &request)
    {
        if (pcm.empty()) {
            return {};
        }

        auto normalizedRequest = request;
        normalizedRequest.post_process.check_values();
        if (normalizedRequest.mel_filter_bank.has_value()) {
            normalizedRequest.mel_filter_bank->sample_rate =
                normalizedRequest.sample_rate;
            normalizedRequest.mel_filter_bank->n_fft = normalizedRequest.n_fft;
        }

        if (normalizedRequest.n_fft <= 0 ||
            (normalizedRequest.n_fft & (normalizedRequest.n_fft - 1)) != 0) {
            return {};
        }

        const auto args = BuildRequestArgs(pcm, normalizedRequest);
        if (args.qtConst <= 0) {
            return {};
        }

        return backend_.Execute(pcm,
                                normalizedRequest.target_window,
                                normalizedRequest.post_process,
                                WindowSizeExpFromFft(normalizedRequest.n_fft),
                                args);
    }

  private:
    PDJE_PARALLEL::detail::SERIAL_STFT backend_;
};

class OpenclStftHarness {
  public:
    bool
    available() const noexcept
    {
        return backend_.active_backend() == PDJE_PARALLEL::BACKEND_T::OPENCL;
    }

    PDJE_PARALLEL::StftResult
    calculate(std::vector<float>         &pcm,
              PDJE_PARALLEL::WINDOW_LIST  target_window =
                  PDJE_PARALLEL::WINDOW_LIST::HANNING,
              const int                   windowSizeExp = 10,
              const float                 overlapRatio  = 0.5f,
              PDJE_PARALLEL::POST_PROCESS post_process  = {})
    {
        if (!available() || pcm.empty() || overlapRatio < 0.0f ||
            overlapRatio >= 1.0f || windowSizeExp < 6 || windowSizeExp >= 31) {
            return {};
        }

        return backend_.calculate(
            pcm, target_window, windowSizeExp, overlapRatio, post_process);
    }

    PDJE_PARALLEL::StftResult
    calculate(std::vector<float> &pcm, const PDJE_PARALLEL::STFTRequest &request)
    {
        if (!available()) {
            return {};
        }

        return backend_.calculate(pcm, request);
    }

  private:
    PDJE_PARALLEL::STFT backend_;
};

class FallbackStftHarness {
  public:
    FallbackStftHarness()
        : serial_backend_(
              std::make_unique<PDJE_PARALLEL::detail::SERIAL_STFT>())
    {
    }

    void
    set_opencl_backend(
        std::unique_ptr<PDJE_PARALLEL::detail::IStftBackend> backend)
    {
        opencl_backend_ = std::move(backend);
        active_backend_ = PDJE_PARALLEL::BACKEND_T::OPENCL;
    }

    PDJE_PARALLEL::BACKEND_T
    active_backend() const noexcept
    {
        return active_backend_;
    }

    PDJE_PARALLEL::StftResult
    calculate(std::vector<float>         &pcm,
              PDJE_PARALLEL::WINDOW_LIST  target_window =
                  PDJE_PARALLEL::WINDOW_LIST::HANNING,
              const int                   windowSizeExp = 10,
              const float                 overlapRatio  = 0.5f,
              PDJE_PARALLEL::POST_PROCESS post_process  = {})
    {
        post_process.check_values();
        if (pcm.empty() || overlapRatio < 0.0f || overlapRatio >= 1.0f ||
            windowSizeExp < 6 || windowSizeExp >= 31) {
            return {};
        }

        const auto args = BuildStftArgs(
            pcm,
            windowSizeExp,
            overlapRatio,
            post_process.mel_scale
                ? std::optional<PDJE_PARALLEL::MelFilterBankSpec>(
                      BuildLegacyMelSpec(1 << windowSizeExp))
                : std::nullopt);

        if (active_backend_ == PDJE_PARALLEL::BACKEND_T::OPENCL &&
            opencl_backend_) {
            try {
                auto result = opencl_backend_->Execute(
                    pcm,
                    target_window,
                    post_process,
                    static_cast<unsigned int>(windowSizeExp),
                    args);
                if (!result.first.empty() || !result.second.empty()) {
                    return result;
                }
            } catch (const std::exception &) {
            }

            opencl_backend_.reset();
            active_backend_ = PDJE_PARALLEL::BACKEND_T::SERIAL;
        }

        if (!serial_backend_) {
            return {};
        }

        return serial_backend_->Execute(pcm,
                                        target_window,
                                        post_process,
                                        static_cast<unsigned int>(windowSizeExp),
                                        args);
    }

  private:
    std::unique_ptr<PDJE_PARALLEL::detail::IStftBackend> serial_backend_;
    std::unique_ptr<PDJE_PARALLEL::detail::IStftBackend> opencl_backend_;
    PDJE_PARALLEL::BACKEND_T active_backend_ =
        PDJE_PARALLEL::BACKEND_T::SERIAL;
};

} // namespace PDJE_TEST::util
