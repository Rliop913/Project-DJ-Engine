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

include(ExternalProject)
ExternalProject_Add(
  libgit2
  PREFIX ${CMAKE_BINARY_DIR}/_deps
  SOURCE_DIR ${CMAKE_SOURCE_DIR}/third_party/libgit2
  BUILD_IN_SOURCE 0
  BUILD_COMMAND cmake --build . --parallel 6
  INSTALL_COMMAND ""
)

ExternalProject_Get_Property(libgit2 source_dir binary_dir)

message("환경변수: ${source_dir} , ${binary_dir}")

link_libraries(git2)
# get_cmake_property(_vars VARIABLES)

# foreach(var ${_vars})
#     if(var MATCHES "^libgit")
#         message(STATUS "환경변수: ${var} = [${${var}}]")
#     endif()
# endforeach()

# message(${LIBGIT2_INCLUDE_DIR})
# link_libraries(libgit2)

find_package(SQLite3 REQUIRED)

FetchContent_MakeAvailable(CapnProto)
FetchContent_MakeAvailable(miniaudio)
FetchContent_MakeAvailable(highway)

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
