include(cmakes/src/GLOBAL/CRYPTOsrc.cmake)
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

set(PDJE_INCLUDE_IPC
${PDJE_INCLUDE_ROOT}/include/input
${PDJE_INCLUDE_ROOT}/include/input/IPC/
${PDJE_INCLUDE_ROOT}/include/input/IPC/common
${PDJE_INCLUDE_ROOT}/include/input/IPC/memory
${PDJE_INCLUDE_ROOT}/include/input/IPC/transmission
${PDJE_INCLUDE_CRYPTO}
)

if(WIN32)
    list(APPEND PDJE_INCLUDE_IPC
    ${PDJE_INCLUDE_ROOT}/include/input/IPC/memory/windows
    ${PDJE_INCLUDE_ROOT}/include/input/IPC/transmission/windows

  )
elseif(APPLE)

else()
    list(APPEND PDJE_INCLUDE_IPC
    ${PDJE_INCLUDE_ROOT}/include/input/IPC/memory/linux
    
    )

endif(WIN32)




add_library(IPC_OBJ OBJECT
${PDJE_IPC_SRC}
${CRYPTO_SRC_EXPORT}
)


target_include_directories(IPC_OBJ PUBLIC 
${PDJE_INCLUDE_IPC}
)
target_link_libraries(IPC_OBJ PUBLIC CRYPTO_OBJ)

set(IPC_SRC_EXPORT
${CRYPTO_SRC_EXPORT}
$<TARGET_OBJECTS:IPC_OBJ>
)