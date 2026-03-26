#pragma once
#include "PDJE_Parallel_Runtime_Loader.hpp"

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
    HAMMING
};

class STFT {
  private:
    struct StftArgs {
        unsigned int FullSize;
        int          windowSize;
        int          qtConst;
        unsigned int OFullSize;
        unsigned int OHalfSize;
        unsigned int OMove;
    };

    StftArgs
    GenArgs(const std::vector<float> &inputVec,
            const int                 windowSizeEXP,
            const float               overlapRatio)
    {
        StftArgs arglist;
        arglist.FullSize   = static_cast<unsigned int>(inputVec.size());
        arglist.windowSize = 1 << windowSizeEXP;
        arglist.qtConst =
            toQuot(arglist.FullSize, overlapRatio, arglist.windowSize);
        arglist.OFullSize = arglist.qtConst * arglist.windowSize;
        arglist.OHalfSize = arglist.OFullSize / 2;
        arglist.OMove     = arglist.windowSize * (1.0f - overlapRatio);
        return arglist;
    }

  public:
    Backend backendinfo;
    STFT();

    std::pair<std::vector<float>, std::vector<float>>
    calculate(std::vector<float> &PCMdata,
              const WINDOW_LIST   target_window = WINDOW_LIST::HANNING,
              const int           windowSizeEXP = 10,
              const float         overlapRatio  = 0.5);
    ~STFT();
};
}; // namespace PDJE_PARALLEL
