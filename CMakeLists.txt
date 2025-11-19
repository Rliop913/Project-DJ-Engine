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

set(PDJE_INCLUDE_ROOT  ${CMAKE_SOURCE_DIR} CACHE STRING "Set pdje root")

option(PDJE_BUILD_TESTS "Enable build pdje test" OFF)
if(WIN32)
option(PDJE_DEVELOP_INPUT "Enable linux develop build" ON)
else()
option(PDJE_DEVELOP_INPUT "Enable linux develop build" OFF)
endif()

option(PDJE_SWIG_BUILD "Enable build with multiple languages" OFF)
option(PDJE_DYNAMIC "Make Shared Library" OFF)

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
include(cmakes/src/IPCsrc.cmake)

if(PDJE_DEVELOP_INPUT)


add_executable(PDJE_MODULE_INPUT_PROCESS ${inputSource} ${globalSource}  ${PDJE_INPUT_PROCESS_SRC} ${PDJE_IPC_SRC})

target_include_directories(PDJE_MODULE_INPUT_PROCESS PUBLIC ${PDJE_INCLUDE_INPUT})

PDJE_COMPILE_OPTION(PDJE_MODULE_INPUT_PROCESS)
PDJE_INPUT_LINK_LIB(PDJE_MODULE_INPUT_PROCESS)
setSpdlogReqLib(PDJE_MODULE_INPUT_PROCESS)
SET_PROPERTIES(PDJE_MODULE_INPUT_PROCESS)
setBotanReqLib(PDJE_MODULE_INPUT_PROCESS)
set(RT_HASH_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/include/input/IPC/common/PDJE_INPUT_PROCESS_HASH.hpp)

add_custom_command(
  OUTPUT ${RT_HASH_HEADER}
  COMMAND ${CMAKE_COMMAND}
    -DRT_BIN=$<TARGET_FILE:PDJE_MODULE_INPUT_PROCESS>
    -DOUT_HDR=${RT_HASH_HEADER}
    -P ${CMAKE_CURRENT_SOURCE_DIR}/cmakes/GenRTHASH.cmake
  DEPENDS PDJE_MODULE_INPUT_PROCESS $<TARGET_FILE:PDJE_MODULE_INPUT_PROCESS>
  BYPRODUCTS ${RT_HASH_HEADER}
  VERBATIM
  USES_TERMINAL
)

add_custom_target(gen_RT_HASH ALL DEPENDS ${RT_HASH_HEADER})

endif()

set(CORE_SRCS
  ${editorSource} 
  ${sql_amalgam_src} 
  ${dbSource} 
  ${audioRenderSource} 
  ${SoundTouch_src} 
  ${miniaudio_src} 
  ${globalSource}
)  

set(JUDGE_SRCS
${judgeSource} 
${globalSource}
)

set(INPUT_SRCS
${inputSource} 
${globalSource} 
${PDJE_OS_INPUT_SRC} 
${PDJE_IPC_SRC}
)

function(LinuxSetAtomic targetName)
  if(LINUX)
    target_link_libraries(${targetName} PUBLIC atomic)
    target_link_options(${targetName} PUBLIC -Wl,-z,defs)
  endif()
endfunction()

function(setCoreReqs targetName)
  target_include_directories(${targetName} PUBLIC ${PDJE_INCLUDE_CORE})
  if(PDJE_DYNAMIC)
    if(WIN32)
      target_compile_definitions(${targetName} PUBLIC PDJE_WINDOWS_DLL)
      target_compile_definitions(${targetName} PRIVATE PDJE_BUILDING)
    endif()
  endif()
  target_link_libraries(${targetName} PUBLIC nlohmann_json::nlohmann_json)
  setRocksDBReqLib(${targetName})
  setSpdlogReqLib(${targetName})
  setCapnpReqLib(${targetName})
  setLibgit2ReqLib(${targetName})
  setHighwayReqLib(${targetName})
  LinuxSetAtomic(${targetName})
  PDJE_COMPILE_OPTION(${targetName})
  setBotanReqLib(${targetName})
  SET_PROPERTIES(${targetName})
endfunction(setCoreReqs)


