
if(WIN32)
    set(PDJE_OS_INPUT_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/include/input/PDJE_Input.cpp
    )
    set(PDJE_INPUT_PROCESS_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/include/input/windows/SubMain.cpp
    )
    function(PDJE_INPUT_LINK_LIB targetName)
        target_link_libraries(${targetName} PUBLIC user32 avrt winmm hid)
    endfunction(PDJE_INPUT_LINK_LIB)



elseif(APPLE)
    set(PDJE_OS_INPUT_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/include/input/PDJE_Input.cpp
    )
else()
    set(PDJE_OS_INPUT_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/include/input/PDJE_Input.cpp
        )
        
    set(PDJE_INPUT_PROCESS_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/include/input/linux/SubMain.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/input/linux/ConfigLayer/ConfigLayer.cpp
        
        
    
    )
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(LIBEVDEV REQUIRED IMPORTED_TARGET libevdev)
    pkg_check_modules(LIBNUMA REQUIRED IMPORTED_TARGET numa)


    function(PDJE_INPUT_LINK_LIB targetName)
        target_link_libraries(${targetName} PUBLIC PkgConfig::LIBEVDEV ${LIBNUMA_LIBRARIES})
        target_include_directories(${targetName} PUBLIC ${LIBNUMA_INCLUDE_DIRS})
    endfunction(PDJE_INPUT_LINK_LIB)

endif()