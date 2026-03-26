#include "PDJE_Parallel_Runtime_Loader.hpp"
#include "OpenCL_Loader.hpp"

namespace PDJE_PARALLEL {

Backend::Backend() : backend_now(BACKEND_T::OPENMP)
{
}

Backend::~Backend() = default;

BACKEND_T
Backend::PrintBackendType()
{
    return backend_now;
}

bool
Backend::LoadBackend()
{
    backend_now =
        EnsureOpenCLRuntimeLoaded() ? BACKEND_T::OPENCL : BACKEND_T::OPENMP;
    return true;
}

}; // namespace PDJE_PARALLEL
