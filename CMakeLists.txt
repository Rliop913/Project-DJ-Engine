include(cmakes/SETcmake.cmake)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
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

if(WIN32)
  include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include/input/windows
    # ${CAPNP_INCLUDE_DIRS}
  )
elseif(UNIX AND NOT APPLE)
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(LIBEVDEV REQUIRED libevdev)

  include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include/input/linux
    ${LIBEVDEV_INCLUDE_DIRS}
  )
  link_libraries(
    ${LIBEVDEV_LIBRARIES}
  )

  set(input_src
    ${CMAKE_CURRENT_SOURCE_DIR}/include/input/linux/LINUX_I.cpp
  )

elseif(APPLE)
  include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include/input/macos
  )

endif()

include(cmakes/src/thirdPartySource.cmake)
include(cmakes/includeDIRS.cmake)
include(cmakes/src/DBsrc.cmake)
include(cmakes/src/AUDIOsrc.cmake)
include(cmakes/src/EDITORsrc.cmake)



if(PDJE_BUILD_TESTS)
  

add_executable(testdb ${CMAKE_CURRENT_SOURCE_DIR}/include/tests/dbTest.cpp ${dbSource} ${sql_amalgam_src} ${editorSource} ${audioRenderSource} ${SoundTouch_src} ${miniaudio_src} )

add_executable(testaudio ${CMAKE_CURRENT_SOURCE_DIR}/include/tests/audioTest.cpp ${miniaudio_src}  ${SoundTouch_src} ${dbSource} ${audioRenderSource} ${sql_amalgam_src} ${editorSource})

add_executable(testCapnp ${CMAKE_CURRENT_SOURCE_DIR}/include/tests/CapnpTest.cpp ${miniaudio_src}  ${SoundTouch_src} ${dbSource} ${audioRenderSource} ${sql_amalgam_src} ${editorSource})


add_executable(testHighway include/tests/highway_Test.cpp ${sql_amalgam_src} )


add_executable(testManualAudio include/tests/manualAudioTest.cpp ${audioRenderSource} ${SoundTouch_src} ${miniaudio_src}  ${dbSource} ${sql_amalgam_src} ${editorSource})


add_executable(testInterface include/tests/interfaceTest.cpp ${audioRenderSource} ${SoundTouch_src} ${miniaudio_src}  ${dbSource} ${sql_amalgam_src} ${editorSource})


add_executable(testGIT include/tests/gittest.cpp ${editorSource} ${sql_amalgam_src} ${dbSource} ${audioRenderSource} ${SoundTouch_src} ${miniaudio_src} )
add_executable(testEditor include/tests/editorTest.cpp ${editorSource} ${sql_amalgam_src} ${dbSource} ${audioRenderSource} ${SoundTouch_src} ${miniaudio_src} )
endif(PDJE_BUILD_TESTS)

if(PDJE_DYNAMIC)
add_library(PDJE SHARED ${audioRenderSource} ${SoundTouch_src} ${miniaudio_src}  ${dbSource} ${sql_amalgam_src} ${editorSource})
target_include_directories(PDJE PUBLIC ${PDJE_INCLUDE_CORE})
if(WIN32)
  target_compile_definitions(PDJE PUBLIC PDJE_WINDOWS_DLL)
  target_compile_definitions(PDJE PRIVATE PDJE_BUILDING)
endif()
else()
add_library(PDJE ${audioRenderSource} ${SoundTouch_src} ${miniaudio_src}  ${dbSource} ${sql_amalgam_src} ${editorSource})
target_include_directories(PDJE PUBLIC ${PDJE_INCLUDE_CORE})
endif()


if(PDJE_BUILD_TESTS)
target_link_libraries(testHighway PRIVATE nlohmann_json::nlohmann_json hwy::hwy rocksdb Annoy::Annoy)
target_link_libraries(testdb PRIVATE nlohmann_json::nlohmann_json hwy::hwy rocksdb Annoy::Annoy)
target_link_libraries(testaudio PRIVATE nlohmann_json::nlohmann_json hwy::hwy rocksdb Annoy::Annoy)
target_link_libraries(testCapnp PRIVATE nlohmann_json::nlohmann_json hwy::hwy rocksdb Annoy::Annoy)
target_link_libraries(testManualAudio PRIVATE nlohmann_json::nlohmann_json hwy::hwy rocksdb Annoy::Annoy)
target_link_libraries(testInterface PRIVATE nlohmann_json::nlohmann_json hwy::hwy rocksdb Annoy::Annoy)
target_link_libraries(testGIT PRIVATE nlohmann_json::nlohmann_json hwy::hwy rocksdb Annoy::Annoy)
target_link_libraries(testEditor PRIVATE nlohmann_json::nlohmann_json hwy::hwy rocksdb Annoy::Annoy)
endif()

target_link_libraries(PDJE PUBLIC nlohmann_json::nlohmann_json rocksdb Annoy::Annoy)
setSpdlogReqLib(PDJE)
setCapnpReqLib(PDJE)
setLibgit2ReqLib(PDJE)
setHighwayReqLib(PDJE)



if(PDJE_SWIG_BUILD)
include(cmakes/ADD_SWIGS.cmake)
endif()

# enable_testing()

# add_test(NAME DB_TEST COMMAND testdb)
# add_test(NAME AUDIO_TEST COMMAND testaudio)
