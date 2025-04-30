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

FetchContent_Declare(
  sql_amalgam
  URL https://www.sqlite.org/2024/sqlite-amalgamation-3450000.zip
)


include(ExternalProject)

if(WIN32)
ExternalProject_Add(
  libgit2
  GIT_REPOSITORY https://github.com/libgit2/libgit2.git
  GIT_TAG v1.9.0

  PREFIX "${CMAKE_BINARY_DIR}/_deps"
  BUILD_IN_SOURCE 0
  CMAKE_ARGS     -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR> -DBUILD_SHARED_LIBS=OFF -DREGEX_BACKEND=builtin
  BUILD_COMMAND  ${CMAKE_COMMAND} --build . --config $<CONFIG>
  INSTALL_COMMAND ${CMAKE_COMMAND} --install . --config $<CONFIG>
)

else()
ExternalProject_Add(
  libgit2
  GIT_REPOSITORY https://github.com/libgit2/libgit2.git
  GIT_TAG v1.9.0

  PREFIX "${CMAKE_BINARY_DIR}/_deps"
  BUILD_IN_SOURCE 0
  CMAKE_ARGS 
    -DBUILD_SHARED_LIBS=OFF 
    -DREGEX_BACKEND=builtin
    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}

  BUILD_COMMAND cmake --build . --parallel 6
  INSTALL_DIR "${CMAKE_BINARY_DIR}/libgitbin"
  INSTALL_COMMAND cmake --install . --prefix "${CMAKE_BINARY_DIR}/libgitbin"
)
endif()

ExternalProject_Get_Property(libgit2 source_dir binary_dir install_dir)
# message("환경변수-install: ${install_dir} $<CONFIG>")




find_package(OpenSSL REQUIRED)
link_libraries(${OPENSSL_LIBRARIES})
# link_directories(${install_dir}/lib)
if(UNIX)
link_libraries(${install_dir}/lib/libgit2.a)
else()
add_library(libgit2_static STATIC IMPORTED GLOBAL)

add_dependencies(libgit2_static libgit2)

# set_target_properties(libgit2_static PROPERTIES
#     IMPORTED_LOCATION  "$<$<CONFIG:Debug>:${install_dir}/src/libgit2-build/Debug/git2.lib>$<$<CONFIG:Release>:${install_dir}/libgit2-build/Release/git2.lib>"
    
#     INTERFACE_INCLUDE_DIRECTORIES "${install_dir}/include"
# )
# file(GLOB LIBGIT2_LIBRARIES "${CMAKE_BINARY_DIR}/src/libgit2-build/${CMAKE_BUILD_TYPE}/git2.lib")
# link_libraries(${CMAKE_BINARY_DIR}/src/libgit2-build/${CMAKE_BUILD_TYPE}/git2.lib)
# link_libraries(
#   $<$<CONFIG:Debug>:${install_dir}/src/libgit2-build/Debug/libgit2_debug.lib>
#   $<$<CONFIG:Release>:${install_dir}/src/libgit2-build/Release/libgit2.lib>
# )
link_libraries("$<$<CONFIG:Debug>:${install_dir}/src/libgit2-build/Debug/git2.lib>$<$<CONFIG:Release>:${install_dir}/src/libgit2-build/Release/git2.lib>")
endif()
# add_library(libgit2_static INTERFACE IMPORTED GLOBAL)

# add_dependencies(libgit2_static libgit2)

# set_target_properties(libgit2_static PROPERTIES
#   INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_BINARY_DIR}/libgitbin/include"
#   INTERFACE_LINK_LIBRARIES "${CMAKE_BINARY_DIR}/libgitbin/lib/libgit2.a"
# )


# message(${LIBGIT2_INCLUDE_DIR})
# link_libraries(libgit2)
# message("LOCATION위치: ${install_dir}/include")
include_directories(${install_dir}/include)
# find_package(SQLite3 REQUIRED)

FetchContent_MakeAvailable(CapnProto)
FetchContent_MakeAvailable(miniaudio)
FetchContent_MakeAvailable(highway)
FetchContent_MakeAvailable(NHJson)
FetchContent_MakeAvailable(sql_amalgam)

# get_cmake_property(_vars VARIABLES)

# foreach(var ${_vars})
#     if(var MATCHES "^cppgit")
#         message(STATUS "환경변수: ${var} = [${${var}}]")
#     endif()
# endforeach()
include_directories(${nlohmann_json_SOURCE_DIR}/include)
include_directories(${hwy_SOURCE_DIR})
include_directories(${sql_amalgam_SOURCE_DIR})
# link_libraries(${hwy_BINARY_DIR}/libhwy.a)
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
