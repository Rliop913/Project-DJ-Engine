
if(WIN32)
set(PDJE_IPC_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/input/IPC/memory/windows/named_mutex.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/input/IPC/memory/windows/named_event.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/input/IPC/memory/windows/Input_Transfer.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/include/input/IPC/memory/Input_Transfer.cpp
    
    
)
elseif(APPLE)
set(PDJE_IPC_SRC

)
else()
set(PDJE_IPC_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/input/IPC/memory/linux/named_mutex.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/input/IPC/memory/linux/named_event.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/input/IPC/memory/linux/Input_Transfer.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/include/input/IPC/memory/Input_Transfer.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/include/input/IPC/transmission/linux/MainProcess.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/include/input/IPC/transmission/linux/ChildProcess.cpp
)
endif()

list(APPEND PDJE_IPC_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/input/IPC/memory/Secured_IPC_TX_RX.cpp
)