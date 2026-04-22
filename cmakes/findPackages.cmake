FetchContent_Declare(
  miniaudio
  GIT_REPOSITORY https://github.com/mackron/miniaudio.git
  GIT_TAG 0.11.21
)

find_package(highway CONFIG REQUIRED)

function(setHighwayReqLib targetName)
  target_link_libraries(${targetName} PRIVATE
  highway::hwy
  )
  target_include_directories(${targetName} PRIVATE ${highway_INCLUDE_DIR})
endfunction(setHighwayReqLib)

find_package(CapnProto CONFIG REQUIRED)
set(SCHEMAS
  ${CMAKE_CURRENT_SOURCE_DIR}/third_party/Capnp/MixBinary.capnp
  ${CMAKE_CURRENT_SOURCE_DIR}/third_party/Capnp/MusicBinary.capnp
  ${CMAKE_CURRENT_SOURCE_DIR}/third_party/Capnp/NoteBinary.capnp
)

set(CAPNPC_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/capnpGen CACHE STRING "" FORCE)
file(MAKE_DIRECTORY ${CAPNPC_OUTPUT_DIR})

capnp_generate_cpp(CAPNP_SRCS CAPNP_HDRS 
  ${SCHEMAS}
)

# Ensure schema codegen is materialized before targets that only include the
# generated headers (without directly compiling CAPNP_SRCS).
add_custom_target(pdje_capnp_codegen DEPENDS ${CAPNP_SRCS} ${CAPNP_HDRS})

function(setCapnpReqLib targetName)
  add_dependencies(${targetName} pdje_capnp_codegen)
  target_link_libraries(${targetName} PRIVATE 
  CapnProto::kj 
  CapnProto::capnp 
  CapnProto::capnpc 
  CapnProto::kj-gzip)
  target_include_directories(${targetName} PRIVATE ${CAPNPC_OUTPUT_DIR})
endfunction(setCapnpReqLib)


FetchContent_Declare(
  NHJson
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.12.0
)

FetchContent_Declare(
  opencl_headers
  GIT_REPOSITORY https://github.com/KhronosGroup/OpenCL-Headers.git
  GIT_TAG v2024.10.24
  GIT_SHALLOW TRUE
)

FetchContent_Declare(
  opencl_clhpp
  GIT_REPOSITORY https://github.com/KhronosGroup/OpenCL-CLHPP.git
  GIT_TAG v2024.10.24
  GIT_SHALLOW TRUE
)

FetchContent_Declare(
  cmrc
  GIT_REPOSITORY https://github.com/vector-of-bool/cmrc.git
  GIT_TAG 2.0.1
  GIT_SHALLOW TRUE
)

FetchContent_Declare(
  sql_amalgam
  URL https://www.sqlite.org/2024/sqlite-amalgamation-3450000.zip
)

FetchContent_Declare(
  cppCodec
  GIT_REPOSITORY https://github.com/tplgy/cppcodec.git
  GIT_TAG v0.2
)

FetchContent_Declare(
    libremidi
    GIT_REPOSITORY https://github.com/celtera/libremidi
    GIT_TAG        v5.3.1
)

