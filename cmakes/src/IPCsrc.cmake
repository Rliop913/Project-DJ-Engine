
if(WIN32)
set(PDJE_IPC_SRC
    # ${CMAKE_CURRENT_SOURCE_DIR}/include/input/host/windows/MainProcess.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/include/input/runner/windows/SubProcess.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/include/input/runner/windows/InputLoop.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/include/input/IPC/memory/windows/named_mutex.cpp
    
    
)
elseif(APPLE)
set(PDJE_IPC_SRC

)
else()
set(PDJE_IPC_SRC
    # ${CMAKE_CURRENT_SOURCE_DIR}/include/input/host/linux/MainProcess.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/include/input/runner/linux/SubProcess.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/include/input/IPC/memory/linux/named_mutex.cpp
)
endif()

list(APPEND PDJE_IPC_SRC

    ${CMAKE_CURRENT_SOURCE_DIR}/include/input/ConfigLayer/ConfigLayer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/input/util/Secured_IPC_TX_RX.cpp
    
)