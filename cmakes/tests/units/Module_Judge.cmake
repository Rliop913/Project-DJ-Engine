
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
doctest::doctest JUDGE_OBJ)

setJudgeReqs(pdje_unit_judge)

target_compile_definitions(pdje_unit_judge PRIVATE PDJE_UNIT_TESTING)
PDJE_COMPILE_OPTION(pdje_unit_judge)
SET_PROPERTIES(pdje_unit_judge)
# AddDynamicDef(pdje_unit_judge)
# setJudgeReqs(pdje_unit_judge)
pdje_discover_unit_tests(pdje_unit_judge judge)