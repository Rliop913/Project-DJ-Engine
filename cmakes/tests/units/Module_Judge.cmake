
add_executable(
pdje_unit_judge
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/main_doctest.cpp
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/judge/judge_algorithms.test.cpp
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/judge/rail_db.test.cpp
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/judge/note_obj.test.cpp
# ${CMAKE_CURRENT_SOURCE_DIR}/include/judge/PDJE_RAIL.cpp
# ${CMAKE_CURRENT_SOURCE_DIR}/include/judge/NoteOBJ/PDJE_Note_OBJ.cpp
${JUDGE_SRC_EXPORT}
)
target_include_directories(
pdje_unit_judge PRIVATE 
${PDJE_INCLUDE_JUDGE} 
${PDJE_INCLUDE_INPUT_MAINPROC})
target_link_libraries(pdje_unit_judge PRIVATE 
doctest::doctest JUDGE_OBJ IPC_OBJ CRYPTO_OBJ)

setJudgeReqs(pdje_unit_judge)

target_compile_definitions(pdje_unit_judge PRIVATE PDJE_UNIT_TESTING)
PDJE_COMPILE_OPTION(pdje_unit_judge)
SET_PROPERTIES(pdje_unit_judge)
# AddDynamicDef(pdje_unit_judge)
# setJudgeReqs(pdje_unit_judge)
pdje_discover_unit_tests(pdje_unit_judge judge)


# Header-only axis core: no judge/input runtime or device dependencies.
add_executable(pdje_unit_axis_model
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/main_doctest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/judge/axis_model.test.cpp)
target_include_directories(pdje_unit_axis_model PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/judge/AxisModel)
target_link_libraries(pdje_unit_axis_model PRIVATE doctest::doctest)
target_compile_features(pdje_unit_axis_model PRIVATE cxx_std_20)
pdje_discover_unit_tests(pdje_unit_axis_model judge)


# Exercise real axis adapters and Match dispatch without the unrelated full
# engine/global implementation objects (or physical MIDI/input devices).
add_executable(pdje_unit_axis_input
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/main_doctest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/judge/axis_input.test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/judge/midi_axis.test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/judge/midi_pipeline.test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/judge/judge_midi_only.test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/judge/PDJE_Judge.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/judge/Loop/PDJE_Judge_Loop.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/judge/Loop/PreProcess/PDJE_PreProcess.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/judge/Loop/PreProcess/PDJE_PreProcess_Work.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/judge/Loop/Match/Keyboard.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/input/midi/PDJE_MIDI.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/judge/Init/PDJE_Judge_Init.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/judge/PDJE_RAIL.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/judge/NoteOBJ/PDJE_Note_OBJ.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/judge/Loop/Match/PDJE_Match.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/judge/Loop/Match/MIDI.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/judge/Loop/Match/Mouse.cpp)
target_include_directories(pdje_unit_axis_input PRIVATE ${PDJE_INCLUDE_INPUT_MAINPROC})
target_link_libraries(pdje_unit_axis_input PRIVATE
    doctest::doctest JUDGE_INCLUDE GLOBAL_INCLUDE IPC_INCLUDE CRYPTO_INCLUDE
    libremidi nlohmann_json::nlohmann_json PDJE_LOG_RUNTIME IPC_OBJ CRYPTO_OBJ)
setSpdlogReqLib(pdje_unit_axis_input)
DynamicInnerFlag(pdje_unit_axis_input)
setBotanReqLib(pdje_unit_axis_input)
LinuxSetAtomic(pdje_unit_axis_input)
target_compile_features(pdje_unit_axis_input PRIVATE cxx_std_20)
pdje_discover_unit_tests(pdje_unit_axis_input judge)