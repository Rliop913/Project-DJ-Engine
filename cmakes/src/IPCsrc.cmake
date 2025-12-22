
if(WIN32)
set(PDJE_IPC_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/input/host/windows/MainProcess.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/input/runner/windows/SubProcess.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/input/runner/windows/InputLoop.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/input/IPC/memory/windows/named_mutex.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/input/IPC/memory/windows/Input_Transfer.cpp
    
    
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

list(APPEND PDJE_IPC_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/input/IPC/memory/Secured_IPC_TX_RX.cpp
)