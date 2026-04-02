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
    arglist.OMove =
        static_cast<unsigned int>(arglist.windowSize * (1.0f - overlapRatio));
    return arglist;
}



} // namespace PDJE_PARALLEL
