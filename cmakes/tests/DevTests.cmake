
add_executable(testEditor ${CMAKE_CURRENT_SOURCE_DIR}/include/tests/editorTest.cpp ${CORE_SRCS})
add_executable(DBTester ${CMAKE_CURRENT_SOURCE_DIR}/include/tests/dbTest.cpp ${CORE_SRCS})
add_executable(gitTester ${CMAKE_CURRENT_SOURCE_DIR}/include/tests/gittest.cpp ${CORE_SRCS})

setCoreReqs(testEditor)
setCoreReqs(DBTester)
setCoreReqs(gitTester)
if(PDJE_DEVELOP_INPUT)
add_executable(testInput ${CMAKE_CURRENT_SOURCE_DIR}/include/tests/INPUT_TESTS/pdjeInputTest.cpp ${INPUT_SRCS})
setInputReqs(testInput)

add_executable(testMIDI ${CMAKE_CURRENT_SOURCE_DIR}/include/tests/INPUT_TESTS/miditest.cpp ${INPUT_SRCS})
setInputReqs(testMIDI)

add_executable(testJudge ${CMAKE_CURRENT_SOURCE_DIR}/include/tests/JUDGE_TESTS/judgeTest.cpp ${INPUT_SRCS} ${CORE_SRCS} ${JUDGE_SRCS})
setJudgeReqs(testJudge)
setCoreReqs(testJudge)
setInputReqs(testJudge)
endif()


add_executable(
    music_to_waveform_png
    ${CMAKE_CURRENT_SOURCE_DIR}/include/tests/music_to_waveform_png.cpp)


target_include_directories(
    music_to_waveform_png 
    PRIVATE
    ${PDJE_INCLUDE_GLOBAL} 
    ${PDJE_INCLUDE_CORE}
    )

target_link_libraries(music_to_waveform_png PRIVATE PDJE)
target_link_libraries(music_to_waveform_png PRIVATE PDJE_UTIL_IMAGE_PNG)
target_link_libraries(music_to_waveform_png PRIVATE PDJE_UTIL_IMAGE_WAVEFORM)

AddDynamicDef(music_to_waveform_png)

if(WIN32)
set(_music_to_waveform_png_zlib_runtime
    "$<$<CONFIG:Debug>:${zlib_BIN_DIRS_DEBUG}/zlib1.dll>$<$<CONFIG:Release>:${zlib_BIN_DIRS_RELEASE}/zlib1.dll>$<$<CONFIG:RelWithDebInfo>:${zlib_BIN_DIRS_RELEASE}/zlib1.dll>$<$<CONFIG:MinSizeRel>:${zlib_BIN_DIRS_RELEASE}/zlib1.dll>")
add_custom_command(
    TARGET music_to_waveform_png
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${_music_to_waveform_png_zlib_runtime}"
    $<TARGET_FILE_DIR:music_to_waveform_png>
    VERBATIM)
endif()
PDJE_COMPILE_OPTION(music_to_waveform_png)
SET_PROPERTIES(music_to_waveform_png)