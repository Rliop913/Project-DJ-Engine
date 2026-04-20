if(TARGET PDJE_UTIL_DB)
  return()
endif()

if(NOT TARGET PDJE_UTIL)
  add_library(PDJE_UTIL INTERFACE)
  target_compile_features(PDJE_UTIL INTERFACE cxx_std_20)
  target_include_directories(PDJE_UTIL INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  )
endif()

add_library(PDJE_UTIL_IMAGE_WEBP INTERFACE)
target_link_libraries(PDJE_UTIL_IMAGE_WEBP INTERFACE PDJE_UTIL)
setWebpReqLib(PDJE_UTIL_IMAGE_WEBP)

set(PDJE_UTIL_STFT_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/include/util/function/stft/STFT_Parallel.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/util/function/stft/BackendLess.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/util/function/stft/detail/SerialBackend.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/util/function/stft/detail/OpenclBackend.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/util/function/stft/detail/OpenCL_Loader.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/util/function/stft/detail/PDJE_Parallel_Runtime_Loader.cpp
)

set(PDJE_UTIL_IMAGE_WAVEFORM_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/include/util/function/image/WaveformWebp.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/util/function/image/detail/WaveformWebpEncoder.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/util/function/image/detail/WaveformWebpPlanBuilder.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/util/function/image/detail/WaveformWebpRasterizer.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/util/function/image/detail/WaveformWebpProcessor.cpp
)

add_library(PDJE_UTIL_IMAGE_WAVEFORM
  ${PDJE_UTIL_IMAGE_WAVEFORM_SOURCES}
  ${PDJE_UTIL_STFT_SOURCES}
)

target_include_directories(PDJE_UTIL_IMAGE_WAVEFORM PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/GenCodes/OKL/GenOut/OpenCL
  ${CMAKE_CURRENT_SOURCE_DIR}/GenCodes/OKL/GenOut/SERIAL
)

setOpenCLCppReqLib(PDJE_UTIL_IMAGE_WAVEFORM)
setOpenCLRuntimeShimReqLib(PDJE_UTIL_IMAGE_WAVEFORM)
setCmrcReqLib(PDJE_UTIL_IMAGE_WAVEFORM)

setHighwayReqLib(PDJE_UTIL_IMAGE_WAVEFORM)
setCapnpReqLib(PDJE_UTIL_IMAGE_WAVEFORM)
PDJE_COMPILE_OPTION(PDJE_UTIL_IMAGE_WAVEFORM)
SET_PROPERTIES(PDJE_UTIL_IMAGE_WAVEFORM)

add_library(PDJE_UTIL_DB_SQLITE INTERFACE)
target_link_libraries(PDJE_UTIL_DB_SQLITE INTERFACE PDJE_UTIL)
setSqliteReqLib(PDJE_UTIL_DB_SQLITE)

add_library(PDJE_UTIL_DB_ROCKSDB
  ${CMAKE_CURRENT_SOURCE_DIR}/include/util/db/backends/RocksDbBackend.cpp)
target_link_libraries(PDJE_UTIL_DB_ROCKSDB PUBLIC PDJE_UTIL)
setRocksDBReqLib(PDJE_UTIL_DB_ROCKSDB)
PDJE_COMPILE_OPTION(PDJE_UTIL_DB_ROCKSDB)
SET_PROPERTIES(PDJE_UTIL_DB_ROCKSDB)

add_library(PDJE_UTIL_DB_ANNOY INTERFACE)
target_link_libraries(PDJE_UTIL_DB_ANNOY INTERFACE PDJE_UTIL)
setAnnoyReqLib(PDJE_UTIL_DB_ANNOY)

add_library(PDJE_UTIL_DB INTERFACE)
target_link_libraries(PDJE_UTIL_DB INTERFACE
  PDJE_UTIL_DB_SQLITE
  PDJE_UTIL_DB_ROCKSDB
  PDJE_UTIL_DB_ANNOY
)

target_link_libraries(PDJE_UTIL_IMAGE_WAVEFORM PUBLIC
  PDJE_UTIL
  PDJE_UTIL_IMAGE_WEBP
)
