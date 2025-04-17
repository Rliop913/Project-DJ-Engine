# set(GIT_USE_STAT_MTIMESPEC ON CACHE BOOL "Don't use nanosecond timestamps" FORCE)

FetchContent_Declare(
  miniaudio
  GIT_REPOSITORY https://github.com/mackron/miniaudio.git
  GIT_TAG 0.11.21
)

FetchContent_Declare(
  highway
  GIT_REPOSITORY https://github.com/google/highway.git
  GIT_TAG 1.2.0
)

FetchContent_Declare(
  CapnProto
  GIT_REPOSITORY https://github.com/capnproto/capnproto.git
  GIT_TAG v1.1.0
)

FetchContent_Declare(
  NHJson
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.12.0
)

include(ExternalProject)
ExternalProject_Add(
  libgit2
  GIT_REPOSITORY https://github.com/libgit2/libgit2.git
  GIT_TAG v1.9.0

  PREFIX ${CMAKE_BINARY_DIR}/_deps
  BUILD_IN_SOURCE 0
  CMAKE_ARGS 
    -DBUILD_SHARED_LIBS=OFF

  BUILD_COMMAND cmake --build . --parallel 6
  INSTALL_DIR ${CMAKE_BINARY_DIR}/libgitbin
  INSTALL_COMMAND cmake --install . --prefix ${CMAKE_BINARY_DIR}/libgitbin
)

# ExternalProject_Get_Property(libgit2 source_dir binary_dir)

if(WIN32)
link_libraries(${CMAKE_BINARY_DIR}/libgitbin/lib/libgit2.lib)
else()
link_libraries(${CMAKE_BINARY_DIR}/libgitbin/lib/libgit2.a pcre)
find_package(OpenSSL REQUIRED)
# find_package(PCRE REQUIRED)
link_libraries(${OPENSSL_LIBRARIES})
# find_package(libcrypto REQUIRED)
# find_package(libpcre REQUIRED)
# find_package(libz REQUIRED)
endif()
get_cmake_property(_vars VARIABLES)

foreach(var ${_vars})
    if(var MATCHES "^nloh")
        message(STATUS "환경변수: ${var} = [${${var}}]")
    endif()
endforeach()

# message(${LIBGIT2_INCLUDE_DIR})
# link_libraries(libgit2)
link_directories(${CMAKE_BINARY_DIR}/libgitbin/include)
find_package(SQLite3 REQUIRED)

FetchContent_MakeAvailable(CapnProto)
FetchContent_MakeAvailable(miniaudio)
FetchContent_MakeAvailable(highway)
FetchContent_MakeAvailable(NHJson)

include_directories(${nlohmann_json_SOURCE_DIR}/include)
# include_directories(${libgit2_INCLUDE_DIRS})
set_target_properties(
  capnp
  capnpc
  kj
  capnp-json
  PROPERTIES POSITION_INDEPENDENT_CODE ON
)
find_package(SWIG REQUIRED)
include(UseSWIG)
