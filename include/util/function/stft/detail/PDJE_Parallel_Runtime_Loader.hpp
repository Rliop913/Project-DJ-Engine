#pragma once

#include "util/function/stft/STFT_Parallel.hpp"

namespace PDJE_PARALLEL::detail {

BACKEND_T
DetectPreferredBackend() noexcept;

} // namespace PDJE_PARALLEL::detail
