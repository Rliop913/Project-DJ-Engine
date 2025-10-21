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
option(PDJE_DEVELOP_INPUT "Enable linux develop build" ON)
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

include(cmakes/src/thirdPartySource.cmake)
include(cmakes/includeDIRS.cmake)
include(cmakes/src/DBsrc.cmake)
include(cmakes/src/AUDIOsrc.cmake)
include(cmakes/src/EDITORsrc.cmake)
include(cmakes/src/JUDGEsrc.cmake)
include(cmakes/src/GLOBALsrc.cmake)
include(cmakes/src/INPUTsrc.cmake)

if(PDJE_DEVELOP_INPUT)
if(LINUX)

add_executable(PDJE_MODULE_INPUT_RTMAIN ${CMAKE_CURRENT_SOURCE_DIR}/include/input/linux/RT/RTMain.cpp ${inputSource} ${globalSource} ${PDJE_OS_INPUT_SRC})

target_include_directories(PDJE_MODULE_INPUT_RTMAIN PUBLIC ${PDJE_INCLUDE_INPUT})
PDJE_INPUT_LINK_LIB(PDJE_MODULE_INPUT_RTMAIN)
setSpdlogReqLib(PDJE_MODULE_INPUT_RTMAIN)

set(RT_HASH_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/include/input/linux/common/RT_EXEC_HASH.hpp)

add_custom_command(
  OUTPUT ${RT_HASH_HEADER}
  COMMAND ${CMAKE_COMMAND}
    -DRT_BIN=$<TARGET_FILE:PDJE_MODULE_INPUT_RTMAIN>
    -DOUT_HDR=${RT_HASH_HEADER}
    -P ${CMAKE_CURRENT_SOURCE_DIR}/cmakes/GenRTHASH.cmake
  DEPENDS PDJE_MODULE_INPUT_RTMAIN $<TARGET_FILE:PDJE_MODULE_INPUT_RTMAIN>
  BYPRODUCTS ${RT_HASH_HEADER}
  VERBATIM
  USES_TERMINAL
)

add_custom_target(gen_RT_HASH ALL DEPENDS ${RT_HASH_HEADER})



endif()

endif()


if(PDJE_BUILD_TESTS)
add_executable(testEditor ${CMAKE_CURRENT_SOURCE_DIR}/include/tests/editorTest.cpp ${editorSource} ${sql_amalgam_src} ${dbSource} ${audioRenderSource} ${SoundTouch_src} ${miniaudio_src} ${globalSource})
add_executable(DBTester ${CMAKE_CURRENT_SOURCE_DIR}/include/tests/dbTest.cpp ${editorSource} ${sql_amalgam_src} ${dbSource} ${audioRenderSource} ${SoundTouch_src} ${miniaudio_src} ${globalSource})

target_include_directories(testEditor PUBLIC ${PDJE_INCLUDE_CORE})
target_include_directories(DBTester PUBLIC ${PDJE_INCLUDE_CORE})

add_executable(testInput ${CMAKE_CURRENT_SOURCE_DIR}/include/tests/INPUT_TESTS/pdjeInputTest.cpp ${PDJE_OS_INPUT_SRC})
target_include_directories(testInput PRIVATE ${PDJE_INCLUDE_INPUT} )
PDJE_INPUT_LINK_LIB(testInput)


add_executable(testJudge ${CMAKE_CURRENT_SOURCE_DIR}/include/tests/JUDGE_TESTS/judgeTest.cpp ${editorSource} ${sql_amalgam_src} ${dbSource} ${audioRenderSource} ${SoundTouch_src} ${miniaudio_src} ${globalSource} ${judgeSource})
target_include_directories(testJudge PRIVATE ${PDJE_INCLUDE_CORE} ${PDJE_INCLUDE_INPUT} ${PDJE_INCLUDE_JUDGE} ${PDJE_INCLUDE_GLOBAL})
PDJE_INPUT_LINK_LIB(testJudge)

endif(PDJE_BUILD_TESTS)

