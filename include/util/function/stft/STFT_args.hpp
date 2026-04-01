#pragma once
namespace PDJE_PARALLEL {
struct StftArgs {
    unsigned int FullSize;
    int          windowSize;
    int          qtConst;
    unsigned int OFullSize;
    unsigned int OHalfSize;
    unsigned int OMove;
};
} // namespace PDJE_PARALLEL