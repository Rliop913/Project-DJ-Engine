FetchContent_Declare(
  miniaudio
  GIT_REPOSITORY https://github.com/mackron/miniaudio.git
  GIT_TAG 0.11.21
)

find_package(highway CONFIG REQUIRED)

function(setHighwayReqLib targetName)
  target_link_libraries(${targetName} PUBLIC
  highway::hwy
  )
  target_include_directories(${targetName} PUBLIC ${highway_INCLUDE_DIR})
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
  target_link_libraries(${targetName} PUBLIC 
  CapnProto::kj 
  CapnProto::capnp 
  CapnProto::capnpc 
  CapnProto::kj-gzip)
  target_include_directories(${targetName} PUBLIC ${CAPNPC_OUTPUT_DIR})
endfunction(setCapnpReqLib)


FetchContent_Declare(
  NHJson
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.12.0
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

if(PDJE_TEST)
FetchContent_Declare(
    doctest
    GIT_REPOSITORY https://github.com/doctest/doctest.git
    GIT_TAG        v2.4.11
)
endif()

function(setLibreMIDIReqLib targetName)
  target_link_libraries(${targetName} PUBLIC libremidi)
endfunction(setLibreMIDIReqLib)



find_package(botan CONFIG REQUIRED)

function(setBotanReqLib targetName)
  target_link_libraries(${targetName} PUBLIC botan::botan)
  if(DEFINED botan_INCLUDE_DIRS)
    target_include_directories(${targetName} PUBLIC ${botan_INCLUDE_DIRS})
  elseif(DEFINED botan_INCLUDE_DIR)
    target_include_directories(${targetName} PUBLIC ${botan_INCLUDE_DIR})
  endif()
endfunction()


# FetchContent_Declare(
#   cppHttp
#   GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
#   GIT_TAG v0.27.0
# ) #DEPRECATED

find_package(Annoy CONFIG REQUIRED)

function(setAnnoyReqLib targetName)
  get_target_property(_pdje_target_type ${targetName} TYPE)
  if("${_pdje_target_type}" STREQUAL "INTERFACE_LIBRARY")
    target_link_libraries(${targetName} INTERFACE Annoy::Annoy)
    if(DEFINED Annoy_INCLUDE_DIRS)
      target_include_directories(${targetName} INTERFACE ${Annoy_INCLUDE_DIRS})
    elseif(DEFINED Annoy_INCLUDE_DIR)
      target_include_directories(${targetName} INTERFACE ${Annoy_INCLUDE_DIR})
    endif()
  else()
    target_link_libraries(${targetName} PUBLIC Annoy::Annoy)
    if(DEFINED Annoy_INCLUDE_DIRS)
      target_include_directories(${targetName} PUBLIC ${Annoy_INCLUDE_DIRS})
    elseif(DEFINED Annoy_INCLUDE_DIR)
      target_include_directories(${targetName} PUBLIC ${Annoy_INCLUDE_DIR})
    endif()
  endif()
endfunction(setAnnoyReqLib)

find_package(spdlog CONFIG REQUIRED)

function(setSpdlogReqLib targetName)
  if(TARGET spdlog::spdlog)
    target_link_libraries(${targetName} PUBLIC spdlog::spdlog)
  else()
    target_link_libraries(${targetName} PUBLIC spdlog::spdlog_header_only)
  endif()
  target_include_directories(${targetName} PUBLIC ${spdlog_INCLUDE_DIR})
endfunction(setSpdlogReqLib)

function(setPdjeLogRuntimeReqLib targetName)
  if("${targetName}" STREQUAL "PDJE_LOG_RUNTIME")
    return()
  endif()
  if(NOT TARGET PDJE_LOG_RUNTIME)
    message(FATAL_ERROR "PDJE_LOG_RUNTIME target must be defined before calling setPdjeLogRuntimeReqLib(${targetName})")
  endif()
  target_link_libraries(${targetName} PUBLIC PDJE_LOG_RUNTIME)
endfunction(setPdjeLogRuntimeReqLib)


find_package(libgit2 CONFIG REQUIRED)

function(setLibgit2ReqLib targetName)
  target_include_directories(${targetName} PUBLIC ${libgit2_INCLUDE_DIR} )
  target_link_libraries(${targetName} PUBLIC libgit2::libgit2)
endfunction(setLibgit2ReqLib)

find_package(RocksDB REQUIRED)

function(setRocksDBReqLib targetName)
  get_target_property(_pdje_target_type ${targetName} TYPE)
  if("${_pdje_target_type}" STREQUAL "INTERFACE_LIBRARY")
    target_link_libraries(${targetName} INTERFACE RocksDB::rocksdb)
    if(DEFINED RocksDB_INCLUDE_DIRS)
      target_include_directories(${targetName} INTERFACE ${RocksDB_INCLUDE_DIRS})
    elseif(DEFINED RocksDB_INCLUDE_DIR)
      target_include_directories(${targetName} INTERFACE ${RocksDB_INCLUDE_DIR})
    endif()
  else()
    target_link_libraries(${targetName} PUBLIC RocksDB::rocksdb)
    if(DEFINED RocksDB_INCLUDE_DIRS)
      target_include_directories(${targetName} PUBLIC ${RocksDB_INCLUDE_DIRS})
    elseif(DEFINED RocksDB_INCLUDE_DIR)
      target_include_directories(${targetName} PUBLIC ${RocksDB_INCLUDE_DIR})
    endif()
  endif()
endfunction(setRocksDBReqLib)

find_package(OpenSSL REQUIRED)
link_libraries(${OPENSSL_LIBRARIES})


FetchContent_MakeAvailable(miniaudio)
FetchContent_MakeAvailable(NHJson)
FetchContent_MakeAvailable(sql_amalgam)
FetchContent_MakeAvailable(cppCodec)
FetchContent_MakeAvailable(libremidi)
if(PDJE_TEST)
FetchContent_MakeAvailable(doctest)
endif()
# FetchContent_MakeAvailable(cppHttp)

if(NOT TARGET PDJE_SQLITE3_AMALGAM)
  add_library(PDJE_SQLITE3_AMALGAM STATIC ${sql_amalgam_SOURCE_DIR}/sqlite3.c)
  set_target_properties(PDJE_SQLITE3_AMALGAM PROPERTIES POSITION_INDEPENDENT_CODE ON)
  target_include_directories(PDJE_SQLITE3_AMALGAM PUBLIC ${sql_amalgam_SOURCE_DIR})
endif()

function(setSqliteReqLib targetName)
  get_target_property(_pdje_target_type ${targetName} TYPE)
  if("${_pdje_target_type}" STREQUAL "INTERFACE_LIBRARY")
    target_link_libraries(${targetName} INTERFACE PDJE_SQLITE3_AMALGAM)
    target_include_directories(${targetName} INTERFACE ${sql_amalgam_SOURCE_DIR})
  else()
    target_link_libraries(${targetName} PUBLIC PDJE_SQLITE3_AMALGAM)
    target_include_directories(${targetName} PUBLIC ${sql_amalgam_SOURCE_DIR})
  endif()
endfunction(setSqliteReqLib)


# get_cmake_property(_vars VARIABLES)

# foreach(var ${_vars})
#     if(var MATCHES "^boost")
#         message(STATUS "환경변수: ${var} = [${${var}}]")
#     endif()
# endforeach()
include_directories(${nlohmann_json_SOURCE_DIR}/include)
include_directories(${sql_amalgam_SOURCE_DIR})
include_directories(${cppcodec_SOURCE_DIR})
# include_directories(${picosha_SOURCE_DIR})
include_directories(${httplib_SOURCE_DIR})





find_package(SWIG REQUIRED)
include(UseSWIG)
