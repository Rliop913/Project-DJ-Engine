
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
)
endif()