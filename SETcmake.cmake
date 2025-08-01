set(cmake_cxx_standard 20)
set(cmake_cxx_flags -std=c++20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(HWY_ENABLE_TESTS OFF)
set(CAPNP_BUILD_TESTS OFF)
set(BUILD_TESTING OFF CACHE BOOL "Disable testing")
if(APPLE)
set(HWY_ENABLE_TARGETS "scalar,arm64" CACHE STRING "Enabled SIMD targets" FORCE)
else()
set(HWY_ENABLE_TARGETS "scalar,sse4,avx,avx2" CACHE STRING "Enabled SIMD targets" FORCE)
endif()
set(WITH_WERROR OFF CACHE BOOL "" FORCE)
set(FAIL_ON_WARNINGS OFF CACHE BOOL "Disable Werror in rocksdb")
set(CMAKE_C_COMPILER_LAUNCHER ccache)
set(CMAKE_CXX_COMPILER_LAUNCHER ccache)


if(MSVC)
add_compile_options(
    /arch:AVX2
)
add_compile_options(/W3 /GR /WX-)
elseif(APPLE)

add_compile_options(
  -mcpu=apple-m1
  -fvectorize
  -ffast-math
)
else()

add_compile_options(
  -march=haswell
  -msse4.1
  -mavx
  -mavx2
  -mfma
)
add_compile_options(-frtti)
endif()
if(WIN32)

    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    # set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
    set(cmake_c_compiler "cl")
    set(cmake_cxx_compiler "cl")
else()
    set(cmake_c_compiler clang)
    set(cmake_cxx_compiler clang++)
endif()
if(CMAKE_BUILD_TYPE STREQUAL "Debug")

    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g -O1 -fno-omit-frame-pointer")
    # set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -g -O1 -fno-omit-frame-pointer")
    # set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=address")
    # if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    #   add_compile_options(
    #       -Wall            # 일반 경고
    #     #   -Wextra          # 추가 경고
    #     #   -Wpedantic       # 표준에 엄격하게 따르는 경고
    #       -Wshadow         # 변수 이름 겹침 경고
    #       -Wnull-dereference  # nullptr 역참조 경고 (GCC 6+)
    #       -fsanitize=undefined # 런타임에서 정의되지 않은 동작 탐지
    #       -fno-strict-aliasing # 알리아싱 최적화 비활성화 (UB 방지)
    #   )
    # endif()
    
    if(NOT WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O1 -fsanitize=address")
    endif()
    set_property(SOURCE PDJE_swig.i PROPERTY CPLUSPLUS ON)
else()
    set_property(SOURCE PDJE_swig.i PROPERTY CPLUSPLUS ON)
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
endif()
