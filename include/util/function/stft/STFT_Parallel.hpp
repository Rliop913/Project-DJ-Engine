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

class STFT {
  private:
    StftArgs
    GenArgs(const std::vector<float> &inputVec,
            const int                 windowSizeEXP,
            const float               overlapRatio);

    std::unique_ptr<OPENCL_STFT> opencl_backend;

  public:
    Backend   backendinfo;
    BACKEND_T backend_now = BACKEND_T::OPENMP;
    STFT();

    std::pair<std::vector<float>, std::vector<float>>
    calculate(std::vector<float> &PCMdata,
              const WINDOW_LIST   target_window = WINDOW_LIST::HANNING,
              const int           windowSizeEXP = 10,
              const float         overlapRatio  = 0.5,
              const bool          toPower       = false);
    ~STFT();
};
}; // namespace PDJE_PARALLEL
