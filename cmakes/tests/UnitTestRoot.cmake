
include(${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake)

if(WIN32)
    set(PDJE_TEST_PLATFORM_LABEL "windows")
elseif(APPLE)
    set(PDJE_TEST_PLATFORM_LABEL "macos")
else()
    set(PDJE_TEST_PLATFORM_LABEL "linux")
endif()

function(pdje_copy_zlib_runtime targetName)
    if(NOT WIN32)
        return()
    endif()

    add_custom_command(
        TARGET ${targetName}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "$<$<CONFIG:Debug>:${zlib_BIN_DIRS_DEBUG}/zlib1.dll>$<$<OR:$<CONFIG:Release>,$<CONFIG:RelWithDebInfo>,$<CONFIG:MinSizeRel>>:${zlib_BIN_DIRS_RELEASE}/zlib1.dll>"
                $<TARGET_FILE_DIR:${targetName}>
        VERBATIM)
endfunction()

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



if(NOT APPLE)
    include(cmakes/tests/units/Module_Util.cmake)
endif()
if(PDJE_DEVELOP_INPUT)
    include(cmakes/tests/units/Module_Input.cmake)
    include(cmakes/tests/units/Module_Judge.cmake)
endif()
include(cmakes/tests/units/Module_Core.cmake)
