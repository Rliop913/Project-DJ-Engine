include(cmakes/src/INPUT/INPUTsrc.cmake)


function(setInputReqs targetName)
  DynamicInnerFlag(${targetName})
  # target_sources(${targetName} PUBLIC ${IPC_SRC_EXPORT})
  target_link_libraries(${targetName} PUBLIC IPC_OBJ)
  target_include_directories(${targetName} PUBLIC ${PDJE_INCLUDE_INPUT_MAINPROC} )
  setBotanReqLib(${targetName})
  PDJE_INPUT_LINK_LIB(${targetName})
  pdje_halide_attach_to_target(${targetName})
  LinuxSetAtomic(${targetName})
  
  SET_PROPERTIES(${targetName})
  PDJE_COMPILE_OPTION(${targetName})
  setLibreMIDIReqLib(${targetName})
endfunction(setInputReqs)


add_library(INPUT_OBJ OBJECT
${GLOBAL_SRC_EXPORT}
${PDJE_INPUT_MAINPROC_SRC}
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
