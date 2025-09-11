include(cmakes/SETcmake.cmake)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(NOT CMAKE_TOOLCHAIN_FILE)
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/conan_cmakes/conan_toolchain.cmake")
endif()
cmake_minimum_required(VERSION 3.12)
cmake_policy(SET CMP0091 NEW)
project(
  PDJE
  VERSION 0.1
  LANGUAGES C CXX)

Include(FetchContent)
include(cmakes/findPackages.cmake)

if(WIN32)
  add_compile_definitions(__WINDOWS__)
  add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
  
else()
endif()


option(PDJE_BUILD_TESTS "Enable build pdje test" OFF)
option(PDJE_SWIG_BUILD "Enable build with multiple languages" OFF)
option(PDJE_DYNAMIC "Make Shared Library" OFF)

# link_libraries(${SQLite3_LIBRARIES})

if(PDJE_DYNAMIC)
  if(WIN32)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
  else()
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
  endif()
endif()

# if(WIN32)
  
# elseif(UNIX AND NOT APPLE)
#   # find_package(PkgConfig REQUIRED)
#   # pkg_check_modules(LIBEVDEV REQUIRED libevdev)

# #   include_directories(
# #     ${CMAKE_CURRENT_SOURCE_DIR}/include/input/linux
# #     ${LIBEVDEV_INCLUDE_DIRS}
# #   )
# #   link_libraries(
# #     ${LIBEVDEV_LIBRARIES}
# #   )

# #   set(input_src
# #     ${CMAKE_CURRENT_SOURCE_DIR}/include/input/linux/LINUX_I.cpp
# #   )

# # elseif(APPLE)
# #   include_directories(
# #     ${CMAKE_CURRENT_SOURCE_DIR}/include/input/macos
# #   )

# endif()

include(cmakes/src/thirdPartySource.cmake)
include(cmakes/includeDIRS.cmake)
include(cmakes/src/DBsrc.cmake)
include(cmakes/src/AUDIOsrc.cmake)
include(cmakes/src/EDITORsrc.cmake)
include(cmakes/src/JUDGEsrc.cmake)
include(cmakes/src/GLOBALsrc.cmake)
include(cmakes/src/INPUTsrc.cmake)


if(PDJE_BUILD_TESTS)
add_executable(testEditor ${CMAKE_CURRENT_SOURCE_DIR}/include/tests/editorTest.cpp ${editorSource} ${sql_amalgam_src} ${dbSource} ${audioRenderSource} ${SoundTouch_src} ${miniaudio_src} ${globalSource})
add_executable(DBTester ${CMAKE_CURRENT_SOURCE_DIR}/include/tests/dbTest.cpp ${editorSource} ${sql_amalgam_src} ${dbSource} ${audioRenderSource} ${SoundTouch_src} ${miniaudio_src} ${globalSource})

target_include_directories(testEditor PUBLIC ${PDJE_INCLUDE_CORE})
target_include_directories(DBTester PUBLIC ${PDJE_INCLUDE_CORE})

endif(PDJE_BUILD_TESTS)

if(PDJE_DYNAMIC)
add_library(PDJE SHARED ${audioRenderSource} ${SoundTouch_src} ${miniaudio_src}  ${dbSource} ${sql_amalgam_src} ${editorSource} ${globalSource})
add_library(PDJE_MODULE_JUDGE SHARED ${judgeSource} ${globalSource})
add_library(PDJE_MODULE_INPUT SHARED ${inputSource} ${globalSource})

target_include_directories(PDJE PUBLIC ${PDJE_INCLUDE_CORE})
target_include_directories(PDJE_MODULE_JUDGE PUBLIC ${PDJE_INCLUDE_JUDGE})
target_include_directories(PDJE_MODULE_INPUT PUBLIC ${PDJE_INCLUDE_INPUT} ${PDJE_INCLUDE_INPUT_OS})

if(WIN32)
  target_compile_definitions(PDJE PUBLIC PDJE_WINDOWS_DLL)
  target_compile_definitions(PDJE PRIVATE PDJE_BUILDING)
  target_compile_definitions(PDJE_MODULE_JUDGE PUBLIC PDJE_WINDOWS_DLL)
  target_compile_definitions(PDJE_MODULE_JUDGE PRIVATE PDJE_BUILDING)
  target_compile_definitions(PDJE_MODULE_INPUT PUBLIC PDJE_WINDOWS_DLL)
  target_compile_definitions(PDJE_MODULE_INPUT PRIVATE PDJE_BUILDING)
  
endif()
else()
add_library(PDJE ${audioRenderSource} ${SoundTouch_src} ${miniaudio_src}  ${dbSource} ${sql_amalgam_src} ${editorSource})
add_library(PDJE_MODULE_JUDGE ${judgeSource} ${globalSource})
add_library(PDJE_MODULE_INPUT ${inputSource} ${globalSource})


target_include_directories(PDJE PUBLIC ${PDJE_INCLUDE_CORE})
target_include_directories(PDJE_MODULE_JUDGE PUBLIC ${PDJE_INCLUDE_JUDGE})
target_include_directories(PDJE_MODULE_INPUT PUBLIC ${PDJE_INCLUDE_INPUT} ${PDJE_INCLUDE_INPUT_OS})

endif()

function(linkPDJELibs targetName)
  target_link_libraries(${targetName} PUBLIC nlohmann_json::nlohmann_json rocksdb Annoy::Annoy)
  setSpdlogReqLib(${targetName})
  setCapnpReqLib(${targetName})
  setLibgit2ReqLib(${targetName})
  setHighwayReqLib(${targetName})
endfunction(linkPDJELibs)


if(PDJE_BUILD_TESTS)
#DEPRECATE TESTS
# linkPDJELibs(testHighway)
# linkPDJELibs(testdb)
# linkPDJELibs(testaudio)
# linkPDJELibs(testCapnp)
# linkPDJELibs(testManualAudio)
# linkPDJELibs(testInterface)
# linkPDJELibs(testGIT)
linkPDJELibs(testEditor)
linkPDJELibs(DBTester)

endif()


# linkPDJELibs(PDJE_MODULE_JUDGE)
linkPDJELibs(PDJE)

# target_link_libraries(PDJE PUBLIC nlohmann_json::nlohmann_json rocksdb Annoy::Annoy)
# setSpdlogReqLib(PDJE)
# setCapnpReqLib(PDJE)
# setLibgit2ReqLib(PDJE)
# setHighwayReqLib(PDJE)



if(PDJE_SWIG_BUILD)
include(cmakes/ADD_SWIGS.cmake)
endif()

# enable_testing()

# add_test(NAME DB_TEST COMMAND testdb)
# add_test(NAME AUDIO_TEST COMMAND testaudio)