set(BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(WITH_LSR_BINDINGS OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(WITH_OPENMP OFF CACHE BOOL "" FORCE)
set(WITH_DEV_TRACE OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
  pysoxr_cpp
  GIT_REPOSITORY https://github.com/dofuuz/soxr.git
  GIT_TAG "a66f3eeeeb62a32403ff143b756eed92b1ec6b62"
  GIT_SHALLOW TRUE
  UPDATE_DISCONNECTED TRUE
)
FetchContent_MakeAvailable(pysoxr_cpp)
function(setPyCustomSoxr targetName)
  
  target_link_libraries(${targetName} PRIVATE 
  soxr)
  target_include_directories(${targetName} PRIVATE 
    "${pysoxr_cpp_SOURCE_DIR}/src"
    "${pysoxr_cpp_BINARY_DIR}")
endfunction(setPyCustomSoxr)
  
  set(WEBP_BUILD_ANIM_UTILS OFF CACHE BOOL "" FORCE)
  set(WEBP_BUILD_CWEBP OFF CACHE BOOL "" FORCE)
set(WEBP_BUILD_DWEBP OFF CACHE BOOL "" FORCE)
set(WEBP_BUILD_GIF2WEBP OFF CACHE BOOL "" FORCE)
set(WEBP_BUILD_IMG2WEBP OFF CACHE BOOL "" FORCE)
set(WEBP_BUILD_VWEBP OFF CACHE BOOL "" FORCE)
set(WEBP_BUILD_WEBPINFO OFF CACHE BOOL "" FORCE)
set(WEBP_BUILD_EXTRAS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    libwebp
    GIT_REPOSITORY https://github.com/webmproject/libwebp.git
    GIT_TAG        v1.5.0
)

if(PDJE_TEST)
FetchContent_Declare(
    doctest
    GIT_REPOSITORY https://github.com/doctest/doctest.git
    GIT_TAG        v2.4.11
)
endif()

function(setLibreMIDIReqLib targetName)
  target_link_libraries(${targetName} PRIVATE libremidi)
endfunction(setLibreMIDIReqLib)

find_package(ZLIB REQUIRED)

function(pdje_resolve_dependency_scope outVar targetName defaultConcreteScope requestedScope)
  if(NOT "${requestedScope}" STREQUAL "")
    set(_pdje_resolved_scope "${requestedScope}")
  else()
    get_target_property(_pdje_target_type ${targetName} TYPE)
    if("${_pdje_target_type}" STREQUAL "INTERFACE_LIBRARY")
      set(_pdje_resolved_scope INTERFACE)
    else()
      set(_pdje_resolved_scope "${defaultConcreteScope}")
    endif()
  endif()

  set(${outVar} "${_pdje_resolved_scope}" PARENT_SCOPE)
endfunction()

include(cmakes/OnnxRuntimePrebuilt.cmake)

function(setWebpReqLib targetName)
  set(_pdje_requested_scope "")
  if(ARGC GREATER 1)
    set(_pdje_requested_scope "${ARGV1}")
  endif()

  set(_pdje_webp_targets "")
  foreach(_pdje_webp_candidate
      webp
      WebP::webp)
    if(TARGET ${_pdje_webp_candidate})
      list(APPEND _pdje_webp_targets ${_pdje_webp_candidate})
      break()
    endif()
  endforeach()

  foreach(_pdje_webpdecoder_candidate
      webpdecoder
      WebP::webpdecoder)
    if(TARGET ${_pdje_webpdecoder_candidate})
      list(APPEND _pdje_webp_targets ${_pdje_webpdecoder_candidate})
      break()
    endif()
  endforeach()

  if("${_pdje_webp_targets}" STREQUAL "")
    message(FATAL_ERROR "libwebp targets not found after FetchContent_MakeAvailable(libwebp)")
  endif()

  pdje_resolve_dependency_scope(
    _pdje_webp_scope
    ${targetName}
    PRIVATE
    "${_pdje_requested_scope}")
  target_link_libraries(${targetName} ${_pdje_webp_scope} ${_pdje_webp_targets})
endfunction(setWebpReqLib)



find_package(botan CONFIG REQUIRED)

function(setBotanReqLib targetName)
  target_link_libraries(${targetName} PRIVATE botan::botan)
  
  if(DEFINED botan_INCLUDE_DIRS)
    target_include_directories(${targetName} PRIVATE ${botan_INCLUDE_DIRS})
    # message(FATAL_ERROR "${botan_INCLUDE_DIRS}")
  elseif(DEFINED botan_INCLUDE_DIR)
    target_include_directories(${targetName} PRIVATE ${botan_INCLUDE_DIR})
  # else()
    endif()
endfunction()


find_package(Annoy CONFIG REQUIRED)

function(setAnnoyReqLib targetName)
  set(_pdje_requested_scope "")
  if(ARGC GREATER 1)
    set(_pdje_requested_scope "${ARGV1}")
  endif()

  pdje_resolve_dependency_scope(
    _pdje_annoy_scope
    ${targetName}
    PRIVATE
    "${_pdje_requested_scope}")

  target_link_libraries(${targetName} ${_pdje_annoy_scope} Annoy::Annoy)
  if(DEFINED Annoy_INCLUDE_DIRS)
    target_include_directories(${targetName} ${_pdje_annoy_scope} ${Annoy_INCLUDE_DIRS})
  elseif(DEFINED Annoy_INCLUDE_DIR)
    target_include_directories(${targetName} ${_pdje_annoy_scope} ${Annoy_INCLUDE_DIR})
  endif()
endfunction(setAnnoyReqLib)

find_package(spdlog CONFIG REQUIRED)

function(setSpdlogReqLib targetName)
  if(TARGET spdlog::spdlog)
    target_link_libraries(${targetName} PRIVATE spdlog::spdlog)
  else()
    target_link_libraries(${targetName} PRIVATE spdlog::spdlog_header_only)
  endif()
  target_include_directories(${targetName} PRIVATE ${spdlog_INCLUDE_DIR})
  target_compile_definitions(${targetName} PRIVATE SPDLOG_FMT_EXTERNAL)
endfunction(setSpdlogReqLib)



find_package(libgit2 CONFIG REQUIRED)

function(setLibgit2ReqLib targetName)
  target_include_directories(${targetName} PRIVATE ${libgit2_INCLUDE_DIR} )
  target_link_libraries(${targetName} PRIVATE libgit2::libgit2)
endfunction(setLibgit2ReqLib)

find_package(RocksDB REQUIRED)

function(setRocksDBReqLib targetName)
  get_target_property(_pdje_target_type ${targetName} TYPE)
  
  if(TARGET RocksDB::rocksdb-shared)
      target_link_libraries(${targetName} PRIVATE RocksDB::rocksdb-shared)
  elseif(TARGET RocksDB::rocksdb)
      target_link_libraries(${targetName} PRIVATE RocksDB::rocksdb)
  else()
    message(FATAL_ERROR "No suitable RocksDB target found")
  endif()
  if(DEFINED RocksDB_INCLUDE_DIRS)
    target_include_directories(${targetName} PRIVATE ${RocksDB_INCLUDE_DIRS})
  elseif(DEFINED RocksDB_INCLUDE_DIR)
    target_include_directories(${targetName} PRIVATE ${RocksDB_INCLUDE_DIR})
  endif()
  
endfunction(setRocksDBReqLib)

find_package(OpenSSL REQUIRED)
link_libraries(${OPENSSL_LIBRARIES})





FetchContent_MakeAvailable(opencl_headers)
FetchContent_MakeAvailable(cmrc)

function(setCmrcReqLib targetName)
  set(_pdje_requested_scope "")
  if(ARGC GREATER 1)
    set(_pdje_requested_scope "${ARGV1}")
  endif()

  pdje_resolve_dependency_scope(
    _pdje_cmrc_scope
    ${targetName}
    PRIVATE
    "${_pdje_requested_scope}")

  target_link_libraries(${targetName} ${_pdje_cmrc_scope} PDJE::util_okl_resources)
endfunction()


cmrc_add_resource_library(
  PDJE_UTIL_OKL_RESOURCES
  ALIAS PDJE::util_okl_resources
  NAMESPACE pdje_okl
  WHENCE ${CMAKE_CURRENT_SOURCE_DIR}/GenCodes/OKL/GenOut/OpenCL
  ${CMAKE_CURRENT_SOURCE_DIR}/GenCodes/OKL/GenOut/OpenCL/STFT_MAIN.cl
)
if(NOT COMMAND cmrc_add_resource_library)
  include("${cmrc_SOURCE_DIR}/CMakeRC.cmake")
endif()


FetchContent_MakeAvailable(miniaudio)
FetchContent_MakeAvailable(NHJson)
if(DEFINED CMAKE_CXX_STANDARD)
  set(_pdje_saved_cxx_standard "${CMAKE_CXX_STANDARD}")
  set(_pdje_had_cxx_standard TRUE)
endif()
if(DEFINED CMAKE_CXX_STANDARD_REQUIRED)
  set(_pdje_saved_cxx_standard_required "${CMAKE_CXX_STANDARD_REQUIRED}")
  set(_pdje_had_cxx_standard_required TRUE)
endif()
if(DEFINED BUILD_DOCS)
  set(_pdje_saved_build_docs "${BUILD_DOCS}")
  set(_pdje_had_build_docs TRUE)
endif()
if(DEFINED BUILD_EXAMPLES)
  set(_pdje_saved_build_examples "${BUILD_EXAMPLES}")
  set(_pdje_had_build_examples TRUE)
endif()
set(BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(opencl_clhpp)
if(_pdje_had_cxx_standard)
  set(CMAKE_CXX_STANDARD "${_pdje_saved_cxx_standard}")
endif()
if(_pdje_had_cxx_standard_required)
  set(CMAKE_CXX_STANDARD_REQUIRED "${_pdje_saved_cxx_standard_required}")
endif()
if(_pdje_had_build_docs)
  set(BUILD_DOCS "${_pdje_saved_build_docs}" CACHE BOOL "" FORCE)
else()
  unset(BUILD_DOCS CACHE)
endif()
if(_pdje_had_build_examples)
  set(BUILD_EXAMPLES "${_pdje_saved_build_examples}" CACHE BOOL "" FORCE)
else()
  unset(BUILD_EXAMPLES CACHE)
endif()
FetchContent_MakeAvailable(sql_amalgam)
FetchContent_MakeAvailable(cppCodec)
FetchContent_MakeAvailable(libremidi)
FetchContent_MakeAvailable(libwebp)
if(PDJE_TEST)
FetchContent_MakeAvailable(doctest)
endif()

if(NOT TARGET PDJE_SQLITE3_AMALGAM)
  add_library(PDJE_SQLITE3_AMALGAM STATIC ${sql_amalgam_SOURCE_DIR}/sqlite3.c)
  set_target_properties(PDJE_SQLITE3_AMALGAM PROPERTIES POSITION_INDEPENDENT_CODE ON)
  target_include_directories(PDJE_SQLITE3_AMALGAM PRIVATE ${sql_amalgam_SOURCE_DIR})
endif()

function(setSqliteReqLib targetName)
  set(_pdje_requested_scope "")
  if(ARGC GREATER 1)
    set(_pdje_requested_scope "${ARGV1}")
  endif()

  pdje_resolve_dependency_scope(
    _pdje_sqlite_scope
    ${targetName}
    PRIVATE
    "${_pdje_requested_scope}")

  target_link_libraries(${targetName} ${_pdje_sqlite_scope} PDJE_SQLITE3_AMALGAM)
  target_include_directories(${targetName} ${_pdje_sqlite_scope} ${sql_amalgam_SOURCE_DIR})
endfunction(setSqliteReqLib)

set(PDJE_OPENCL_CPP_INCLUDE_DIR
    "${opencl_clhpp_SOURCE_DIR}/include"
    CACHE INTERNAL "FetchContent-provided OpenCL C++ header include directory")

include(CheckCXXSourceCompiles)

set(_pdje_saved_required_includes "${CMAKE_REQUIRED_INCLUDES}")
set(CMAKE_REQUIRED_INCLUDES
    "${PDJE_OPENCL_CPP_INCLUDE_DIR};${OPENCL_INCLUDE_DIR}")

check_cxx_source_compiles(
  "
  #define CL_HPP_TARGET_OPENCL_VERSION 300
  #define CL_TARGET_OPENCL_VERSION 300
  #define CL_NO_PROTOTYPES
  #include <CL/opencl.hpp>
  int main() { return 0; }
  "
  PDJE_OPENCL_CLHPP_SUPPORTS_NO_PROTOTYPES)

check_cxx_source_compiles(
  "
  #define CL_HPP_TARGET_OPENCL_VERSION 300
  #define CL_TARGET_OPENCL_VERSION 300
  #define CL_NO_CORE_PROTOTYPES
  #include <CL/opencl.hpp>
  int main() { return 0; }
  "
  PDJE_OPENCL_CLHPP_SUPPORTS_NO_CORE_PROTOTYPES)

set(CMAKE_REQUIRED_INCLUDES "${_pdje_saved_required_includes}")

function(setOpenCLCppReqLib targetName)
  get_target_property(_pdje_target_type ${targetName} TYPE)
  if("${_pdje_target_type}" STREQUAL "INTERFACE_LIBRARY")
    set(_pdje_opencl_scope INTERFACE)
  else()
    set(_pdje_opencl_scope PRIVATE)
  endif()

  if(TARGET OpenCL::Headers)
    target_link_libraries(${targetName} ${_pdje_opencl_scope} OpenCL::Headers)
  endif()

  if(TARGET OpenCL::HeadersCpp)
    target_link_libraries(${targetName} ${_pdje_opencl_scope} OpenCL::HeadersCpp)
  endif()

  target_include_directories(${targetName}
                             ${_pdje_opencl_scope}
                             ${PDJE_OPENCL_CPP_INCLUDE_DIR})
endfunction(setOpenCLCppReqLib)

function(setOpenCLRuntimeShimReqLib targetName)
  get_target_property(_pdje_target_type ${targetName} TYPE)
  if("${_pdje_target_type}" STREQUAL "INTERFACE_LIBRARY")
    set(_pdje_opencl_runtime_scope INTERFACE)
  else()
    set(_pdje_opencl_runtime_scope PRIVATE)
  endif()

  if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_link_libraries(${targetName}
                          ${_pdje_opencl_runtime_scope}
                          ${CMAKE_DL_LIBS})
  endif()
endfunction(setOpenCLRuntimeShimReqLib)


# get_cmake_property(_vars VARIABLES)

# foreach(var ${_vars})
#     if(var MATCHES "^boost")
#         message(STATUS "환경변수: ${var} = [${${var}}]")
#     endif()
# endforeach()
include_directories(${nlohmann_json_SOURCE_DIR}/include)
include_directories(${sql_amalgam_SOURCE_DIR})
include_directories(${cppcodec_SOURCE_DIR})



if(PDJE_SWIG_BUILD)

find_package(SWIG REQUIRED)
include(UseSWIG)
endif()
