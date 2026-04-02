#pragma once
#include "CL/opencl.hpp"
#include "PDJE_Parallel_Runtime_Loader.hpp"
#include "Parallel_Args.hpp"

#include "STFT_args.hpp"
#include <memory>
#include <utility>
#include <vector>
namespace PDJE_PARALLEL {

class OPENCL_STFT;

inline int
toQuot(float fullSize, float overlapRatio, float windowSize)
{
    if (overlapRatio == 0.0f) {
        return fullSize / windowSize + 1;
    } else {
        return ((fullSize) / (windowSize * (1.0f - overlapRatio))) + 1;
    }
}

enum WINDOW_LIST {
    BLACKMAN,
    BLACKMAN_HARRIS,
    BLACKMAN_NUTTALL,
    HANNING,
    NUTTALL,
    FLATTOP,
    GAUSSIAN,
    HAMMING,
    NONE
};

enum class POST_PROCESS{
    POWER = 0,
    TO_DB,
    MEL_SCALE,
    KERNEL_CHAIN__POWER_MEL_DB,
    NONE
};

static inline POST_PROCESS
NormalizePostProcess(const POST_PROCESS post_process)
{
    switch (post_process) {
    case POST_PROCESS::POWER:
    case POST_PROCESS::NONE:
        return post_process;
    case POST_PROCESS::TO_DB:
    case POST_PROCESS::MEL_SCALE:
    case POST_PROCESS::KERNEL_CHAIN__POWER_MEL_DB:
    default:
        return POST_PROCESS::NONE;
    }
}

class STFT {
  private:
    StftArgs
    GenArgs(const std::vector<float> &inputVec,
            const int                 windowSizeEXP,
            const float               overlapRatio);

    std::unique_ptr<OPENCL_STFT> opencl_backend;

    std::vector<float> mel_filter_matrix;
    void
    GenMelFilter(const uint32_t window_exp_sz);

  public:
    Backend   backendinfo;
    BACKEND_T backend_now = BACKEND_T::SERIAL;
    STFT();

    std::pair<std::vector<float>, std::vector<float>>
    calculate(std::vector<float> &PCMdata,
              const WINDOW_LIST   target_window = WINDOW_LIST::HANNING,
              const int           windowSizeEXP = 10,
              const float         overlapRatio  = 0.5,
              const POST_PROCESS  post_process  = POST_PROCESS::NONE);
    ~STFT();
};
}; // namespace PDJE_PARALLEL
