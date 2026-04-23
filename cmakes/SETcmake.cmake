
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
option(PDJE_USE_CCACHE "Use ccache compiler launcher when available" ON)
if(PDJE_USE_CCACHE)
  find_program(PDJE_CCACHE_PROGRAM ccache)
  if(PDJE_CCACHE_PROGRAM)
    set(PDJE_CCACHE_TMPDIR "${CMAKE_BINARY_DIR}/.ccache-tmp")
    file(MAKE_DIRECTORY "${PDJE_CCACHE_TMPDIR}")
    set(
      PDJE_CCACHE_LAUNCHER
      ${CMAKE_COMMAND}
      -E
      env
      "CCACHE_TEMPDIR=${PDJE_CCACHE_TMPDIR}"
      ${PDJE_CCACHE_PROGRAM})
    set(CMAKE_C_COMPILER_LAUNCHER ${PDJE_CCACHE_LAUNCHER})
    set(CMAKE_CXX_COMPILER_LAUNCHER ${PDJE_CCACHE_LAUNCHER})
  endif()
endif()

if(NOT PDJE_RTTI_FORCE_OFF)
  # Keep RTTI on only for the symbol-bearing dev config across single- and multi-config generators.
  add_compile_options(
    "$<$<AND:$<COMPILE_LANGUAGE:CXX>,$<CXX_COMPILER_ID:GNU,Clang,AppleClang>,$<CONFIG:RelWithDebInfo>>:-frtti>")
endif()

function(SET_PROPERTIES targetname)
    
  if(APPLE)
  set_target_properties(${targetname} PROPERTIES
      MACOSX_RPATH ON
      BUILD_RPATH "@loader_path"
  )
  target_link_options(${targetname} PRIVATE
      "-Wl,-rpath,@loader_path"
  )

  elseif(UNIX)
  set_target_properties(${targetname} PROPERTIES
      BUILD_RPATH "\$ORIGIN"
  )
  target_link_options(${targetname} PRIVATE
  "-Wl,--enable-new-dtags"
  "-Wl,-rpath,\$ORIGIN"
  )

  endif()

endfunction(SET_PROPERTIES)


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

function(PDJE_COMPILE_OPTION targetName)
target_compile_options(${targetName} PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/permissive- /WX- /W3 /GR /EHsc /arch:AVX2>
  $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:
    -Wnon-virtual-dtor
    -Wpacked -Wpragma-pack
    -Wabi
  >
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
set_property(SOURCE PDJE_swig.i PROPERTY CPLUSPLUS ON)

if(NOT WIN32)
  string(APPEND CMAKE_CXX_FLAGS_RELWITHDEBINFO_INIT " -O3")
endif()
