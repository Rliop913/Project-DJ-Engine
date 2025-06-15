
include(SETcmake.cmake)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
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
  # find_package(hwy REQUIRED)
  add_compile_definitions(__WINDOWS__)
else()
endif()


option(PDJE_BUILD_TESTS "Enable build pdje test" OFF)


# link_libraries(${SQLite3_LIBRARIES})

if(WIN32)
  include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include/input/windows
    ${CAPNP_INCLUDE_DIRS}
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

include(thirdPartySource.cmake)
include(includeDIRS.cmake)
include(DBsrc.cmake)
include(AUDIOsrc.cmake)
include(EDITORsrc.cmake)
if(WIN32)
link_libraries(
  CapnProto::kj 
  CapnProto::capnp 
)
else()
link_libraries(
  CapnProto::kj 
  CapnProto::capnp 
  CapnProto::kj-gzip
)

endif()


if(PDJE_BUILD_TESTS)
  

add_executable(testdb ${CMAKE_CURRENT_SOURCE_DIR}/include/tests/dbTest.cpp ${dbSource} ${sql_amalgam_src} ${editorSource} ${audioRenderSource} ${SoundTouch_src} ${miniaudio_src})

add_executable(testaudio ${CMAKE_CURRENT_SOURCE_DIR}/include/tests/audioTest.cpp ${miniaudio_src} ${SoundTouch_src} ${dbSource} ${audioRenderSource} ${sql_amalgam_src} ${editorSource})

add_executable(testCapnp ${CMAKE_CURRENT_SOURCE_DIR}/include/tests/CapnpTest.cpp ${miniaudio_src} ${SoundTouch_src} ${dbSource} ${audioRenderSource} ${sql_amalgam_src} ${editorSource})


add_executable(testHighway include/tests/highway_Test.cpp ${sql_amalgam_src} )


add_executable(testManualAudio include/tests/manualAudioTest.cpp ${audioRenderSource} ${SoundTouch_src} ${miniaudio_src} ${dbSource} ${sql_amalgam_src} ${editorSource})


add_executable(testInterface include/tests/interfaceTest.cpp ${audioRenderSource} ${SoundTouch_src} ${miniaudio_src} ${dbSource} ${sql_amalgam_src} ${editorSource})


add_executable(testGIT include/tests/gittest.cpp ${editorSource} ${sql_amalgam_src} ${dbSource} ${audioRenderSource} ${SoundTouch_src} ${miniaudio_src})
add_executable(testEditor include/tests/editorTest.cpp ${editorSource} ${sql_amalgam_src} ${dbSource} ${audioRenderSource} ${SoundTouch_src} ${miniaudio_src})
endif(PDJE_BUILD_TESTS)


add_library(PDJE ${audioRenderSource} ${SoundTouch_src} ${miniaudio_src} ${dbSource} ${sql_amalgam_src} ${editorSource})
add_library(PDJE_dynamic SHARED ${audioRenderSource} ${SoundTouch_src} ${miniaudio_src} ${dbSource} ${sql_amalgam_src} ${editorSource})
target_include_directories(PDJE PUBLIC ${INCLUDE_LIST})
target_include_directories(PDJE_dynamic PUBLIC ${INCLUDE_LIST})

if(WIN32)
target_compile_definitions(PDJE_dynamic PUBLIC PDJE_WINDOWS_DLL)
target_compile_definitions(PDJE_dynamic PRIVATE PDJE_BUILDING)
endif()

if(WIN32)

  link_libraries(
    debug "${zlib_BINARY_DIR}/Debug/zlibd.lib"
    optimized "${zlib_BINARY_DIR}/Release/zlib.lib"
  )
  if(PDJE_BUILD_TESTS)
  target_link_libraries(testHighway PRIVATE ${libgit2_static} winhttp crypt32 rpcrt4)
  target_link_libraries(testdb PRIVATE ${libgit2_static} winhttp crypt32 rpcrt4)
  target_link_libraries(testaudio PRIVATE ${libgit2_static} winhttp crypt32 rpcrt4)
  target_link_libraries(testCapnp PRIVATE ${libgit2_static} winhttp crypt32 rpcrt4)
  target_link_libraries(testManualAudio PRIVATE ${libgit2_static} winhttp crypt32 rpcrt4)
  target_link_libraries(testInterface PRIVATE ${libgit2_static} winhttp crypt32 rpcrt4)
  target_link_libraries(testGIT PRIVATE ${libgit2_static} winhttp crypt32 rpcrt4)
  target_link_libraries(testEditor PRIVATE ${libgit2_static} winhttp crypt32 rpcrt4)
  endif()
  target_link_libraries(PDJE PUBLIC ${libgit2_static} winhttp crypt32 rpcrt4)
  target_link_libraries(PDJE_dynamic PUBLIC ${libgit2_static} winhttp crypt32 rpcrt4)
  
elseif(UNIX AND NOT APPLE)
  # add_executable(EvdevAccModule include/input/linux/LINUX_SUDO_GETTER.cpp)
  # add_dependencies(EvdevAccModule libgit2)

  # add_executable(testLinuxInput_waiter include/tests/INPUT_TESTS/linux_I_WAITER.cpp)
  # add_dependencies(testLinuxInput_waiter libgit2)
  # add_executable(testLinuxInput_waker include/tests/INPUT_TESTS/linux_I_WAKER.cpp)
  # add_dependencies(testLinuxInput_waker libgit2)
  # add_executable(testLinuxInput include/tests/INPUT_TESTS/evdevtest.cpp ${input_src})
  # add_dependencies(testLinuxInput libgit2)
  # target_link_libraries(testHighway PRIVATE hwy )
  # target_link_libraries(testdb PRIVATE hwy )
  # target_link_libraries(testaudio PRIVATE hwy )
  # target_link_libraries(testCapnp PRIVATE hwy )
  # target_link_libraries(testManualAudio PRIVATE hwy )
  # target_link_libraries(testInterface PRIVATE hwy)
  # target_link_libraries(testGIT PRIVATE hwy)
  
  
elseif(APPLE)
  
endif()

if(PDJE_BUILD_TESTS)
target_link_libraries(testHighway PRIVATE nlohmann_json::nlohmann_json hwy)
target_link_libraries(testdb PRIVATE nlohmann_json::nlohmann_json hwy)
target_link_libraries(testaudio PRIVATE nlohmann_json::nlohmann_json hwy)
target_link_libraries(testCapnp PRIVATE nlohmann_json::nlohmann_json hwy)
target_link_libraries(testManualAudio PRIVATE nlohmann_json::nlohmann_json hwy)
target_link_libraries(testInterface PRIVATE nlohmann_json::nlohmann_json hwy)
target_link_libraries(testGIT PRIVATE nlohmann_json::nlohmann_json hwy)
target_link_libraries(testEditor PRIVATE nlohmann_json::nlohmann_json hwy)
endif()

target_link_libraries(PDJE PUBLIC nlohmann_json::nlohmann_json hwy)
target_link_libraries(PDJE_dynamic PUBLIC nlohmann_json::nlohmann_json hwy)




if(CMAKE_CFG_INTDIR STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "Release")
include(ADD_SWIGS.cmake)
endif()
# enable_testing()

# add_test(NAME DB_TEST COMMAND testdb)
# add_test(NAME AUDIO_TEST COMMAND testaudio)