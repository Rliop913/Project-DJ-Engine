include(SETcmake.cmake)
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
include(findPackages.cmake)

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

include(thirdPartySource.cmake)
include(includeDIRS.cmake)
include(DBsrc.cmake)
include(AUDIOsrc.cmake)
include(EDITORsrc.cmake)



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
# add_executable(testLogger include/tests/logTest.cpp )

foreach(dir IN LISTS PDJE_INCLUDE_LIST)
  message(STATUS "PDJE include dir -> ${dir}")
endforeach()
if(PDJE_DYNAMIC)
add_library(PDJE SHARED ${audioRenderSource} ${SoundTouch_src} ${miniaudio_src}  ${dbSource} ${sql_amalgam_src} ${editorSource})
target_include_directories(PDJE PUBLIC ${PDJE_INCLUDE_LIST})
else()
add_library(PDJE ${audioRenderSource} ${SoundTouch_src} ${miniaudio_src}  ${dbSource} ${sql_amalgam_src} ${editorSource})
target_include_directories(PDJE PUBLIC ${PDJE_INCLUDE_LIST})
endif()



# if(WIN32)

#   # link_libraries(
#   #   debug "${zlib_BINARY_DIR}/Debug/zlibd.lib"
#   #   optimized "${zlib_BINARY_DIR}/Release/zlib.lib"
#   # )
#   if(PDJE_BUILD_TESTS)
#   target_link_libraries(testHighway PRIVATE libgit2_static winhttp crypt32 rpcrt4 secur32  )
#   add_dependencies(testHighway  libgit2_static)
#   target_link_libraries(testdb PRIVATE libgit2_static winhttp crypt32 rpcrt4 secur32  )
#   add_dependencies(testdb  libgit2_static)
#   target_link_libraries(testaudio PRIVATE libgit2_static winhttp crypt32 rpcrt4 secur32  )
#   add_dependencies(testaudio  libgit2_static)
#   target_link_libraries(testCapnp PRIVATE libgit2_static winhttp crypt32 rpcrt4 secur32  )
#   add_dependencies(testCapnp  libgit2_static)
#   target_link_libraries(testManualAudio PRIVATE libgit2_static winhttp crypt32 rpcrt4 secur32  )
#   add_dependencies(testManualAudio  libgit2_static)
#   target_link_libraries(testInterface PRIVATE libgit2_static winhttp crypt32 rpcrt4 secur32  )
#   add_dependencies(testInterface  libgit2_static)
#   target_link_libraries(testGIT PRIVATE libgit2_static winhttp crypt32 rpcrt4 secur32  )
#   add_dependencies(testGIT  libgit2_static)
#   endif()
#   # target_link_libraries(testEditor PRIVATE libgit2_static winhttp crypt32 rpcrt4 secur32  )
#   # add_dependencies(testEditor libgit2_static)
#   # target_link_libraries(PDJE PUBLIC libgit2_static winhttp crypt32 rpcrt4 secur32 )
#   # add_dependencies(PDJE libgit2_static)
#   # target_link_libraries(PDJE_dynamic PUBLIC libgit2_static winhttp crypt32 rpcrt4 secur32 )
#   # add_dependencies(PDJE_dynamic libgit2_static)
# elseif(UNIX AND NOT APPLE)
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
  
  
# elseif(APPLE)
# if(PDJE_BUILD_TESTS)
# target_link_libraries(testHighway PRIVATE iconv "-framework CoreFoundation" "-framework Security" )
# target_link_libraries(testdb PRIVATE iconv "-framework CoreFoundation" "-framework Security" )
# target_link_libraries(testaudio PRIVATE iconv "-framework CoreFoundation" "-framework Security" )
# target_link_libraries(testCapnp PRIVATE iconv "-framework CoreFoundation" "-framework Security" )
# target_link_libraries(testManualAudio PRIVATE iconv "-framework CoreFoundation" "-framework Security" )
# target_link_libraries(testInterface PRIVATE iconv "-framework CoreFoundation" "-framework Security" )
# target_link_libraries(testGIT PRIVATE iconv "-framework CoreFoundation" "-framework Security" )
# endif()
# target_link_libraries(testEditor PRIVATE iconv "-framework CoreFoundation" "-framework Security" )

# target_link_libraries(PDJE PUBLIC iconv "-framework CoreFoundation" "-framework Security" )
# target_link_libraries(PDJE_dynamic PUBLIC iconv "-framework CoreFoundation" "-framework Security" )
  
# endif()

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
include(ADD_SWIGS.cmake)
endif()

# enable_testing()

# add_test(NAME DB_TEST COMMAND testdb)
# add_test(NAME AUDIO_TEST COMMAND testaudio)