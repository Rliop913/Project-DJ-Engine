
include(${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake)

if(WIN32)
    set(PDJE_TEST_PLATFORM_LABEL "windows")
elseif(APPLE)
    set(PDJE_TEST_PLATFORM_LABEL "macos")
else()
    set(PDJE_TEST_PLATFORM_LABEL "linux")
endif()

function(pdje_discover_unit_tests targetName moduleLabel)
    set(_pdje_test_list_var "${targetName}_TESTS")
    set(_pdje_dollar "$")
    doctest_discover_tests(
        ${targetName}
        TEST_PREFIX "unit.${moduleLabel}::"
        TEST_LIST ${_pdje_test_list_var})

    set(_pdje_label_script "${CMAKE_CURRENT_BINARY_DIR}/${targetName}_labels.cmake")
    file(
        WRITE
        "${_pdje_label_script}"
        "foreach(_pdje_test_case ${_pdje_dollar}{${_pdje_test_list_var}})\n"
        "  set_tests_properties(${_pdje_dollar}{_pdje_test_case} PROPERTIES LABELS [==[unit;${moduleLabel};${PDJE_TEST_PLATFORM_LABEL}]==])\n"
        "endforeach()\n")
    set_property(DIRECTORY APPEND PROPERTY TEST_INCLUDE_FILES "${_pdje_label_script}")
endfunction()



include(cmakes/tests/units/Module_Util.cmake)
if(PDJE_DEVELOP_INPUT)
    include(cmakes/tests/units/Module_Input.cmake)
    include(cmakes/tests/units/Module_Judge.cmake)
endif()
include(cmakes/tests/units/Module_Core.cmake)
