
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include/db
  ${CMAKE_CURRENT_SOURCE_DIR}/include/audioRender
  ${CMAKE_CURRENT_SOURCE_DIR}/include/audioRender/MixMachine
  ${CMAKE_CURRENT_SOURCE_DIR}/include/audioRender/MixMachine/EFFECT
  ${CMAKE_CURRENT_SOURCE_DIR}/include/audioRender/MixMachine/FAUST_VALS
  
  ${CMAKE_CURRENT_SOURCE_DIR}/include/audioRender/MixMachine/MUSIC_CTR
  
  ${CMAKE_CURRENT_SOURCE_DIR}/include/audioRender/MixMachine/MiniaudioObjects
  
  ${CMAKE_CURRENT_SOURCE_DIR}/include/audioRender/MixMachine/INTERPOLATOR

  ${CMAKE_CURRENT_SOURCE_DIR}/include/audioRender/ManualMix
  ${CMAKE_CURRENT_SOURCE_DIR}/include/audioRender/ManualMix/ManualFausts
  
  ${CMAKE_CURRENT_SOURCE_DIR}/include/db/Capnp/CapnpBinary
  ${CMAKE_CURRENT_SOURCE_DIR}/include/db/Capnp/Translators
  ${CMAKE_CURRENT_SOURCE_DIR}/include/db/Capnp/Translators/MixTranslator
  ${CMAKE_CURRENT_SOURCE_DIR}/include/db/Capnp/Translators/MusicTranslator
  ${CMAKE_CURRENT_SOURCE_DIR}/include/db/Capnp/Translators/NoteTranslator

  ${CMAKE_CURRENT_SOURCE_DIR}/third_party/Capnp
  ${CMAKE_CURRENT_SOURCE_DIR}/include/ERR
	${miniaudio_SOURCE_DIR}/extras/miniaudio_split
  ${SQLite3_INCLUDE_DIR}
  # ${highway_INCLUDE_DIR}

	${CMAKE_CURRENT_SOURCE_DIR}/third_party/SoundTouch/soundtouch_include
	${CMAKE_CURRENT_SOURCE_DIR}/third_party/Faust/compiled
  ${CMAKE_CURRENT_SOURCE_DIR}/third_party/Faust/manual_compiled
  

  ${CMAKE_CURRENT_SOURCE_DIR}/include/input
  ${CMAKE_CURRENT_SOURCE_DIR}/include/input/linux
  ${CMAKE_CURRENT_SOURCE_DIR}/include/input/macos
  
  ${CMAKE_CURRENT_SOURCE_DIR}/include/MainObjects
  ${CMAKE_CURRENT_SOURCE_DIR}/include/MainObjects/audioPlayer
  ${CMAKE_CURRENT_SOURCE_DIR}/include/MainObjects/editorObject
  ${CMAKE_CURRENT_SOURCE_DIR}/include/MainObjects/tempDBObject
  

  ${CMAKE_CURRENT_SOURCE_DIR}/include/interface



  ${CMAKE_CURRENT_SOURCE_DIR}/include/editor
  ${CMAKE_CURRENT_SOURCE_DIR}/include/editor/edit
  ${CMAKE_CURRENT_SOURCE_DIR}/include/editor/featureWrapper
  ${CMAKE_CURRENT_SOURCE_DIR}/include/editor/featureWrapper/GitWrap
  ${CMAKE_CURRENT_SOURCE_DIR}/include/editor/featureWrapper/GitWrap/Branch
  ${CMAKE_CURRENT_SOURCE_DIR}/include/editor/featureWrapper/GitWrap/Commit
  ${CMAKE_CURRENT_SOURCE_DIR}/include/editor/featureWrapper/GitWrap/Add
  ${CMAKE_CURRENT_SOURCE_DIR}/include/editor/featureWrapper/GitWrap/Blame
  ${CMAKE_CURRENT_SOURCE_DIR}/include/editor/featureWrapper/GitWrap/Diff
  ${CMAKE_CURRENT_SOURCE_DIR}/include/editor/featureWrapper/GitWrap/Log
  
  ${CMAKE_CURRENT_SOURCE_DIR}/include/editor/featureWrapper/JSONWrap
  ${CMAKE_CURRENT_SOURCE_DIR}/include/editor/objects
  ${CMAKE_CURRENT_SOURCE_DIR}/include/editor/renderer
  
  
  



)


