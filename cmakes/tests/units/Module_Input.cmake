
add_executable(
pdje_unit_input
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/main_doctest.cpp
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/input/input_sanity.test.cpp
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/input/pdje_input_state_logic.test.cpp
${INPUT_SRC_EXPORT}
)

if(LINUX)
target_sources(
    pdje_unit_input
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/input/linux_evdev_codemap.test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/input/linux_defaultdev_contracts.test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/input/linux_mouse_button_mapping.test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/input/linux_wayland_runtime_loader.test.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/include/input/DefaultDevs/linux/wayland_things/WaylandRuntimeLoader.cpp
    )
    target_link_libraries(pdje_unit_input PRIVATE PkgConfig::LIBEVDEV)
    target_link_libraries(pdje_unit_input PRIVATE ${CMAKE_DL_LIBS})
endif()

if(WIN32)
target_sources(
    pdje_unit_input
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/input/windows_keyboard_press_tracker.test.cpp
    )
endif()

target_include_directories(pdje_unit_input PRIVATE ${PDJE_INCLUDE_INPUT_MAINPROC})
target_link_libraries(pdje_unit_input PRIVATE 
doctest::doctest 
INPUT_OBJ 
INPUT_MAIN_INCLUDE 
CRYPTO_OBJ
CRYPTO_INCLUDE
GLOBAL_OBJ
GLOBAL_INCLUDE
)

setInputReqs(pdje_unit_input)

target_compile_definitions(pdje_unit_input PRIVATE PDJE_UNIT_TESTING)
PDJE_COMPILE_OPTION(pdje_unit_input)
SET_PROPERTIES(pdje_unit_input)
# AddDynamicDef(pdje_unit_input)

pdje_discover_unit_tests(pdje_unit_input input)
