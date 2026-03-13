
add_executable(testEditor ${CMAKE_CURRENT_SOURCE_DIR}/include/tests/editorTest.cpp ${CORE_SRC_EXPORT})
add_executable(DBTester ${CMAKE_CURRENT_SOURCE_DIR}/include/tests/dbTest.cpp ${CORE_SRC_EXPORT})
add_executable(gitTester ${CMAKE_CURRENT_SOURCE_DIR}/include/tests/gittest.cpp ${CORE_SRC_EXPORT})

target_link_libraries(testEditor PRIVATE CORE_OBJ)
target_link_libraries(DBTester PRIVATE CORE_OBJ)
target_link_libraries(gitTester PRIVATE CORE_OBJ)


if(PDJE_DEVELOP_INPUT)
add_executable(testInput ${CMAKE_CURRENT_SOURCE_DIR}/include/tests/INPUT_TESTS/pdjeInputTest.cpp ${INPUT_SRC_EXPORT})


add_executable(testMIDI ${CMAKE_CURRENT_SOURCE_DIR}/include/tests/INPUT_TESTS/miditest.cpp ${INPUT_SRC_EXPORT})


add_executable(testJudge ${CMAKE_CURRENT_SOURCE_DIR}/include/tests/JUDGE_TESTS/judgeTest.cpp ${INPUT_SRC_EXPORT} ${CORE_SRC_ONLY_EXPORT} ${JUDGE_SRC_ONLY_EXPORT})



target_link_libraries(testInput PRIVATE INPUT_OBJ)
target_link_libraries(testMIDI PRIVATE INPUT_OBJ)
target_link_libraries(testJudge PRIVATE INPUT_OBJ CORE_OBJ JUDGE_OBJ)

endif()


add_executable(
    music_to_waveform_png
    ${CMAKE_CURRENT_SOURCE_DIR}/include/tests/music_to_waveform_png.cpp ${CORE_SRC_EXPORT})

target_link_libraries(music_to_waveform_png PRIVATE CORE_OBJ)

target_include_directories(
    music_to_waveform_png 
    PRIVATE
    ${PDJE_INCLUDE_GLOBAL} 
    ${PDJE_INCLUDE_CORE}
    )


target_link_libraries(music_to_waveform_png PRIVATE PDJE_UTIL_IMAGE_PNG)
target_link_libraries(music_to_waveform_png PRIVATE PDJE_UTIL_IMAGE_WAVEFORM)



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
