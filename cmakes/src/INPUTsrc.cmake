
if(WIN32)
set(PDJE_OS_INPUT_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/input/PDJE_Input.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/input/windows/QPC_Timer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/input/windows/windows_input.cpp

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
    ${CMAKE_CURRENT_SOURCE_DIR}/include/input/linux/linux_input.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/input/linux/RT/RTSocket.cpp
    
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBEVDEV REQUIRED IMPORTED_TARGET libevdev)
pkg_check_modules(LIBNUMA REQUIRED IMPORTED_TARGET numa)


function(PDJE_INPUT_LINK_LIB targetName)
    target_link_libraries(${targetName} PUBLIC PkgConfig::LIBEVDEV ${LIBNUMA_LIBRARIES})
    target_include_directories(${targetName} PUBLIC ${LIBNUMA_INCLUDE_DIRS})
endfunction(PDJE_INPUT_LINK_LIB)

endif()