function(setInputReqs targetName)
  if(PDJE_DYNAMIC AND WIN32)
    target_compile_definitions(${targetName} PUBLIC PDJE_WINDOWS_DLL)
    target_compile_definitions(${targetName} PRIVATE PDJE_BUILDING)
  endif()
  add_dependencies(${targetName} gen_RT_HASH)
  target_include_directories(${targetName} PUBLIC ${PDJE_INCLUDE_INPUT} ${PDJE_INCLUDE_GLOBAL})
  setSpdlogReqLib(${targetName})
  setBotanReqLib(${targetName})
  PDJE_INPUT_LINK_LIB(${targetName})
  LinuxSetAtomic(${targetName})
  SET_PROPERTIES(${targetName})
  PDJE_COMPILE_OPTION(${targetName})
endfunction(setInputReqs)

function(setJudgeReqs targetName)
  if(PDJE_DYNAMIC AND WIN32)
    target_compile_definitions(${targetName} PUBLIC PDJE_WINDOWS_DLL)
    target_compile_definitions(${targetName} PRIVATE PDJE_BUILDING)
  endif()
  add_dependencies(${targetName} gen_RT_HASH)
  target_include_directories(${targetName} PUBLIC ${PDJE_INCLUDE_INPUT} ${PDJE_INCLUDE_JUDGE} ${PDJE_INCLUDE_GLOBAL})
  setSpdlogReqLib(${targetName})
  setCapnpReqLib(${targetName})
  setBotanReqLib(${targetName})
  LinuxSetAtomic(${targetName})
  SET_PROPERTIES(${targetName})
  PDJE_COMPILE_OPTION(${targetName})
endfunction(setJudgeReqs)

if(PDJE_BUILD_TESTS)

add_executable(testEditor ${CMAKE_CURRENT_SOURCE_DIR}/include/tests/editorTest.cpp ${CORE_SRCS})
add_executable(DBTester ${CMAKE_CURRENT_SOURCE_DIR}/include/tests/dbTest.cpp ${CORE_SRCS})
add_executable(gitTester ${CMAKE_CURRENT_SOURCE_DIR}/include/tests/gittest.cpp ${CORE_SRCS})

setCoreReqs(testEditor)
setCoreReqs(DBTester)
setCoreReqs(gitTester)
if(PDJE_DEVELOP_INPUT)
  add_executable(testInput ${CMAKE_CURRENT_SOURCE_DIR}/include/tests/INPUT_TESTS/pdjeInputTest.cpp ${INPUT_SRCS})
  setInputReqs(testInput)

  add_executable(testJudge ${CMAKE_CURRENT_SOURCE_DIR}/include/tests/JUDGE_TESTS/judgeTest.cpp ${INPUT_SRCS} ${CORE_SRCS} ${JUDGE_SRCS})
  setJudgeReqs(testJudge)
  setCoreReqs(testJudge)
  setInputReqs(testJudge)
endif()
endif(PDJE_BUILD_TESTS)



if(PDJE_DYNAMIC)
  add_library(PDJE SHARED ${CORE_SRCS})
else()
  add_library(PDJE ${CORE_SRCS})
endif()

setCoreReqs(PDJE)

if(PDJE_DEVELOP_INPUT)
if(PDJE_DYNAMIC)

  add_library(PDJE_MODULE_JUDGE SHARED ${JUDGE_SRCS})
  add_library(PDJE_MODULE_INPUT SHARED ${INPUT_SRCS})

else()
  add_library(PDJE_MODULE_JUDGE ${JUDGE_SRCS})
  add_library(PDJE_MODULE_INPUT ${INPUT_SRCS})

endif()# end dynamic
  setInputReqs(PDJE_MODULE_INPUT)
  setJudgeReqs(PDJE_MODULE_JUDGE)
endif()


if(PDJE_SWIG_BUILD)
  include(cmakes/ADD_SWIGS.cmake)
endif()

# enable_testing()

# add_test(NAME DB_TEST COMMAND testdb)
# add_test(NAME AUDIO_TEST COMMAND testaudio)


include(GNUInstallDirs)


install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  FILES_MATCHING
    PATTERN "*.h" PATTERN "*.hpp" PATTERN "*.hxx" PATTERN "*.inl" PATTERN "*.ipp"
  PATTERN ".git*" EXCLUDE
  PATTERN "CMakeLists.txt" EXCLUDE)
