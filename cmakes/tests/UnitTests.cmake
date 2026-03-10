
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



add_executable(
pdje_unit_core
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/main_doctest.cpp
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/util/public_headers.test.cpp
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/util/png_writer.test.cpp
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/util/waveform_png.test.cpp
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/util/db_relational_sqlite.test.cpp
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/util/db_keyvalue_rocksdb.test.cpp
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/util/db_nearest_annoy.test.cpp
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/util/status.test.cpp
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/core/name_sanitizer.test.cpp
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/core/atomic_double_buffer.test.cpp
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/core/framecalc_countframe.test.cpp
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/core/bpm_struct.test.cpp
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/core/json_diff_friendly_io.test.cpp
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/core/track_linter_rules.test.cpp
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/core/timeline_diff_types.test.cpp
${CMAKE_CURRENT_SOURCE_DIR}/include/core/db/Capnp/Translators/FrameCalc.cpp
${CMAKE_CURRENT_SOURCE_DIR}/include/global/DataLines/fileNameSanitizer.cpp
${CORE_SRCS})


target_include_directories(pdje_unit_core PRIVATE ${PDJE_INCLUDE_GLOBAL} ${PDJE_INCLUDE_CORE})
target_link_libraries(pdje_unit_core PRIVATE doctest::doctest)
target_link_libraries(pdje_unit_core PRIVATE PDJE_UTIL)
target_link_libraries(pdje_unit_core PRIVATE PDJE_UTIL_IMAGE_PNG)
target_link_libraries(pdje_unit_core PRIVATE PDJE_UTIL_IMAGE_WAVEFORM)
target_link_libraries(pdje_unit_core PRIVATE PDJE_UTIL_DB)
target_compile_definitions(pdje_unit_core PRIVATE PDJE_UNIT_TESTING)
if(PDJE_DYNAMIC)
if(WIN32)
target_compile_definitions(pdje_unit_core PUBLIC PDJE_WINDOWS_DLL)
target_compile_definitions(pdje_unit_core PRIVATE PDJE_BUILDING)
endif()
endif()

if(WIN32)
set(_pdje_unit_core_zlib_runtime
    "$<$<CONFIG:Debug>:${zlib_BIN_DIRS_DEBUG}/zlib1.dll>$<$<CONFIG:Release>:${zlib_BIN_DIRS_RELEASE}/zlib1.dll>$<$<CONFIG:RelWithDebInfo>:${zlib_BIN_DIRS_RELEASE}/zlib1.dll>$<$<CONFIG:MinSizeRel>:${zlib_BIN_DIRS_RELEASE}/zlib1.dll>")
add_custom_command(
    TARGET pdje_unit_core
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${_pdje_unit_core_zlib_runtime}"
    $<TARGET_FILE_DIR:pdje_unit_core>
    VERBATIM)
endif()
setSpdlogReqLib(pdje_unit_core)
setPdjeLogRuntimeReqLib(pdje_unit_core)
setCapnpReqLib(pdje_unit_core)
setLibgit2ReqLib(pdje_unit_core)
PDJE_COMPILE_OPTION(pdje_unit_core)
SET_PROPERTIES(pdje_unit_core)
pdje_discover_unit_tests(pdje_unit_core core)

add_executable(
pdje_unit_input
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/main_doctest.cpp
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/input/input_sanity.test.cpp
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/input/pdje_input_state_logic.test.cpp
${INPUT_SRCS})
if(LINUX)
target_sources(
    pdje_unit_input
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/input/linux_evdev_codemap.test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/input/linux_defaultdev_contracts.test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/input/linux_mouse_button_mapping.test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/input/linux_wayland_runtime_loader.test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/input/DefaultDevs/linux/wayland_things/WaylandRuntimeLoader.cpp)
endif()
target_include_directories(pdje_unit_input PRIVATE ${PDJE_INCLUDE_INPUT_MAINPROC})
target_link_libraries(pdje_unit_input PRIVATE doctest::doctest)
if(LINUX)
target_link_libraries(pdje_unit_input PRIVATE PkgConfig::LIBEVDEV)
target_link_libraries(pdje_unit_input PRIVATE ${CMAKE_DL_LIBS})
endif()
target_compile_definitions(pdje_unit_input PRIVATE PDJE_UNIT_TESTING)
PDJE_COMPILE_OPTION(pdje_unit_input)
SET_PROPERTIES(pdje_unit_input)
pdje_discover_unit_tests(pdje_unit_input input)

if(PDJE_DYNAMIC)
if(WIN32)
    target_compile_definitions(pdje_unit_input PUBLIC PDJE_WINDOWS_DLL)
    target_compile_definitions(pdje_unit_input PRIVATE PDJE_BUILDING)
endif()
endif()

add_executable(
pdje_unit_judge
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/main_doctest.cpp
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/judge/judge_algorithms.test.cpp
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/judge/rail_db.test.cpp
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/judge/note_obj.test.cpp
${CMAKE_CURRENT_SOURCE_DIR}/include/judge/PDJE_RAIL.cpp
${CMAKE_CURRENT_SOURCE_DIR}/include/judge/NoteOBJ/PDJE_Note_OBJ.cpp
${JUDGE_SRCS})
target_include_directories(
pdje_unit_judge PRIVATE ${PDJE_INCLUDE_JUDGE} ${PDJE_INCLUDE_INPUT_MAINPROC})
target_link_libraries(pdje_unit_judge PRIVATE doctest::doctest)
target_compile_definitions(pdje_unit_judge PRIVATE PDJE_UNIT_TESTING)
PDJE_COMPILE_OPTION(pdje_unit_judge)
SET_PROPERTIES(pdje_unit_judge)
pdje_discover_unit_tests(pdje_unit_judge judge)

if(PDJE_DYNAMIC)
if(WIN32)
    target_compile_definitions(pdje_unit_judge PUBLIC PDJE_WINDOWS_DLL)
    target_compile_definitions(pdje_unit_judge PRIVATE PDJE_BUILDING)
endif()
endif()
add_executable(
pdje_unit_interface
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/main_doctest.cpp
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/core/pdje_interface_pcm.test.cpp
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/core/editor_object_diff_api.test.cpp)
target_include_directories(
pdje_unit_interface PRIVATE ${PDJE_INCLUDE_GLOBAL} ${PDJE_INCLUDE_CORE})
target_link_libraries(pdje_unit_interface PRIVATE doctest::doctest)
target_link_libraries(pdje_unit_interface PRIVATE PDJE)
target_compile_definitions(pdje_unit_interface PRIVATE PDJE_UNIT_TESTING)
PDJE_COMPILE_OPTION(pdje_unit_interface)
SET_PROPERTIES(pdje_unit_interface)
pdje_discover_unit_tests(pdje_unit_interface core)
if(PDJE_DYNAMIC)
if(WIN32)
    target_compile_definitions(pdje_unit_interface PUBLIC PDJE_WINDOWS_DLL)
    target_compile_definitions(pdje_unit_interface PRIVATE PDJE_BUILDING)
endif()
endif()