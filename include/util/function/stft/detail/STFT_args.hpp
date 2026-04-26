#pragma once

#include "util/function/stft/MelFilterBank.hpp"

#include <optional>

namespace PDJE_PARALLEL::detail {

struct StftArgs {
    unsigned int                     FullSize = 0u;
    int                              sample_rate = 0;
    int                              windowSize  = 0;
    int                              qtConst     = 0;
    unsigned int                     OFullSize   = 0u;
    unsigned int                     OHalfSize   = 0u;
    unsigned int                     OMove       = 0u;
    std::optional<MelFilterBankSpec> mel_filter_bank {};
    bool                             dc_remove = true;
};

} // namespace PDJE_PARALLEL::detail
