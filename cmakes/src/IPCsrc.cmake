
if(WIN32)
set(PDJE_IPC_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/input/IPC/transmission/windows/MainProcess.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/input/IPC/transmission/windows/ChildProcess.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/input/IPC/transmission/windows/InputLoop.cpp
    
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