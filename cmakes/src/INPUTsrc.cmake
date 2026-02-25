
set(PDJE_INPUT_MAINPROC_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/input/PDJE_Input.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/input/midi/PDJE_MIDI.cpp
    
)
set(PDJE_INPUT_SUBPROC_SRC
    # ${CMAKE_CURRENT_SOURCE_DIR}/include/input/runner/SetTXRXFeatures.cpp
    
)
if(WIN32)
    list(APPEND PDJE_INPUT_MAINPROC_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/input/DefaultDevs/windows/DefaultDevs.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/input/DefaultDevs/windows/TXRX/SetFeatures.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/input/DefaultDevs/windows/TXRX/MetadataTXRX.cpp
    )
    list(APPEND PDJE_INPUT_SUBPROC_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/include/input/runner/windows/SubMain.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/input/runner/windows/SubProcess.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/input/runner/windows/InputLoop.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/input/runner/SetTXRXFeatures.cpp
    )
    function(PDJE_INPUT_LINK_LIB targetName)
        target_link_libraries(${targetName} PUBLIC user32 avrt winmm hid)
    endfunction(PDJE_INPUT_LINK_LIB)



elseif(APPLE)
    
else()
    list(APPEND PDJE_INPUT_MAINPROC_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/include/input/DefaultDevs/linux/DefaultDevs.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/input/DefaultDevs/linux/InputCore.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/input/DefaultDevs/linux/WaylandRuntimeLoader.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/input/DefaultDevs/linux/ParseMouse.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/input/DefaultDevs/linux/ParseKeyboard.cpp
        
        
        
        # ${CMAKE_CURRENT_SOURCE_DIR}/include/input/host/linux/MainProcess.cpp
        # ${CMAKE_CURRENT_SOURCE_DIR}/include/input/host/linux/RTEvent.cpp
        
        # ${CMAKE_CURRENT_SOURCE_DIR}/include/input/host/linux/ipc_Send_Linux.cpp
    )
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(LIBEVDEV REQUIRED IMPORTED_TARGET libevdev)
    pkg_check_modules(LIBNUMA REQUIRED IMPORTED_TARGET numa)
    pkg_check_modules(LIBSYSTEMD REQUIRED IMPORTED_TARGET libsystemd)
    

    function(PDJE_INPUT_LINK_LIB targetName)
        target_link_libraries(${targetName} PUBLIC
            PkgConfig::LIBEVDEV
            PkgConfig::LIBSYSTEMD
            ${LIBNUMA_LIBRARIES}
            ${CMAKE_DL_LIBS}
        )
        target_include_directories(${targetName} PUBLIC ${LIBNUMA_INCLUDE_DIRS})
    endfunction(PDJE_INPUT_LINK_LIB)

endif()
