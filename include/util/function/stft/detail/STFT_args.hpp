#pragma once

namespace PDJE_PARALLEL::detail {

struct StftArgs {
    unsigned int FullSize;
    int          windowSize;
    int          qtConst;
    unsigned int OFullSize;
    unsigned int OHalfSize;
    unsigned int OMove;
};

} // namespace PDJE_PARALLEL::detail
