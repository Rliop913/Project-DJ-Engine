
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

find_package(SQLite3 REQUIRED)

FetchContent_MakeAvailable(CapnProto)
FetchContent_MakeAvailable(miniaudio)
FetchContent_MakeAvailable(highway)

find_package(SWIG REQUIRED)
include(UseSWIG)
