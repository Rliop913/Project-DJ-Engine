include(cmakes/src/CORE/DBsrc.cmake)
include(cmakes/src/CORE/AUDIOsrc.cmake)
include(cmakes/src/CORE/EDITORsrc.cmake)


set(PDJE_INCLUDE_CORE
    ${PDJE_INCLUDE_ROOT}/include/core
  ${PDJE_INCLUDE_ROOT}/include/core/db
  ${PDJE_INCLUDE_ROOT}/include/core/audioRender
  ${PDJE_INCLUDE_ROOT}/include/core/audioRender/MixMachine
  ${PDJE_INCLUDE_ROOT}/include/core/audioRender/MixMachine/EFFECT
  ${PDJE_INCLUDE_ROOT}/include/core/audioRender/MixMachine/FAUST_VALS
  
  ${PDJE_INCLUDE_ROOT}/include/core/audioRender/MixMachine/MUSIC_CTR
  
  ${PDJE_INCLUDE_ROOT}/include/core/audioRender/MixMachine/MiniaudioObjects
  
  ${PDJE_INCLUDE_ROOT}/include/core/audioRender/MixMachine/INTERPOLATOR

  ${PDJE_INCLUDE_ROOT}/include/core/audioRender/ManualMix
  ${PDJE_INCLUDE_ROOT}/include/core/audioRender/ManualMix/ManualFausts
  ${PDJE_INCLUDE_ROOT}/include/core/audioRender/ManualMix/PreLoadedMusic
  
  ${PDJE_INCLUDE_ROOT}/include/core/db/Capnp/CapnpBinary
  ${PDJE_INCLUDE_ROOT}/include/core/db/Capnp/Translators
  ${PDJE_INCLUDE_ROOT}/include/core/db/Capnp/Translators/MixTranslator
  ${PDJE_INCLUDE_ROOT}/include/core/db/Capnp/Translators/MusicTranslator
  ${PDJE_INCLUDE_ROOT}/include/core/db/Capnp/Translators/NoteTranslator

	
  
  ${highway_INCLUDE_DIR}

	
	${PDJE_INCLUDE_ROOT}/third_party/Faust/compiled
  ${PDJE_INCLUDE_ROOT}/third_party/Faust/manual_compiled
  

  
  ${PDJE_INCLUDE_ROOT}/include/core/MainObjects
  ${PDJE_INCLUDE_ROOT}/include/core/MainObjects/audioPlayer
  ${PDJE_INCLUDE_ROOT}/include/core/MainObjects/editorObject
  ${PDJE_INCLUDE_ROOT}/include/core/MainObjects/tempDBObject
  

  ${PDJE_INCLUDE_ROOT}/include/core/interface



  ${PDJE_INCLUDE_ROOT}/include/core/editor
  ${PDJE_INCLUDE_ROOT}/include/core/editor/TimeLine
  ${PDJE_INCLUDE_ROOT}/include/core/editor/TimeLine/EventMarker
  ${PDJE_INCLUDE_ROOT}/include/core/editor/TimeLine/LineVersion
  ${PDJE_INCLUDE_ROOT}/include/core/editor/TimeLine/GitDataPack

  ${PDJE_INCLUDE_ROOT}/include/core/editor/pdjeLinter
  
  ${PDJE_INCLUDE_ROOT}/include/core/editor/TimeLine/JSONWrap
  ${PDJE_INCLUDE_ROOT}/include/core/editor/objects
  ${PDJE_INCLUDE_ROOT}/include/core/editor/renderer
  ${CAPNPC_OUTPUT_DIR}/third_party/Capnp
  ${PDJE_INCLUDE_ROOT}/include/input
  ${nlohmann_json_SOURCE_DIR}/include
  ${sql_amalgam_SOURCE_DIR}
  ${cppcodec_SOURCE_DIR}
  )

function(setCoreReqs targetName)
  target_include_directories(${targetName} PUBLIC ${PDJE_INCLUDE_CORE})
  
  DynamicInnerFlag(${targetName})
  target_link_libraries(${targetName} PRIVATE
   nlohmann_json::nlohmann_json
    PDJE_LOG_RUNTIME
    MINIAUDIO_OBJ
    MINIAUDIO_INCLUDE
    SOUNDTOUCH_OBJ
    SOUNDTOUCH_INCLUDE
    SQL_OBJ
    SQL_INCLUDE
    GLOBAL_OBJ
    GLOBAL_INCLUDE
    )
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
    # ${SoundTouch_src} 
    # ${miniaudio_src} 
    ${GLOBAL_SRC_EXPORT}
    # ${globalSource}
)

add_library(CORE_INCLUDE INTERFACE)
target_include_directories(CORE_INCLUDE INTERFACE
  ${PDJE_INCLUDE_CORE}
)

setCoreReqs(CORE_OBJ)

target_link_libraries(CORE_OBJ PRIVATE CORE_INCLUDE)

set(CORE_SRC_EXPORT
$<TARGET_OBJECTS:CORE_OBJ>
${GLOBAL_SRC_EXPORT}
)


set(CORE_SRC_ONLY_EXPORT
$<TARGET_OBJECTS:CORE_OBJ>

)