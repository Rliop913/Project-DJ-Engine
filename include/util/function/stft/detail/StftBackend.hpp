#pragma once

#include "util/function/stft/STFT_Parallel.hpp"
#include "util/function/stft/detail/STFT_args.hpp"

namespace PDJE_PARALLEL::detail {

class IStftBackend {
  public:
    virtual ~IStftBackend() = default;

    struct Execution {
        std::vector<float> &pcm;
        WINDOW_LIST         window;
        POST_PROCESS        post_process;
        unsigned int        window_size_exp;
        const StftArgs     &args;
    };

    virtual StftResult
    Execute(const Execution &execution) = 0;
};

} // namespace PDJE_PARALLEL::detail
