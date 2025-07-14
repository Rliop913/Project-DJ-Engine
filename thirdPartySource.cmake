
set(miniaudio_src
${miniaudio_SOURCE_DIR}/extras/miniaudio_split/miniaudio.c
)

set(SoundTouch_src
${CMAKE_CURRENT_SOURCE_DIR}/third_party/SoundTouch/soundtouch_src/AAFilter.cpp
${CMAKE_CURRENT_SOURCE_DIR}/third_party/SoundTouch/soundtouch_src/BPMDetect.cpp
${CMAKE_CURRENT_SOURCE_DIR}/third_party/SoundTouch/soundtouch_src/cpu_detect_x86.cpp
${CMAKE_CURRENT_SOURCE_DIR}/third_party/SoundTouch/soundtouch_src/FIFOSampleBuffer.cpp
${CMAKE_CURRENT_SOURCE_DIR}/third_party/SoundTouch/soundtouch_src/FIRFilter.cpp
${CMAKE_CURRENT_SOURCE_DIR}/third_party/SoundTouch/soundtouch_src/InterpolateCubic.cpp
${CMAKE_CURRENT_SOURCE_DIR}/third_party/SoundTouch/soundtouch_src/InterpolateLinear.cpp
${CMAKE_CURRENT_SOURCE_DIR}/third_party/SoundTouch/soundtouch_src/InterpolateShannon.cpp
${CMAKE_CURRENT_SOURCE_DIR}/third_party/SoundTouch/soundtouch_src/mmx_optimized.cpp
${CMAKE_CURRENT_SOURCE_DIR}/third_party/SoundTouch/soundtouch_src/PeakFinder.cpp
${CMAKE_CURRENT_SOURCE_DIR}/third_party/SoundTouch/soundtouch_src/RateTransposer.cpp
${CMAKE_CURRENT_SOURCE_DIR}/third_party/SoundTouch/soundtouch_src/SoundTouch.cpp
${CMAKE_CURRENT_SOURCE_DIR}/third_party/SoundTouch/soundtouch_src/sse_optimized.cpp
${CMAKE_CURRENT_SOURCE_DIR}/third_party/SoundTouch/soundtouch_src/TDStretch.cpp

)

set(sql_amalgam_src
${sql_amalgam_SOURCE_DIR}/sqlite3.c
)

set(nanolog_src
${nanolog_SOURCE_DIR}/NanoLog.cpp
)