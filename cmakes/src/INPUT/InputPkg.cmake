include(cmakes/src/INPUT/INPUTsrc.cmake)


function(setInputReqs targetName)
  DynamicInnerFlag(${targetName})
  # target_sources(${targetName} PUBLIC ${IPC_SRC_EXPORT})
  target_link_libraries(${targetName} PRIVATE IPC_OBJ CRYPTO_OBJ PDJE_LOG_RUNTIME )
  target_link_libraries(${targetName} PUBLIC IPC_INCLUDE CRYPTO_INCLUDE)
  # target_include_directories(${targetName} 
  # PUBLIC 
  # ${PDJE_INCLUDE_INPUT_MAINPROC} )
  setBotanReqLib(${targetName})
  PDJE_INPUT_LINK_LIB(${targetName})
  pdje_halide_attach_to_target(${targetName})
  LinuxSetAtomic(${targetName})
  setSpdlogReqLib(${targetName})
  SET_PROPERTIES(${targetName})
  PDJE_COMPILE_OPTION(${targetName})
  setLibreMIDIReqLib(${targetName})
endfunction(setInputReqs)


add_library(INPUT_OBJ OBJECT
# ${GLOBAL_SRC_EXPORT}
${PDJE_INPUT_MAINPROC_SRC}
)

add_library(INPUT_MAIN_INCLUDE INTERFACE)
target_include_directories(INPUT_MAIN_INCLUDE
  INTERFACE
  ${PDJE_INCLUDE_ROOT}/include/input
  ${PDJE_INCLUDE_ROOT}/include/input/midi
  ${PDJE_INCLUDE_ROOT}/include/input/host
  ${PDJE_INCLUDE_ROOT}/include/input/runner
  ${PDJE_INCLUDE_ROOT}/include/input/DefaultDevs
)

if(WIN32)
target_include_directories(INPUT_MAIN_INCLUDE
  INTERFACE
  ${PDJE_INCLUDE_ROOT}/include/input/runner/windows
  ${PDJE_INCLUDE_ROOT}/include/input/DefaultDevs/windows
  ${PDJE_INCLUDE_ROOT}/include/input/DefaultDevs/windows/TXRX
)
elseif(APPLE)

else()
target_include_directories(INPUT_MAIN_INCLUDE
  INTERFACE
  ${PDJE_INCLUDE_ROOT}/include/input/DefaultDevs/linux
  ${PDJE_INCLUDE_ROOT}/include/input/DefaultDevs/linux/evdev_things
  ${PDJE_INCLUDE_ROOT}/include/input/DefaultDevs
  ${PDJE_INCLUDE_ROOT}/include/input/DefaultDevs/linux/wayland_things

)
endif()

add_library(INPUT_SUBPROC_INCLUDE INTERFACE)

target_include_directories(INPUT_SUBPROC_INCLUDE INTERFACE
  ${PDJE_INCLUDE_ROOT}/include/input
  ${PDJE_INCLUDE_ROOT}/include/input/runner
  ${PDJE_INCLUDE_ROOT}/include/input/midi
)



target_link_libraries(INPUT_OBJ PRIVATE 
  INPUT_MAIN_INCLUDE
  IPC_OBJ
  IPC_INCLUDE
  CRYPTO_OBJ
  CRYPTO_INCLUDE
  GLOBAL_OBJ 
  GLOBAL_INCLUDE
  )



if(PDJE_TEST)
  target_compile_definitions(INPUT_OBJ PRIVATE PDJE_UNIT_TESTING)
endif()

setInputReqs(INPUT_OBJ)


set(INPUT_SRC_EXPORT
$<TARGET_OBJECTS:INPUT_OBJ>
${GLOBAL_SRC_EXPORT}
${IPC_SRC_EXPORT}
)

set(INPUT_SRC_ONLY_EXPORT
$<TARGET_OBJECTS:INPUT_OBJ>
)
