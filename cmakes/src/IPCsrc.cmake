
if(WIN32)
set(PDJE_IPC_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/input/host/windows/MainProcess.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/input/runner/windows/SubProcess.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/input/runner/windows/InputLoop.cpp
    
)
elseif(APPLE)
set(PDJE_IPC_SRC

)
else()
set(PDJE_IPC_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/input/IPC/transmission/linux/MainProcess.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/input/IPC/transmission/linux/ChildProcess.cpp
)
endif()