include(cmakes/src/JUDGE/JUDGEsrc.cmake)

function(setJudgeReqs targetName)
  
  DynamicInnerFlag(${targetName})
  
  # if(WIN32)
  #   add_dependencies(${targetName} gen_RT_HASH)
  #   target_include_directories(${targetName} PUBLIC ${RT_HASH_HEADER_INCLUDE})
  # endif()
  # target_include_directories(${targetName} PUBLIC ${PDJE_INCLUDE_INPUT_MAINPROC} ${PDJE_INCLUDE_JUDGE})
  # target_sources(${targetName} PUBLIC ${IPC_SRC_EXPORT})
  target_link_libraries(${targetName} PRIVATE IPC_OBJ)
  target_link_libraries(${targetName} PUBLIC IPC_INCLUDE)
  setSpdlogReqLib(${targetName})
  setCapnpReqLib(${targetName})
  
  setBotanReqLib(${targetName})
  LinuxSetAtomic(${targetName})
  SET_PROPERTIES(${targetName})
  PDJE_COMPILE_OPTION(${targetName})
  setLibreMIDIReqLib(${targetName})
endfunction(setJudgeReqs)

add_library(JUDGE_OBJ OBJECT
${judgeSource} 
${GLOBAL_SRC_EXPORT}
)

add_library(JUDGE_INCLUDE INTERFACE)

target_include_directories(JUDGE_INCLUDE INTERFACE
  ${PDJE_INCLUDE_JUDGE}
)

target_link_libraries(JUDGE_OBJ PUBLIC JUDGE_INCLUDE GLOBAL_INCLUDE CRYPTO_INCLUDE)

setJudgeReqs(JUDGE_OBJ)


set(JUDGE_SRC_EXPORT
$<TARGET_OBJECTS:JUDGE_OBJ>
${IPC_SRC_EXPORT}
)

set(JUDGE_SRC_ONLY_EXPORT
$<TARGET_OBJECTS:JUDGE_OBJ>

)