set(PDJE_INCLUDE_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include/db
  ${CMAKE_CURRENT_SOURCE_DIR}/include/audioRender
  ${CMAKE_CURRENT_SOURCE_DIR}/include/audioRender/MixMachine
  ${CMAKE_CURRENT_SOURCE_DIR}/include/audioRender/MixMachine/EFFECT
  ${CMAKE_CURRENT_SOURCE_DIR}/include/audioRender/MixMachine/FAUST_VALS
  
  ${CMAKE_CURRENT_SOURCE_DIR}/include/audioRender/MixMachine/MUSIC_CTR
  
  ${CMAKE_CURRENT_SOURCE_DIR}/include/audioRender/MixMachine/MiniaudioObjects
  
  ${CMAKE_CURRENT_SOURCE_DIR}/include/audioRender/MixMachine/INTERPOLATOR

  ${CMAKE_CURRENT_SOURCE_DIR}/include/audioRender/ManualMix
  ${CMAKE_CURRENT_SOURCE_DIR}/include/audioRender/ManualMix/ManualFausts
  ${CMAKE_CURRENT_SOURCE_DIR}/include/db/Capnp/CapnpBinary
  ${CMAKE_CURRENT_SOURCE_DIR}/include/db/Capnp/Translators
  ${CMAKE_CURRENT_SOURCE_DIR}/include/db/Capnp/Translators/MixTranslator
  ${CMAKE_CURRENT_SOURCE_DIR}/include/db/Capnp/Translators/MusicTranslator
  ${CMAKE_CURRENT_SOURCE_DIR}/include/db/Capnp/Translators/NoteTranslator

  ${CMAKE_CURRENT_SOURCE_DIR}/third_party/Capnp
	${miniaudio_SOURCE_DIR}/extras/miniaudio_split
  ${SQLite3_INCLUDE_DIR}
  ${highway_INCLUDE_DIR}

	${CMAKE_CURRENT_SOURCE_DIR}/third_party/SoundTouch/soundtouch_include
	${CMAKE_CURRENT_SOURCE_DIR}/third_party/Faust/compiled
  ${CMAKE_CURRENT_SOURCE_DIR}/third_party/Faust/manual_compiled
  

  ${CMAKE_CURRENT_SOURCE_DIR}/include/input
  ${CMAKE_CURRENT_SOURCE_DIR}/include/input/linux
  ${CMAKE_CURRENT_SOURCE_DIR}/include/input/macos
  
  ${CMAKE_CURRENT_SOURCE_DIR}/include/MainObjects
  ${CMAKE_CURRENT_SOURCE_DIR}/include/MainObjects/audioPlayer
  ${CMAKE_CURRENT_SOURCE_DIR}/include/MainObjects/editorObject
  ${CMAKE_CURRENT_SOURCE_DIR}/include/MainObjects/tempDBObject
  

  ${CMAKE_CURRENT_SOURCE_DIR}/include/interface



  ${CMAKE_CURRENT_SOURCE_DIR}/include/editor
  ${CMAKE_CURRENT_SOURCE_DIR}/include/editor/edit
  ${CMAKE_CURRENT_SOURCE_DIR}/include/editor/featureWrapper
  ${CMAKE_CURRENT_SOURCE_DIR}/include/editor/featureWrapper/GitWrap
  ${CMAKE_CURRENT_SOURCE_DIR}/include/editor/featureWrapper/GitWrap/Branch
  ${CMAKE_CURRENT_SOURCE_DIR}/include/editor/featureWrapper/GitWrap/Commit
  ${CMAKE_CURRENT_SOURCE_DIR}/include/editor/featureWrapper/GitWrap/Add
  ${CMAKE_CURRENT_SOURCE_DIR}/include/editor/featureWrapper/GitWrap/Blame
  ${CMAKE_CURRENT_SOURCE_DIR}/include/editor/featureWrapper/GitWrap/Diff
  ${CMAKE_CURRENT_SOURCE_DIR}/include/editor/featureWrapper/GitWrap/Log
  
  ${CMAKE_CURRENT_SOURCE_DIR}/include/editor/featureWrapper/JSONWrap
  ${CMAKE_CURRENT_SOURCE_DIR}/include/editor/objects
  ${CMAKE_CURRENT_SOURCE_DIR}/include/editor/renderer
  
  ${nlohmann_json_SOURCE_DIR}/include
  ${sql_amalgam_SOURCE_DIR}
  ${cppcodec_SOURCE_DIR}
  )