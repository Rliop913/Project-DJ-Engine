
add_executable(
pdje_unit_core
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/main_doctest.cpp
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/core/pdje_interface_pcm.test.cpp
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/core/editor_object_diff_api.test.cpp
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/core/name_sanitizer.test.cpp
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/core/atomic_double_buffer.test.cpp
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/core/framecalc_countframe.test.cpp
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/core/bpm_struct.test.cpp
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/core/json_diff_friendly_io.test.cpp
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/core/track_linter_rules.test.cpp
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/core/timeline_diff_types.test.cpp
${CORE_SRC_EXPORT}
)

target_include_directories(
pdje_unit_core PRIVATE 
${PDJE_INCLUDE_GLOBAL} 
${PDJE_INCLUDE_CORE}
)

target_link_libraries(pdje_unit_core PRIVATE 
doctest::doctest CORE_OBJ)

target_compile_definitions(pdje_unit_core PRIVATE PDJE_UNIT_TESTING)
PDJE_COMPILE_OPTION(pdje_unit_core)
SET_PROPERTIES(pdje_unit_core)


pdje_discover_unit_tests(pdje_unit_core core)