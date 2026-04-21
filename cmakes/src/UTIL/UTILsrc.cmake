if(TARGET PDJE_UTIL)
  return()
endif()

set(PDJE_UTIL_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/include/util/db/backends/RocksDbBackend.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/util/function/stft/STFT_Parallel.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/util/function/stft/BackendLess.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/util/function/stft/detail/SerialBackend.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/util/function/stft/detail/OpenclBackend.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/util/function/stft/detail/OpenCL_Loader.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/util/function/stft/detail/PDJE_Parallel_Runtime_Loader.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/util/function/image/WaveformWebp.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/util/function/image/detail/WaveformWebpEncoder.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/util/function/image/detail/WaveformWebpPlanBuilder.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/util/function/image/detail/WaveformWebpRasterizer.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/util/function/image/detail/WaveformWebpProcessor.cpp
)

if(PDJE_DYNAMIC)
  add_library(PDJE_UTIL SHARED ${PDJE_UTIL_SOURCES})
  set_target_properties(PDJE_UTIL PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN ON
  )
else()
  add_library(PDJE_UTIL ${PDJE_UTIL_SOURCES})
endif()

target_compile_features(PDJE_UTIL PUBLIC cxx_std_20)
target_include_directories(PDJE_UTIL
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
target_include_directories(PDJE_UTIL PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/GenCodes/OKL/GenOut/OpenCL
  ${CMAKE_CURRENT_SOURCE_DIR}/GenCodes/OKL/GenOut/SERIAL
)
DynamicInnerFlag(PDJE_UTIL)
DynamicOuterFlag(PDJE_UTIL)
setWebpReqLib(PDJE_UTIL PUBLIC)
setSqliteReqLib(PDJE_UTIL PUBLIC)
setAnnoyReqLib(PDJE_UTIL PUBLIC)
setRocksDBReqLib(PDJE_UTIL)
setOpenCLCppReqLib(PDJE_UTIL)
setOpenCLRuntimeShimReqLib(PDJE_UTIL)
setCmrcReqLib(PDJE_UTIL)
setHighwayReqLib(PDJE_UTIL)
setCapnpReqLib(PDJE_UTIL)
PDJE_COMPILE_OPTION(PDJE_UTIL)
SET_PROPERTIES(PDJE_UTIL)
