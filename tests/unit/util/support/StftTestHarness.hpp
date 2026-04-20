#pragma once

#include "util/function/stft/STFT_Parallel.hpp"
#include "util/function/stft/detail/PDJE_Parallel_Runtime_Loader.hpp"
#include "util/function/stft/detail/SerialBackend.hpp"
#include "util/function/stft/detail/StftBackend.hpp"

#include <exception>
#include <memory>
#include <vector>

namespace PDJE_TEST::util {

inline PDJE_PARALLEL::detail::StftArgs
BuildStftArgs(const std::vector<float> &inputVec,
              const int                 windowSizeExp,
              const float               overlapRatio)
{
    PDJE_PARALLEL::detail::StftArgs arglist;
    arglist.FullSize   = static_cast<unsigned int>(inputVec.size());
    arglist.windowSize = 1 << windowSizeExp;
    arglist.qtConst =
        PDJE_PARALLEL::toQuot(arglist.FullSize, overlapRatio, arglist.windowSize);
    arglist.OFullSize = arglist.qtConst * arglist.windowSize;
    arglist.OHalfSize = arglist.OFullSize / 2;
    arglist.OMove =
        static_cast<unsigned int>(arglist.windowSize * (1.0f - overlapRatio));
    return arglist;
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
        if (pcm.empty() || overlapRatio < 0.0f || overlapRatio >= 1.0f ||
            windowSizeExp < 6 || windowSizeExp >= 31) {
            return {};
        }

        const auto args = BuildStftArgs(pcm, windowSizeExp, overlapRatio);
        return backend_.Execute(pcm,
                                target_window,
                                post_process,
                                static_cast<unsigned int>(windowSizeExp),
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
        if (pcm.empty() || overlapRatio < 0.0f || overlapRatio >= 1.0f ||
            windowSizeExp < 6 || windowSizeExp >= 31) {
            return {};
        }

        const auto args = BuildStftArgs(pcm, windowSizeExp, overlapRatio);

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