if(PDJE_DYNAMIC)
add_library(PDJE SHARED ${audioRenderSource} ${SoundTouch_src} ${miniaudio_src}  ${dbSource} ${sql_amalgam_src} ${editorSource} ${globalSource})

add_library(PDJE_MODULE_JUDGE SHARED ${judgeSource} ${globalSource})
if(PDJE_DEVELOP_INPUT)
add_library(PDJE_MODULE_INPUT SHARED ${inputSource} ${globalSource})
endif()

target_include_directories(PDJE PUBLIC ${PDJE_INCLUDE_CORE})
target_include_directories(PDJE_MODULE_JUDGE PUBLIC ${PDJE_INCLUDE_JUDGE})
if(PDJE_DEVELOP_INPUT)
target_include_directories(PDJE_MODULE_INPUT PUBLIC ${PDJE_INCLUDE_INPUT} )
endif()
if(WIN32)
  target_compile_definitions(PDJE PUBLIC PDJE_WINDOWS_DLL)
  target_compile_definitions(PDJE PRIVATE PDJE_BUILDING)
  target_compile_definitions(PDJE_MODULE_JUDGE PUBLIC PDJE_WINDOWS_DLL)
  target_compile_definitions(PDJE_MODULE_JUDGE PRIVATE PDJE_BUILDING)
  target_compile_definitions(PDJE_MODULE_INPUT PUBLIC PDJE_WINDOWS_DLL)
  target_compile_definitions(PDJE_MODULE_INPUT PRIVATE PDJE_BUILDING)
  
endif()
else()
add_library(PDJE ${audioRenderSource} ${SoundTouch_src} ${miniaudio_src}  ${dbSource} ${sql_amalgam_src} ${editorSource} ${globalSource})


add_library(PDJE_MODULE_JUDGE ${judgeSource} ${globalSource})
if(PDJE_DEVELOP_INPUT)
add_library(PDJE_MODULE_INPUT ${inputSource} ${globalSource})
endif()


target_include_directories(PDJE PUBLIC ${PDJE_INCLUDE_CORE})
target_include_directories(PDJE_MODULE_JUDGE PUBLIC ${PDJE_INCLUDE_JUDGE})
if(PDJE_DEVELOP_INPUT)
target_include_directories(PDJE_MODULE_INPUT PUBLIC ${PDJE_INCLUDE_INPUT} )
endif()

endif()

if(PDJE_DEVELOP_INPUT)
if(LINUX)

target_link_libraries(PDJE_MODULE_INPUT PUBLIC OpenSSL::Crypto)

# add_dependencies(PDJE_MODULE_INPUT gen_RT_HASH)

endif()
endif()#skip linux

function(linkPDJELibs targetName)
  target_link_libraries(${targetName} PUBLIC nlohmann_json::nlohmann_json rocksdb Annoy::Annoy)
  setSpdlogReqLib(${targetName})
  setCapnpReqLib(${targetName})
  setLibgit2ReqLib(${targetName})
  setHighwayReqLib(${targetName})
endfunction(linkPDJELibs)


if(PDJE_BUILD_TESTS)
linkPDJELibs(testEditor)
linkPDJELibs(DBTester)
linkPDJELibs(testJudge)

endif()


# linkPDJELibs(PDJE_MODULE_JUDGE)
linkPDJELibs(PDJE)

setSpdlogReqLib(PDJE_MODULE_JUDGE)
setCapnpReqLib(PDJE_MODULE_JUDGE)
if(PDJE_DEVELOP_INPUT)
setSpdlogReqLib(PDJE_MODULE_INPUT)
PDJE_INPUT_LINK_LIB(PDJE_MODULE_INPUT)

endif()




if(PDJE_SWIG_BUILD)
include(cmakes/ADD_SWIGS.cmake)
endif()

# enable_testing()

# add_test(NAME DB_TEST COMMAND testdb)
# add_test(NAME AUDIO_TEST COMMAND testaudio)
