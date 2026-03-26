#define CL_HPP_TARGET_OPENCL_VERSION 300
#define CL_TARGET_OPENCL_VERSION 300

#include "util/common/BackendLoader/OpenCL_Loader.hpp"
#include "util/common/BackendLoader/PDJE_Parallel_Runtime_Loader.hpp"

#include <CL/opencl.hpp>
#include <doctest/doctest.h>

#include <vector>

TEST_CASE("util: opencl runtime shim caches readiness and selects backend")
{
    const bool firstLoadResult = PDJE_PARALLEL::EnsureOpenCLRuntimeLoaded();
    const bool secondLoadResult = PDJE_PARALLEL::EnsureOpenCLRuntimeLoaded();

    CHECK(firstLoadResult == secondLoadResult);

    PDJE_PARALLEL::Backend backend;
    CHECK(backend.LoadBackend());
    CHECK(backend.PrintBackendType() ==
          (firstLoadResult ? PDJE_PARALLEL::BACKEND_T::OPENCL
                           : PDJE_PARALLEL::BACKEND_T::OPENMP));

    cl_uint platformCount = 0;

    if (!firstLoadResult) {
        CHECK(::clGetPlatformIDs(0, nullptr, &platformCount) ==
              CL_PLATFORM_NOT_FOUND_KHR);
        return;
    }

    CHECK(::clGetPlatformIDs(0, nullptr, &platformCount) == CL_SUCCESS);
    CHECK(platformCount > 0);

    std::vector<cl::Platform> platforms;
    CHECK(cl::Platform::get(&platforms) == CL_SUCCESS);
    CHECK_FALSE(platforms.empty());
}
