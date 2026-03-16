
add_library(MINIAUDIO_OBJ OBJECT
${miniaudio_SOURCE_DIR}/extras/miniaudio_split/miniaudio.c
)
add_library(MINIAUDIO_INCLUDE INTERFACE)
target_include_directories(MINIAUDIO_INCLUDE INTERFACE 
${miniaudio_SOURCE_DIR}/extras/miniaudio_split
)
target_link_libraries(MINIAUDIO_OBJ PRIVATE MINIAUDIO_INCLUDE)


add_library(SOUNDTOUCH_OBJ OBJECT
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

add_library(SOUNDTOUCH_INCLUDE INTERFACE)
target_include_directories(SOUNDTOUCH_INCLUDE INTERFACE
    ${PDJE_INCLUDE_ROOT}/third_party/SoundTouch/soundtouch_include
)
target_link_libraries(SOUNDTOUCH_OBJ PRIVATE SOUNDTOUCH_INCLUDE)


add_library(SQL_OBJ OBJECT
${sql_amalgam_SOURCE_DIR}/sqlite3.c
)
add_library(SQL_INCLUDE INTERFACE)
target_include_directories(SQL_INCLUDE INTERFACE
${SQLite3_INCLUDE_DIR}
)
target_link_libraries(SQL_OBJ PRIVATE SQL_INCLUDE)