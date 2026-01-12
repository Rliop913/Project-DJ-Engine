
set(PDJE_INPUT_MAINPROC_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/input/PDJE_Input.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/input/host/MainProcess.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/input/host/SetTXRXFeatures.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/input/midi/PDJE_MIDI.cpp
    
)
set(PDJE_INPUT_SUBPROC_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/input/runner/SetTXRXFeatures.cpp
    
)
if(WIN32)
    list(APPEND PDJE_INPUT_MAINPROC_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/input/host/windows/MainProcess.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/input/host/windows/ipc_Send_Windows.cpp
    )
    list(APPEND PDJE_INPUT_SUBPROC_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/include/input/runner/windows/SubMain.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/input/runner/windows/SubProcess.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/input/runner/windows/InputLoop.cpp
    )
    function(PDJE_INPUT_LINK_LIB targetName)
        target_link_libraries(${targetName} PUBLIC user32 avrt winmm hid)
    endfunction(PDJE_INPUT_LINK_LIB)



elseif(APPLE)
    
else()
    list(APPEND PDJE_INPUT_MAINPROC_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/include/input/host/linux/MainProcess.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/input/host/linux/ipc_Send_Linux.cpp
    )
    list(APPEND PDJE_INPUT_SUBPROC_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/include/input/runner/linux/SubMain.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/input/runner/linux/SubProcess.cpp
    )
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(LIBEVDEV REQUIRED IMPORTED_TARGET libevdev)
    pkg_check_modules(LIBNUMA REQUIRED IMPORTED_TARGET numa)


    function(PDJE_INPUT_LINK_LIB targetName)
        target_link_libraries(${targetName} PUBLIC PkgConfig::LIBEVDEV ${LIBNUMA_LIBRARIES})
        target_include_directories(${targetName} PUBLIC ${LIBNUMA_INCLUDE_DIRS})
    endfunction(PDJE_INPUT_LINK_LIB)

endif()