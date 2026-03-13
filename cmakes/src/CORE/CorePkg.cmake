include(cmakes/src/CORE/DBsrc.cmake)
include(cmakes/src/CORE/AUDIOsrc.cmake)
include(cmakes/src/CORE/EDITORsrc.cmake)


function(setCoreReqs targetName)
  target_include_directories(${targetName} PUBLIC ${PDJE_INCLUDE_CORE})
  
  DynamicInnerFlag(${targetName})
  target_link_libraries(${targetName} PUBLIC nlohmann_json::nlohmann_json PDJE_LOG_RUNTIME)
  setSqliteReqLib(${targetName})
  setRocksDBReqLib(${targetName})
  setSpdlogReqLib(${targetName})
  
  setCapnpReqLib(${targetName})
  setLibgit2ReqLib(${targetName})
  setHighwayReqLib(${targetName})
  pdje_halide_attach_to_target(${targetName})
  LinuxSetAtomic(${targetName})
  LinuxSetAudioLibs(${targetName})
  PDJE_COMPILE_OPTION(${targetName})
  setBotanReqLib(${targetName})
  SET_PROPERTIES(${targetName})
endfunction(setCoreReqs)


add_library(CORE_OBJ OBJECT
    ${editorSource} 
    ${dbSource} 
    ${audioRenderSource} 
    ${SoundTouch_src} 
    ${miniaudio_src} 
    ${GLOBAL_SRC_EXPORT}
    # ${globalSource}
)

setCoreReqs(CORE_OBJ)

set(CORE_SRC_EXPORT
$<TARGET_OBJECTS:CORE_OBJ>
${GLOBAL_SRC_EXPORT}
)


set(CORE_SRC_ONLY_EXPORT
$<TARGET_OBJECTS:CORE_OBJ>

)