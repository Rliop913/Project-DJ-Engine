#include "util/function/stft/detail/PDJE_Parallel_Runtime_Loader.hpp"

#include "util/function/stft/detail/OpenCL_Loader.hpp"

namespace PDJE_PARALLEL::detail {

BACKEND_T
DetectPreferredBackend() noexcept
{
    return EnsureOpenCLRuntimeLoaded() ? BACKEND_T::OPENCL : BACKEND_T::SERIAL;
}

} // namespace PDJE_PARALLEL::detail
