#include "Parallel_Args.hpp"
#include "STFT_Parallel.hpp"
namespace PDJE_PARALLEL {

StftArgs
STFT::GenArgs(const std::vector<float> &inputVec,
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

// bool
// STFT::CheckFusable(const WINDOW_LIST target_window, const int windowSizeEXP)
// {
//     return target_window == WINDOW_LIST::HANNING &&
//            (windowSizeEXP == 10 || windowSizeEXP == 11);
// }

} // namespace PDJE_PARALLEL
