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
  # OUTPUT_DIR ${GEN_DIR}
)


function(setCapnpReqLib targetName)
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
  picoSHA
  GIT_REPOSITORY https://github.com/okdshin/PicoSHA2.git
  GIT_TAG v1.0.1
)

FetchContent_Declare(
  cppHttp
  GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
  GIT_TAG v0.27.0
)

find_package(Annoy CONFIG REQUIRED)

find_package(spdlog CONFIG REQUIRED)

function(setSpdlogReqLib targetName)
  target_link_libraries(${targetName} PUBLIC spdlog::spdlog_header_only)
  target_compile_definitions(${targetName} PUBLIC SPDLOG_USE_STD_FORMAT)
  target_include_directories(${targetName} PUBLIC ${spdlog_INCLUDE_DIR})
endfunction(setSpdlogReqLib)


find_package(libgit2 CONFIG REQUIRED)
function(setLibgit2ReqLib targetName)
  target_link_libraries(${targetName} PUBLIC libgit2::libgit2)
  target_include_directories(${targetName} PUBLIC ${libgit2_INCLUDE_DIR})
endfunction(setLibgit2ReqLib)


set(WITH_TESTS OFF CACHE BOOL "" FORCE)
set(WITH_TOOLS OFF CACHE BOOL "" FORCE)
set(WITH_BENCHMARK_TOOLS OFF CACHE BOOL "" FORCE)
set(WITH_ZLIB OFF CACHE BOOL "" FORCE)
set(WITH_GFLAGS OFF CACHE BOOL "" FORCE)
if(PDJE_DYNAMIC)
set(ROCKSDB_BUILD_SHARED ON CACHE BOOL "" FORCE)
else()
set(ROCKSDB_BUILD_SHARED OFF CACHE BOOL "" FORCE)
endif()
FetchContent_Declare(
  rocksDB
  GIT_REPOSITORY https://github.com/facebook/rocksdb.git
  GIT_TAG v10.5.1
)



find_package(OpenSSL REQUIRED)
link_libraries(${OPENSSL_LIBRARIES})






FetchContent_MakeAvailable(miniaudio)
FetchContent_MakeAvailable(NHJson)
FetchContent_MakeAvailable(sql_amalgam)
FetchContent_MakeAvailable(cppCodec)
FetchContent_MakeAvailable(rocksDB)
FetchContent_MakeAvailable(picoSHA)
FetchContent_MakeAvailable(cppHttp)


if(WIN32)
set_target_properties(rocksdb PROPERTIES
  COMPILE_FLAGS "/wd4702 /WX-"
)
endif()
# get_cmake_property(_vars VARIABLES)

# foreach(var ${_vars})
#     if(var MATCHES "^http")
#         message(STATUS "환경변수: ${var} = [${${var}}]")
#     endif()
# endforeach()
include_directories(${nlohmann_json_SOURCE_DIR}/include)
include_directories(${sql_amalgam_SOURCE_DIR})
include_directories(${cppcodec_SOURCE_DIR})
include_directories(${rocksdb_SOURCE_DIR}/include)
include_directories(${picosha_SOURCE_DIR})
include_directories(${httplib_SOURCE_DIR})





find_package(SWIG REQUIRED)
include(UseSWIG)
