
add_executable(testEditor ${CMAKE_CURRENT_SOURCE_DIR}/include/tests/editorTest.cpp ${CORE_SRC_EXPORT})
add_executable(DBTester ${CMAKE_CURRENT_SOURCE_DIR}/include/tests/dbTest.cpp ${CORE_SRC_EXPORT})
add_executable(gitTester ${CMAKE_CURRENT_SOURCE_DIR}/include/tests/gittest.cpp ${CORE_SRC_EXPORT})
add_executable(
    testOnnxLoad
    ${CMAKE_CURRENT_SOURCE_DIR}/include/tests/testOnnxLoad.cpp)

target_link_libraries(testEditor PRIVATE CORE_OBJ)
target_link_libraries(DBTester PRIVATE CORE_OBJ)
target_link_libraries(gitTester PRIVATE CORE_OBJ)
setCoreReqs(testEditor)
setCoreReqs(DBTester)
setCoreReqs(gitTester)
setOnnxRuntimeReqLib(testOnnxLoad)
pdje_copy_onnxruntime_runtime(testOnnxLoad)
target_compile_definitions(
    testOnnxLoad
    PRIVATE PDJE_EXPECTED_ONNXRUNTIME_VERSION="${PDJE_ONNXRUNTIME_VERSION}")
PDJE_COMPILE_OPTION(testOnnxLoad)
SET_PROPERTIES(testOnnxLoad)

if(PDJE_DEVELOP_INPUT)
add_executable(testInput ${CMAKE_CURRENT_SOURCE_DIR}/include/tests/INPUT_TESTS/pdjeInputTest.cpp ${INPUT_SRC_EXPORT})


add_executable(testMIDI ${CMAKE_CURRENT_SOURCE_DIR}/include/tests/INPUT_TESTS/miditest.cpp ${INPUT_SRC_EXPORT})


add_executable(testJudge ${CMAKE_CURRENT_SOURCE_DIR}/include/tests/JUDGE_TESTS/judgeTest.cpp ${INPUT_SRC_EXPORT} ${CORE_SRC_ONLY_EXPORT} ${JUDGE_SRC_ONLY_EXPORT})



target_link_libraries(testInput PRIVATE 
INPUT_OBJ
INPUT_MAIN_INCLUDE

)
target_link_libraries(testMIDI PRIVATE 
INPUT_OBJ
INPUT_MAIN_INCLUDE

)
target_link_libraries(testJudge PRIVATE 
INPUT_OBJ 
INPUT_MAIN_INCLUDE
CORE_OBJ
JUDGE_OBJ
JUDGE_INCLUDE)


setInputReqs(testInput)
setInputReqs(testMIDI)
setInputReqs(testJudge)
setJudgeReqs(testJudge)
setCoreReqs(testJudge)
endif()


add_executable(
    music_to_waveform_webp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/tests/music_to_waveform_webp.cpp ${CORE_SRC_EXPORT})

target_link_libraries(music_to_waveform_webp PRIVATE 
CORE_OBJ 
CORE_INCLUDE 
GLOBAL_INCLUDE 
GLOBAL_OBJ)
setCoreReqs(music_to_waveform_webp)
target_include_directories(
    music_to_waveform_webp 
    PRIVATE
    ${PDJE_INCLUDE_GLOBAL} 
    ${PDJE_INCLUDE_CORE}
    )

setCapnpReqLib(music_to_waveform_webp)

target_link_libraries(music_to_waveform_webp PRIVATE PDJE_UTIL)



if(WIN32)
set(_music_to_waveform_webp_zlib_runtime
    "$<$<CONFIG:Debug>:${zlib_BIN_DIRS_DEBUG}/zlib1.dll>$<$<CONFIG:Release>:${zlib_BIN_DIRS_RELEASE}/zlib1.dll>$<$<CONFIG:RelWithDebInfo>:${zlib_BIN_DIRS_RELEASE}/zlib1.dll>$<$<CONFIG:MinSizeRel>:${zlib_BIN_DIRS_RELEASE}/zlib1.dll>")
add_custom_command(
    TARGET music_to_waveform_webp
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${_music_to_waveform_webp_zlib_runtime}"
    $<TARGET_FILE_DIR:music_to_waveform_webp>
    VERBATIM)
endif()
PDJE_COMPILE_OPTION(music_to_waveform_webp)
SET_PROPERTIES(music_to_waveform_webp)
