
set(audioRenderSource

${CMAKE_CURRENT_SOURCE_DIR}/include/audioRender/MixMachine/MixMachine.cpp
${CMAKE_CURRENT_SOURCE_DIR}/include/audioRender/MixMachine/MiniaudioObjects/Decoder.cpp
${CMAKE_CURRENT_SOURCE_DIR}/include/audioRender/MixMachine/MUSIC_CTR/MUSIC_CTR.cpp
${CMAKE_CURRENT_SOURCE_DIR}/include/audioRender/MixMachine/MUSIC_CTR/SetCue.cpp
${CMAKE_CURRENT_SOURCE_DIR}/include/audioRender/MixMachine/EFFECT/EFFECTS.cpp

${CMAKE_CURRENT_SOURCE_DIR}/include/audioRender/audioRender.cpp
${CMAKE_CURRENT_SOURCE_DIR}/include/audioRender/MixMachine/MUSIC_CTR/BattleDj.cpp
${CMAKE_CURRENT_SOURCE_DIR}/include/audioRender/MixMachine/MixTypes/Type_MUSCTR.cpp
${CMAKE_CURRENT_SOURCE_DIR}/include/audioRender/MixMachine/MixTypes/Type_FX_COMPRESSOR.cpp
${CMAKE_CURRENT_SOURCE_DIR}/include/audioRender/MixMachine/MixTypes/Type_FX_EQ.cpp

${CMAKE_CURRENT_SOURCE_DIR}/include/audioRender/MixMachine/MixTypes/Type_FX_ECHO.cpp
${CMAKE_CURRENT_SOURCE_DIR}/include/audioRender/MixMachine/MixTypes/Type_FX_FILTER.cpp
${CMAKE_CURRENT_SOURCE_DIR}/include/audioRender/MixMachine/MixTypes/Type_FX_OCS_FILTER.cpp
${CMAKE_CURRENT_SOURCE_DIR}/include/audioRender/MixMachine/MixTypes/Type_FX_PHASER.cpp
${CMAKE_CURRENT_SOURCE_DIR}/include/audioRender/MixMachine/MixTypes/Type_FX_TRANCE.cpp
${CMAKE_CURRENT_SOURCE_DIR}/include/audioRender/MixMachine/MixTypes/Type_FX_DISTORTION.cpp
${CMAKE_CURRENT_SOURCE_DIR}/include/audioRender/MixMachine/MixTypes/Type_FX_FLANGER.cpp
${CMAKE_CURRENT_SOURCE_DIR}/include/audioRender/MixMachine/MixTypes/Type_FX_PANNER.cpp
${CMAKE_CURRENT_SOURCE_DIR}/include/audioRender/MixMachine/MixTypes/Type_FX_ROLL.cpp
${CMAKE_CURRENT_SOURCE_DIR}/include/audioRender/MixMachine/MixTypes/Type_FX_ROBOT.cpp
${CMAKE_CURRENT_SOURCE_DIR}/include/audioRender/MixMachine/MixTypes/Type_VOL.cpp

${CMAKE_CURRENT_SOURCE_DIR}/include/audioRender/ManualMix/ManualMix.cpp
${CMAKE_CURRENT_SOURCE_DIR}/include/audioRender/ManualMix/MusicControlPannel.cpp
${CMAKE_CURRENT_SOURCE_DIR}/include/MainObjects/audioPlayer/audioPlayer.cpp
${CMAKE_CURRENT_SOURCE_DIR}/include/MainObjects/audioPlayer/audioCallbacks.cpp

${CMAKE_CURRENT_SOURCE_DIR}/include/interface/PDJE_interface.cpp
${CMAKE_CURRENT_SOURCE_DIR}/include/interface/PDJE_polyglot_wraps.cpp

)


set(INCLUDE_LIST
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
  ${CMAKE_CURRENT_SOURCE_DIR}/include/editor/renderer)