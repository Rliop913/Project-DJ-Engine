
set(PDJE_INCLUDE_CORE
    ${CMAKE_CURRENT_SOURCE_DIR}/core_include
  ${CMAKE_CURRENT_SOURCE_DIR}/core_include/db
  ${CMAKE_CURRENT_SOURCE_DIR}/core_include/audioRender
  ${CMAKE_CURRENT_SOURCE_DIR}/core_include/audioRender/MixMachine
  ${CMAKE_CURRENT_SOURCE_DIR}/core_include/audioRender/MixMachine/EFFECT
  ${CMAKE_CURRENT_SOURCE_DIR}/core_include/audioRender/MixMachine/FAUST_VALS
  
  ${CMAKE_CURRENT_SOURCE_DIR}/core_include/audioRender/MixMachine/MUSIC_CTR
  
  ${CMAKE_CURRENT_SOURCE_DIR}/core_include/audioRender/MixMachine/MiniaudioObjects
  
  ${CMAKE_CURRENT_SOURCE_DIR}/core_include/audioRender/MixMachine/INTERPOLATOR

  ${CMAKE_CURRENT_SOURCE_DIR}/core_include/audioRender/ManualMix
  ${CMAKE_CURRENT_SOURCE_DIR}/core_include/audioRender/ManualMix/ManualFausts
  ${CMAKE_CURRENT_SOURCE_DIR}/core_include/db/Capnp/CapnpBinary
  ${CMAKE_CURRENT_SOURCE_DIR}/core_include/db/Capnp/Translators
  ${CMAKE_CURRENT_SOURCE_DIR}/core_include/db/Capnp/Translators/MixTranslator
  ${CMAKE_CURRENT_SOURCE_DIR}/core_include/db/Capnp/Translators/MusicTranslator
  ${CMAKE_CURRENT_SOURCE_DIR}/core_include/db/Capnp/Translators/NoteTranslator

  ${CMAKE_CURRENT_SOURCE_DIR}/third_party/Capnp
	${miniaudio_SOURCE_DIR}/extras/miniaudio_split
  ${SQLite3_INCLUDE_DIR}
  ${highway_INCLUDE_DIR}

	${CMAKE_CURRENT_SOURCE_DIR}/third_party/SoundTouch/soundtouch_include
	${CMAKE_CURRENT_SOURCE_DIR}/third_party/Faust/compiled
  ${CMAKE_CURRENT_SOURCE_DIR}/third_party/Faust/manual_compiled
  

  ${CMAKE_CURRENT_SOURCE_DIR}/core_include/input
  ${CMAKE_CURRENT_SOURCE_DIR}/core_include/input/linux
  ${CMAKE_CURRENT_SOURCE_DIR}/core_include/input/macos
  
  ${CMAKE_CURRENT_SOURCE_DIR}/core_include/MainObjects
  ${CMAKE_CURRENT_SOURCE_DIR}/core_include/MainObjects/audioPlayer
  ${CMAKE_CURRENT_SOURCE_DIR}/core_include/MainObjects/editorObject
  ${CMAKE_CURRENT_SOURCE_DIR}/core_include/MainObjects/tempDBObject
  

  ${CMAKE_CURRENT_SOURCE_DIR}/core_include/interface



  ${CMAKE_CURRENT_SOURCE_DIR}/core_include/editor
  ${CMAKE_CURRENT_SOURCE_DIR}/core_include/editor/edit
  ${CMAKE_CURRENT_SOURCE_DIR}/core_include/editor/featureWrapper
  ${CMAKE_CURRENT_SOURCE_DIR}/core_include/editor/featureWrapper/GitWrap
  ${CMAKE_CURRENT_SOURCE_DIR}/core_include/editor/featureWrapper/GitWrap/Branch
  ${CMAKE_CURRENT_SOURCE_DIR}/core_include/editor/featureWrapper/GitWrap/Commit
  ${CMAKE_CURRENT_SOURCE_DIR}/core_include/editor/featureWrapper/GitWrap/Add
  ${CMAKE_CURRENT_SOURCE_DIR}/core_include/editor/featureWrapper/GitWrap/Blame
  ${CMAKE_CURRENT_SOURCE_DIR}/core_include/editor/featureWrapper/GitWrap/Diff
  ${CMAKE_CURRENT_SOURCE_DIR}/core_include/editor/featureWrapper/GitWrap/Log
  ${CMAKE_CURRENT_SOURCE_DIR}/core_include/editor/pdjeLinter
  
  ${CMAKE_CURRENT_SOURCE_DIR}/core_include/editor/featureWrapper/JSONWrap
  ${CMAKE_CURRENT_SOURCE_DIR}/core_include/editor/objects
  ${CMAKE_CURRENT_SOURCE_DIR}/core_include/editor/renderer
  
  ${CMAKE_CURRENT_SOURCE_DIR}/global_include
  ${CMAKE_CURRENT_SOURCE_DIR}/global_include/DataLines

  ${nlohmann_json_SOURCE_DIR}/include
  ${sql_amalgam_SOURCE_DIR}
  ${cppcodec_SOURCE_DIR}
  )