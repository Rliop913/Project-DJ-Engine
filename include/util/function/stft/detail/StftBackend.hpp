#pragma once

#include "util/function/stft/STFT_Parallel.hpp"
#include "util/function/stft/detail/STFT_args.hpp"

namespace PDJE_PARALLEL::detail {

class IStftBackend {
  public:
    virtual ~IStftBackend() = default;

    virtual StftResult
    Execute(std::vector<float> &PCMdata,
            WINDOW_LIST         target_window,
            POST_PROCESS        post_process,
            unsigned int        windowSizeEXP,
            const StftArgs     &gargs) = 0;
};

} // namespace PDJE_PARALLEL::detail
