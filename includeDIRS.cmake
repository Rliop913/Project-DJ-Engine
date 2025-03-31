
include_directories(
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/include/db
  ${CMAKE_SOURCE_DIR}/include/audioRender
  ${CMAKE_SOURCE_DIR}/include/audioRender/MixMachine
  ${CMAKE_SOURCE_DIR}/include/audioRender/MixMachine/EFFECT
  ${CMAKE_SOURCE_DIR}/include/audioRender/MixMachine/FAUST_VALS
  
  ${CMAKE_SOURCE_DIR}/include/audioRender/MixMachine/MUSIC_CTR
  
  ${CMAKE_SOURCE_DIR}/include/audioRender/MixMachine/MiniaudioObjects
  
  ${CMAKE_SOURCE_DIR}/include/audioRender/MixMachine/INTERPOLATOR

  ${CMAKE_SOURCE_DIR}/include/audioRender/ManualMix
  ${CMAKE_SOURCE_DIR}/include/audioRender/ManualMix/ManualFausts
  
  ${CMAKE_SOURCE_DIR}/include/db/Capnp/CapnpBinary
  ${CMAKE_SOURCE_DIR}/include/db/Capnp/Translators
  ${CMAKE_SOURCE_DIR}/include/db/Capnp/Translators/MixTranslator
  ${CMAKE_SOURCE_DIR}/include/db/Capnp/Translators/MusicTranslator
  ${CMAKE_SOURCE_DIR}/include/db/Capnp/Translators/NoteTranslator

  ${CMAKE_SOURCE_DIR}/third_party/Capnp
  ${CMAKE_SOURCE_DIR}/include/ERR
	${miniaudio_SOURCE_DIR}/extras/miniaudio_split
  ${SQLite3_INCLUDE_DIR}
  ${highway_INCLUDE_DIR}

	${CMAKE_SOURCE_DIR}/third_party/SoundTouch/soundtouch_include
	${CMAKE_SOURCE_DIR}/third_party/Faust/compiled
  ${CMAKE_SOURCE_DIR}/third_party/Faust/manual_compiled
  

  ${CMAKE_SOURCE_DIR}/include/input
  ${CMAKE_SOURCE_DIR}/include/input/linux
  ${CMAKE_SOURCE_DIR}/include/input/macos
  
  ${CMAKE_SOURCE_DIR}/include/players
  ${CMAKE_SOURCE_DIR}/include/players/audioPlayer

  ${CMAKE_SOURCE_DIR}/include/interface

)