#pragma once
#include "CL/opencl.hpp"
#include "PDJE_Parallel_Runtime_Loader.hpp"
#include "Parallel_Args.hpp"

#include "STFT_args.hpp"
#include <optional>
#include <utility>
#include <vector>
namespace PDJE_PARALLEL {

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

    // bool
    // CheckFusable(const WINDOW_LIST target_window, const int windowSizeEXP);

    bool
    ReadyBackend();

    bool
    PreProcess();

    bool
    Butterfly_Stage();

    bool
    PostProcess();

  public:
    Backend   backendinfo;
    BACKEND_T backend_now = BACKEND_T::OPENMP;
    STFT();

    std::pair<std::vector<float>, std::vector<float>>
    calculate(std::vector<float> &PCMdata,
              const WINDOW_LIST   target_window = WINDOW_LIST::HANNING,
              const int           windowSizeEXP = 10,
              const float         overlapRatio  = 0.5);
    ~STFT();
};
}; // namespace PDJE_PARALLEL
