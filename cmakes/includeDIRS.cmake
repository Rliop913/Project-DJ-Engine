
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
  ${PDJE_INCLUDE_ROOT}/include/core/db/Capnp/CapnpBinary
  ${PDJE_INCLUDE_ROOT}/include/core/db/Capnp/Translators
  ${PDJE_INCLUDE_ROOT}/include/core/db/Capnp/Translators/MixTranslator
  ${PDJE_INCLUDE_ROOT}/include/core/db/Capnp/Translators/MusicTranslator
  ${PDJE_INCLUDE_ROOT}/include/core/db/Capnp/Translators/NoteTranslator

	${miniaudio_SOURCE_DIR}/extras/miniaudio_split
  ${SQLite3_INCLUDE_DIR}
  ${highway_INCLUDE_DIR}

	${PDJE_INCLUDE_ROOT}/third_party/SoundTouch/soundtouch_include
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

  #######################DEPRECATED
  # ${PDJE_INCLUDE_ROOT}/include/core/editor/featureWrapper
  # ${PDJE_INCLUDE_ROOT}/include/core/editor/featureWrapper/GitWrap
  # ${PDJE_INCLUDE_ROOT}/include/core/editor/featureWrapper/GitWrap/Branch
  # ${PDJE_INCLUDE_ROOT}/include/core/editor/featureWrapper/GitWrap/Commit
  # ${PDJE_INCLUDE_ROOT}/include/core/editor/featureWrapper/GitWrap/Add
  # ${PDJE_INCLUDE_ROOT}/include/core/editor/featureWrapper/GitWrap/Blame
  # ${PDJE_INCLUDE_ROOT}/include/core/editor/featureWrapper/GitWrap/Diff
  # ${PDJE_INCLUDE_ROOT}/include/core/editor/featureWrapper/GitWrap/Log
  #######################END DEPRECATED
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

set(PDJE_INCLUDE_JUDGE
${PDJE_INCLUDE_ROOT}/include/judge
${PDJE_INCLUDE_ROOT}/include/judge/NoteOBJ
${PDJE_INCLUDE_ROOT}/include/judge/Init
${PDJE_INCLUDE_ROOT}/include/judge/Loop
${CAPNPC_OUTPUT_DIR}/third_party/Capnp
)

set(PDJE_INCLUDE_IPC
${PDJE_INCLUDE_ROOT}/include/input/IPC/
${PDJE_INCLUDE_ROOT}/include/input/IPC/common
${PDJE_INCLUDE_ROOT}/include/input/IPC/memory
${PDJE_INCLUDE_ROOT}/include/input/IPC/transmission
${httplib_SOURCE_DIR}
)


set(PDJE_INCLUDE_INPUT
${PDJE_INCLUDE_ROOT}/include/input
${PDJE_INCLUDE_ROOT}/include/input/midi
)

set(PDJE_INCLUDE_GLOBAL
  ${PDJE_INCLUDE_ROOT}/include/global
  ${PDJE_INCLUDE_ROOT}/include/global/DataLines
  ${PDJE_INCLUDE_ROOT}/include/global/Crypto
)
if(WIN32)
  list(APPEND PDJE_INCLUDE_GLOBAL
    ${PDJE_INCLUDE_ROOT}/include/global/Highres_Clock/Windows
  )
  list(APPEND PDJE_INCLUDE_INPUT
    ${PDJE_INCLUDE_ROOT}/include/input/windows
  )
  list(APPEND PDJE_INCLUDE_IPC
    ${PDJE_INCLUDE_ROOT}/include/input/IPC/memory/windows
    ${PDJE_INCLUDE_ROOT}/include/input/IPC/transmission/windows
  )
elseif(APPLE)
  list(APPEND PDJE_INCLUDE_GLOBAL
    ${PDJE_INCLUDE_ROOT}/include/global/Highres_Clock/Mac
  )
  list(APPEND PDJE_INCLUDE_INPUT ${PDJE_INCLUDE_ROOT}/include/input/apple)
  # list(APPEND PDJE_INCLUDE_IPC
  #   ${PDJE_INCLUDE_ROOT}/include/global/Process/utils/mac
  # )
else()
  list(APPEND PDJE_INCLUDE_GLOBAL
    ${PDJE_INCLUDE_ROOT}/include/global/Highres_Clock/Linux
  )
  list(APPEND PDJE_INCLUDE_INPUT
    ${PDJE_INCLUDE_ROOT}/include/input/linux 
    ${PDJE_INCLUDE_ROOT}/include/input/linux/RT
    ${PDJE_INCLUDE_ROOT}/include/input/linux/common
    ${PDJE_INCLUDE_ROOT}/include/input/linux/socket
    
    )
  list(APPEND PDJE_INCLUDE_IPC
    ${PDJE_INCLUDE_ROOT}/include/input/IPC/memory/linux
    ${PDJE_INCLUDE_ROOT}/include/input/IPC/transmission/linux
  )
endif()

list(APPEND PDJE_INCLUDE_JUDGE
  ${PDJE_INCLUDE_GLOBAL}
  ${PDJE_INCLUDE_IPC}
)
list(APPEND PDJE_INCLUDE_CORE
  ${PDJE_INCLUDE_GLOBAL}
)
list(APPEND PDJE_INCLUDE_INPUT
  ${PDJE_INCLUDE_GLOBAL}
  ${PDJE_INCLUDE_IPC}
)
