
set(PDJE_INCLUDE_CORE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/core
  ${CMAKE_CURRENT_SOURCE_DIR}/include/core/db
  ${CMAKE_CURRENT_SOURCE_DIR}/include/core/audioRender
  ${CMAKE_CURRENT_SOURCE_DIR}/include/core/audioRender/MixMachine
  ${CMAKE_CURRENT_SOURCE_DIR}/include/core/audioRender/MixMachine/EFFECT
  ${CMAKE_CURRENT_SOURCE_DIR}/include/core/audioRender/MixMachine/FAUST_VALS
  
  ${CMAKE_CURRENT_SOURCE_DIR}/include/core/audioRender/MixMachine/MUSIC_CTR
  
  ${CMAKE_CURRENT_SOURCE_DIR}/include/core/audioRender/MixMachine/MiniaudioObjects
  
  ${CMAKE_CURRENT_SOURCE_DIR}/include/core/audioRender/MixMachine/INTERPOLATOR

  ${CMAKE_CURRENT_SOURCE_DIR}/include/core/audioRender/ManualMix
  ${CMAKE_CURRENT_SOURCE_DIR}/include/core/audioRender/ManualMix/ManualFausts
  ${CMAKE_CURRENT_SOURCE_DIR}/include/core/db/Capnp/CapnpBinary
  ${CMAKE_CURRENT_SOURCE_DIR}/include/core/db/Capnp/Translators
  ${CMAKE_CURRENT_SOURCE_DIR}/include/core/db/Capnp/Translators/MixTranslator
  ${CMAKE_CURRENT_SOURCE_DIR}/include/core/db/Capnp/Translators/MusicTranslator
  ${CMAKE_CURRENT_SOURCE_DIR}/include/core/db/Capnp/Translators/NoteTranslator

  ${CMAKE_CURRENT_SOURCE_DIR}/third_party/Capnp
	${miniaudio_SOURCE_DIR}/extras/miniaudio_split
  ${SQLite3_INCLUDE_DIR}
  ${highway_INCLUDE_DIR}

	${CMAKE_CURRENT_SOURCE_DIR}/third_party/SoundTouch/soundtouch_include
	${CMAKE_CURRENT_SOURCE_DIR}/third_party/Faust/compiled
  ${CMAKE_CURRENT_SOURCE_DIR}/third_party/Faust/manual_compiled
  

  ${CMAKE_CURRENT_SOURCE_DIR}/include/core/input
  ${CMAKE_CURRENT_SOURCE_DIR}/include/core/input/linux
  ${CMAKE_CURRENT_SOURCE_DIR}/include/core/input/macos
  
  ${CMAKE_CURRENT_SOURCE_DIR}/include/core/MainObjects
  ${CMAKE_CURRENT_SOURCE_DIR}/include/core/MainObjects/audioPlayer
  ${CMAKE_CURRENT_SOURCE_DIR}/include/core/MainObjects/editorObject
  ${CMAKE_CURRENT_SOURCE_DIR}/include/core/MainObjects/tempDBObject
  

  ${CMAKE_CURRENT_SOURCE_DIR}/include/core/interface



  ${CMAKE_CURRENT_SOURCE_DIR}/include/core/editor
  ${CMAKE_CURRENT_SOURCE_DIR}/include/core/editor/edit
  ${CMAKE_CURRENT_SOURCE_DIR}/include/core/editor/featureWrapper
  ${CMAKE_CURRENT_SOURCE_DIR}/include/core/editor/featureWrapper/GitWrap
  ${CMAKE_CURRENT_SOURCE_DIR}/include/core/editor/featureWrapper/GitWrap/Branch
  ${CMAKE_CURRENT_SOURCE_DIR}/include/core/editor/featureWrapper/GitWrap/Commit
  ${CMAKE_CURRENT_SOURCE_DIR}/include/core/editor/featureWrapper/GitWrap/Add
  ${CMAKE_CURRENT_SOURCE_DIR}/include/core/editor/featureWrapper/GitWrap/Blame
  ${CMAKE_CURRENT_SOURCE_DIR}/include/core/editor/featureWrapper/GitWrap/Diff
  ${CMAKE_CURRENT_SOURCE_DIR}/include/core/editor/featureWrapper/GitWrap/Log
  ${CMAKE_CURRENT_SOURCE_DIR}/include/core/editor/pdjeLinter
  
  ${CMAKE_CURRENT_SOURCE_DIR}/include/core/editor/featureWrapper/JSONWrap
  ${CMAKE_CURRENT_SOURCE_DIR}/include/core/editor/objects
  ${CMAKE_CURRENT_SOURCE_DIR}/include/core/editor/renderer
  
  ${CMAKE_CURRENT_SOURCE_DIR}/include/global
  ${CMAKE_CURRENT_SOURCE_DIR}/include/global/DataLines

  ${CMAKE_CURRENT_SOURCE_DIR}/include/input

  ${nlohmann_json_SOURCE_DIR}/include
  ${sql_amalgam_SOURCE_DIR}
  ${cppcodec_SOURCE_DIR}
  )

set(PDJE_INCLUDE_JUDGE
${CMAKE_CURRENT_SOURCE_DIR}/include/global
${CMAKE_CURRENT_SOURCE_DIR}/include/global/DataLines
${CMAKE_CURRENT_SOURCE_DIR}/include/judge

#external include
# ${CMAKE_CURRENT_SOURCE_DIR}/include/core/audioRender/ManualMix
# ${CMAKE_CURRENT_SOURCE_DIR}/include/core/db/
# ${CMAKE_CURRENT_SOURCE_DIR}/include/core/audioRender/MixMachine/MiniaudioObjects
# ${CMAKE_CURRENT_SOURCE_DIR}/include/core/db/Capnp/Translators
# ${CMAKE_CURRENT_SOURCE_DIR}/include/core/db/Capnp/CapnpBinary
# ${CMAKE_CURRENT_SOURCE_DIR}/third_party/Capnp
)

set(PDJE_INCLUDE_INPUT
${CMAKE_CURRENT_SOURCE_DIR}/include/global
${CMAKE_CURRENT_SOURCE_DIR}/include/global/DataLines
${CMAKE_CURRENT_SOURCE_DIR}/include/input
${CMAKE_CURRENT_SOURCE_DIR}/include/input/midi
)

if(WIN32)
set(PDJE_INCLUDE_INPUT_OS ${CMAKE_CURRENT_SOURCE_DIR}/include/input/windows)
elseif(APPLE)
set(PDJE_INCLUDE_INPUT_OS ${CMAKE_CURRENT_SOURCE_DIR}/include/input/apple)
else()
set(PDJE_INCLUDE_INPUT_OS ${CMAKE_CURRENT_SOURCE_DIR}/include/input/linux 
  ${CMAKE_CURRENT_SOURCE_DIR}/include/input/linux/RT
  ${CMAKE_CURRENT_SOURCE_DIR}/include/input/linux/common
  )
endif()