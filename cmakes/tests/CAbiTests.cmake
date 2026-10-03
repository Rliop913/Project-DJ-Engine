add_executable(
  pdje_unit_cabi_core
  ${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/main_doctest.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/core/cpdje_interface_c_api.test.cpp
)
target_include_directories(
  pdje_unit_cabi_core
  PRIVATE
    ${PDJE_INCLUDE_CORE}
)
target_link_libraries(
  pdje_unit_cabi_core
  PRIVATE
    doctest::doctest
    CPDJE
)
setSqliteReqLib(pdje_unit_cabi_core)
target_compile_definitions(pdje_unit_cabi_core PRIVATE PDJE_UNIT_TESTING)
PDJE_COMPILE_OPTION(pdje_unit_cabi_core)
SET_PROPERTIES(pdje_unit_cabi_core)
pdje_copy_zlib_runtime(pdje_unit_cabi_core)
pdje_discover_unit_tests(pdje_unit_cabi_core cabi)

add_executable(
  pdje_cabi_core_smoke
  ${CMAKE_CURRENT_SOURCE_DIR}/tests/smoke/cpdje_interface_smoke.c
)
target_link_libraries(pdje_cabi_core_smoke PRIVATE CPDJE)
PDJE_COMPILE_OPTION(pdje_cabi_core_smoke)
SET_PROPERTIES(pdje_cabi_core_smoke)
pdje_copy_zlib_runtime(pdje_cabi_core_smoke)
add_test(NAME unit.cabi::core_c_smoke COMMAND pdje_cabi_core_smoke)
set_tests_properties(
  unit.cabi::core_c_smoke
  PROPERTIES LABELS "unit;cabi;core;${PDJE_TEST_PLATFORM_LABEL}"
)

if(PDJE_DEVELOP_INPUT)
  add_executable(
    pdje_unit_cabi_modules
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/main_doctest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/input/cpdje_input_c_api.test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/judge/cpdje_judge_c_api.test.cpp
  )
  target_include_directories(
    pdje_unit_cabi_modules
    PRIVATE
      ${PDJE_INCLUDE_CORE}
      ${PDJE_INCLUDE_ROOT}/include/input
      ${PDJE_INCLUDE_JUDGE}
  )
  target_link_libraries(
    pdje_unit_cabi_modules
    PRIVATE
      doctest::doctest
      CPDJE
      CPDJE_MODULE_INPUT
      CPDJE_MODULE_JUDGE
  )
  target_compile_definitions(pdje_unit_cabi_modules PRIVATE PDJE_UNIT_TESTING)
  PDJE_COMPILE_OPTION(pdje_unit_cabi_modules)
  SET_PROPERTIES(pdje_unit_cabi_modules)
  pdje_copy_zlib_runtime(pdje_unit_cabi_modules)
  pdje_discover_unit_tests(pdje_unit_cabi_modules cabi)

  add_executable(
    pdje_cabi_input_smoke
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/smoke/cpdje_input_smoke.c
  )
  target_link_libraries(pdje_cabi_input_smoke PRIVATE CPDJE_MODULE_INPUT)
  PDJE_COMPILE_OPTION(pdje_cabi_input_smoke)
  SET_PROPERTIES(pdje_cabi_input_smoke)
  add_test(NAME unit.cabi::input_c_smoke COMMAND pdje_cabi_input_smoke)
  set_tests_properties(
    unit.cabi::input_c_smoke
    PROPERTIES LABELS "unit;cabi;input;${PDJE_TEST_PLATFORM_LABEL}"
  )

  add_executable(
    pdje_cabi_judge_smoke
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/smoke/cpdje_judge_smoke.c
  )
  target_link_libraries(pdje_cabi_judge_smoke PRIVATE CPDJE_MODULE_JUDGE)
  PDJE_COMPILE_OPTION(pdje_cabi_judge_smoke)
  SET_PROPERTIES(pdje_cabi_judge_smoke)
  add_test(NAME unit.cabi::judge_c_smoke COMMAND pdje_cabi_judge_smoke)
  set_tests_properties(
    unit.cabi::judge_c_smoke
    PROPERTIES LABELS "unit;cabi;judge;${PDJE_TEST_PLATFORM_LABEL}"
  )
endif()
