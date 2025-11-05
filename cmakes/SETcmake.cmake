
set(cmake_cxx_standard 20)
set(cmake_cxx_flags -std=c++20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(HWY_ENABLE_TESTS OFF)
set(CAPNP_BUILD_TESTS OFF)
set(BUILD_TESTING OFF CACHE BOOL "Disable testing")
if(APPLE)
set(HWY_ENABLE_TARGETS "NEON,scalar" CACHE STRING "Enabled SIMD targets" FORCE)
else()
set(HWY_ENABLE_TARGETS "scalar,sse4,avx,avx2" CACHE STRING "Enabled SIMD targets" FORCE)
endif()
set(WITH_WERROR OFF CACHE BOOL "" FORCE)
set(FAIL_ON_WARNINGS OFF CACHE BOOL "Disable Werror in rocksdb")
set(CMAKE_C_COMPILER_LAUNCHER ccache)
set(CMAKE_CXX_COMPILER_LAUNCHER ccache)


# if(MSVC)
# set(CMAKE_INCLUDE_SYSTEM_FLAG_MSVC "")
# add_compile_options(
#     /arch:AVX2
#     /permissive- 
# )
# add_compile_options(/W3 /GR /WX-)
# elseif(APPLE)
# set(CMAKE_OSX_ARCHITECTURES arm64 CACHE STRING "" FORCE)
# add_compile_options(
#   -mcpu=apple-m1
#   -fvectorize
#   -ffast-math
# )
# add_compile_options(
#   -frtti
#   -Wabi
#   -Wweak-vtables
#   -Wnon-virtual-dtor
#   -Wpadded -Wpacked -Wpragma-pack
#   )
# else()

# add_compile_options(
#   -march=haswell
#   -msse4.1
#   -mavx
#   -mavx2
#   -mfma
# )
# add_compile_options(
#   -frtti
#   -Wabi
#   -Wweak-vtables
#   -Wnon-virtual-dtor
#   -Wpadded -Wpacked -Wpragma-pack
#   )

# endif()
function(PDJE_COMPILE_OPTION targetName)
  

target_compile_options(${targetName} PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/permissive- /WX- /W3 /GR>
  $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-fno-exceptions>
)

# MSVC
target_compile_options(${targetName} PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W3>
)

target_compile_options(${targetName} PRIVATE
  $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:
    -Wnon-virtual-dtor
    -Wweak-vtables
    -Wpadded -Wpacked -Wpragma-pack
    -Wabi
    -frtti
  >
)

# MSVC x86/x64
target_compile_options(${targetName} PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/arch:AVX2>
)

# Apple Silicon (arm64)
target_compile_options(${targetName} PRIVATE
  $<$<AND:$<CXX_COMPILER_ID:AppleClang>,$<STREQUAL:${CMAKE_SYSTEM_PROCESSOR},arm64>>:
    -mcpu=apple-m1 -fvectorize -ffast-math
  >
)

target_compile_options(${targetName} PRIVATE
  $<$<AND:$<CXX_COMPILER_ID:GNU,Clang>,$<OR:$<STREQUAL:${CMAKE_SYSTEM_PROCESSOR},x86_64>,$<STREQUAL:${CMAKE_SYSTEM_PROCESSOR},AMD64>>>:
    -march=haswell -msse4.1 -mavx -mavx2 -mfma
  >
)

endfunction(PDJE_COMPILE_OPTION)
# if(WIN32)

#     set(CMAKE_CXX_STANDARD 20)
#     set(CMAKE_CXX_STANDARD_REQUIRED ON)
#     # set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
#     set(cmake_c_compiler "cl")
#     set(cmake_cxx_compiler "cl")
# else()
#     set(cmake_c_compiler clang)
#     set(cmake_cxx_compiler clang++)
# endif()
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    
    if(NOT WIN32)
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O1 -fsanitize=address")
    endif()
    set_property(SOURCE PDJE_swig.i PROPERTY CPLUSPLUS ON)
else()
    set_property(SOURCE PDJE_swig.i PROPERTY CPLUSPLUS ON)
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
endif()
