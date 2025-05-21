
set(audioRenderSource

${CMAKE_SOURCE_DIR}/include/audioRender/MixMachine/MixMachine.cpp
${CMAKE_SOURCE_DIR}/include/audioRender/MixMachine/MiniaudioObjects/Decoder.cpp
${CMAKE_SOURCE_DIR}/include/audioRender/MixMachine/MUSIC_CTR/MUSIC_CTR.cpp
${CMAKE_SOURCE_DIR}/include/audioRender/MixMachine/MUSIC_CTR/SetCue.cpp
${CMAKE_SOURCE_DIR}/include/audioRender/MixMachine/EFFECT/EFFECTS.cpp

${CMAKE_SOURCE_DIR}/include/audioRender/audioRender.cpp
${CMAKE_SOURCE_DIR}/include/audioRender/MixMachine/MUSIC_CTR/BattleDj.cpp
${CMAKE_SOURCE_DIR}/include/audioRender/MixMachine/MixTypes/Type_MUSCTR.cpp
${CMAKE_SOURCE_DIR}/include/audioRender/MixMachine/MixTypes/Type_FX_COMPRESSOR.cpp
${CMAKE_SOURCE_DIR}/include/audioRender/MixMachine/MixTypes/Type_FX_EQ.cpp

${CMAKE_SOURCE_DIR}/include/audioRender/MixMachine/MixTypes/Type_FX_ECHO.cpp
${CMAKE_SOURCE_DIR}/include/audioRender/MixMachine/MixTypes/Type_FX_FILTER.cpp
${CMAKE_SOURCE_DIR}/include/audioRender/MixMachine/MixTypes/Type_FX_OCS_FILTER.cpp
${CMAKE_SOURCE_DIR}/include/audioRender/MixMachine/MixTypes/Type_FX_PHASER.cpp
${CMAKE_SOURCE_DIR}/include/audioRender/MixMachine/MixTypes/Type_FX_TRANCE.cpp
${CMAKE_SOURCE_DIR}/include/audioRender/MixMachine/MixTypes/Type_FX_DISTORTION.cpp
${CMAKE_SOURCE_DIR}/include/audioRender/MixMachine/MixTypes/Type_FX_FLANGER.cpp
${CMAKE_SOURCE_DIR}/include/audioRender/MixMachine/MixTypes/Type_FX_PANNER.cpp
${CMAKE_SOURCE_DIR}/include/audioRender/MixMachine/MixTypes/Type_FX_ROLL.cpp
${CMAKE_SOURCE_DIR}/include/audioRender/MixMachine/MixTypes/Type_FX_ROBOT.cpp
${CMAKE_SOURCE_DIR}/include/audioRender/MixMachine/MixTypes/Type_VOL.cpp

${CMAKE_SOURCE_DIR}/include/audioRender/ManualMix/ManualMix.cpp
${CMAKE_SOURCE_DIR}/include/audioRender/ManualMix/MusicControlPannel.cpp
${CMAKE_SOURCE_DIR}/include/MainObjects/audioPlayer/audioPlayer.cpp
${CMAKE_SOURCE_DIR}/include/MainObjects/audioPlayer/audioCallbacks.cpp

${CMAKE_SOURCE_DIR}/include/interface/PDJE_interface.cpp
${CMAKE_SOURCE_DIR}/include/interface/PDJE_polyglot_wraps.cpp

)
