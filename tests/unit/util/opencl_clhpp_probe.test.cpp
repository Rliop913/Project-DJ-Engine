#define CL_HPP_TARGET_OPENCL_VERSION 300
#define CL_TARGET_OPENCL_VERSION 300

#include "util/function/stft/detail/OpenclBackend.hpp"
#include <CL/opencl.hpp>
#include <doctest/doctest.h>

namespace PDJE_TEST::util {

} // namespace PDJE_TEST::util

namespace {

#if defined(__has_include)
#if !__has_include(<CL/cl2.hpp>)
#error "FetchContent OpenCL-CLHPP import must expose <CL/cl2.hpp>."
#endif
#endif

constexpr bool kHasOpenClApiWrapperMacro =
#if defined(CL_HPP_OPENCL_API_WRAPPER)
    true;
#else
    false;
#endif
    ;

constexpr bool kHasInitExtFcnPtrMacro =
#if defined(CL_HPP_INIT_CL_EXT_FCN_PTR_)
    true;
#else
    false;
#endif
    ;

constexpr bool kHasInitExtFcnPtrPlatformMacro =
#if defined(CL_HPP_INIT_CL_EXT_FCN_PTR_PLATFORM_)
    true;
#else
    false;
#endif
    ;

} // namespace

TEST_CASE("util: opencl clhpp fetchcontent import exposes headers")
{
    CHECK(CL_HPP_TARGET_OPENCL_VERSION == 300);
    CHECK(CL_TARGET_OPENCL_VERSION == 300);
}

TEST_CASE("util: opencl clhpp helper macros are not exported after include")
{
    CHECK_FALSE(kHasInitExtFcnPtrMacro);
    CHECK_FALSE(kHasInitExtFcnPtrPlatformMacro);
}

TEST_CASE("util: opencl clhpp probe records api wrapper macro state")
{
    INFO("CL_HPP_OPENCL_API_WRAPPER defined: "
         << (kHasOpenClApiWrapperMacro ? "true" : "false"));
    CHECK_FALSE(kHasOpenClApiWrapperMacro);
}

TEST_CASE("util: opencl clhpp compiles with CL_NO_PROTOTYPES")
{
    INFO("CMake compile probe result for CL_NO_PROTOTYPES: "
         << PDJE_OPENCL_CLHPP_SUPPORTS_NO_PROTOTYPES);
    CHECK(PDJE_OPENCL_CLHPP_SUPPORTS_NO_PROTOTYPES == 0);
}

TEST_CASE("util: opencl clhpp compiles with CL_NO_CORE_PROTOTYPES")
{
    INFO("CMake compile probe result for CL_NO_CORE_PROTOTYPES: "
         << PDJE_OPENCL_CLHPP_SUPPORTS_NO_CORE_PROTOTYPES);
    CHECK(PDJE_OPENCL_CLHPP_SUPPORTS_NO_CORE_PROTOTYPES == 0);
}
