include(SETcmake.cmake)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_EXTENSIONS OFF) # optional: -std=c++20 instead of -std=gnu++20
cmake_minimum_required(VERSION 3.12)
project(
  PDJE
  VERSION 0.1
  LANGUAGES C CXX)

Include(FetchContent)
include(findPackages.cmake)

if(WIN32)
  find_package(hwy REQUIRED)
  add_compile_definitions(__WINDOWS__)
else()
endif()


link_libraries(${SQLite3_LIBRARIES})

if(WIN32)
  include_directories(
    ${CMAKE_SOURCE_DIR}/include/input/windows
    ${CAPNP_INCLUDE_DIRS}
  )
elseif(UNIX AND NOT APPLE)
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(LIBEVDEV REQUIRED libevdev)

  include_directories(
    ${CMAKE_SOURCE_DIR}/include/input/linux
    ${LIBEVDEV_INCLUDE_DIRS}
  )
  link_libraries(
    ${LIBEVDEV_LIBRARIES}
  )

  set(input_src
    ${CMAKE_SOURCE_DIR}/include/input/linux/LINUX_I.cpp
  )

elseif(APPLE)
  include_directories(
    ${CMAKE_SOURCE_DIR}/include/input/macos
  )

endif()

include(thirdPartySource.cmake)
include(includeDIRS.cmake)
include(DBsrc.cmake)
include(AUDIOsrc.cmake)
include(EDITORsrc.cmake)
link_libraries(
  CapnProto::kj 
  CapnProto::capnp 
  CapnProto::kj-gzip
)

add_executable(testdb ${CMAKE_SOURCE_DIR}/include/tests/dbTest.cpp ${dbSource})

add_executable(testaudio ${CMAKE_SOURCE_DIR}/include/tests/audioTest.cpp ${miniaudio_src} ${SoundTouch_src} ${dbSource} ${audioRenderSource})

add_executable(testCapnp ${CMAKE_SOURCE_DIR}/include/tests/CapnpTest.cpp ${miniaudio_src} ${SoundTouch_src} ${dbSource} ${audioRenderSource})


add_executable(testHighway include/tests/highway_Test.cpp)


add_executable(testManualAudio include/tests/manualAudioTest.cpp ${audioRenderSource} ${SoundTouch_src} ${miniaudio_src} ${dbSource})


add_executable(testInterface include/tests/interfaceTest.cpp ${audioRenderSource} ${SoundTouch_src} ${miniaudio_src} ${dbSource})


add_executable(testGIT include/tests/gittest.cpp ${editorSource})




if(WIN32)
  target_link_libraries(testHighway PRIVATE hwy::hwy ${CMAKE_BINARY_DIR}/libgitbin/lib/libgit2.lib)
  # link_libraries(hwy::hwy)
elseif(UNIX AND NOT APPLE)
  add_executable(EvdevAccModule include/input/linux/LINUX_SUDO_GETTER.cpp)
  add_dependencies(EvdevAccModule libgit2)

  add_executable(testLinuxInput_waiter include/tests/INPUT_TESTS/linux_I_WAITER.cpp)
  add_dependencies(testLinuxInput_waiter libgit2)
  add_executable(testLinuxInput_waker include/tests/INPUT_TESTS/linux_I_WAKER.cpp)
  add_dependencies(testLinuxInput_waker libgit2)
  add_executable(testLinuxInput include/tests/INPUT_TESTS/evdevtest.cpp ${input_src})
  add_dependencies(testLinuxInput libgit2)
  target_link_libraries(testHighway PRIVATE hwy )
  target_link_libraries(testdb PRIVATE hwy )
  target_link_libraries(testaudio PRIVATE hwy )
  target_link_libraries(testCapnp PRIVATE hwy )
  target_link_libraries(testManualAudio PRIVATE hwy )
  target_link_libraries(testInterface PRIVATE hwy)
  target_link_libraries(testGIT PRIVATE hwy)
  
  
elseif(APPLE)
  
endif()


target_link_libraries(testHighway PRIVATE nlohmann_json::nlohmann_json )
target_link_libraries(testdb PRIVATE nlohmann_json::nlohmann_json )
target_link_libraries(testaudio PRIVATE nlohmann_json::nlohmann_json )
target_link_libraries(testCapnp PRIVATE nlohmann_json::nlohmann_json )
target_link_libraries(testManualAudio PRIVATE nlohmann_json::nlohmann_json )
target_link_libraries(testInterface PRIVATE nlohmann_json::nlohmann_json)

# message(${NHJson_SOURCE_DIRS})
include(ADD_SWIGS.cmake)
enable_testing()

add_test(NAME DB_TEST COMMAND testdb)
add_test(NAME AUDIO_TEST COMMAND testaudio)