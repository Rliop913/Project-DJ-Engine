cmake_minimum_required(VERSION 3.0)
project("DJ_Engine" )

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_VERBOSE_MAKEFILE true)
set(cmake_cxx_standard 20)
set(cmake_cxx_flags -std=c++20)
set(cmake_c_compiler clang)
set(cmake_cxx_compiler clang++)
#add_subdirectory(${CMAKE_SOURCE_DIR}/soundtouch-master/soundtouch)




set(engine_src
${CMAKE_SOURCE_DIR}/engine_src/ALBUM.cpp
${CMAKE_SOURCE_DIR}/engine_src/beat_compiler_extension.cpp
${CMAKE_SOURCE_DIR}/engine_src/CURSOR.cpp
${CMAKE_SOURCE_DIR}/engine_src/DeckWorker.cpp
${CMAKE_SOURCE_DIR}/engine_src/Engine_Main.cpp
${CMAKE_SOURCE_DIR}/engine_src/MiniAudioWrapper.cpp
${CMAKE_SOURCE_DIR}/engine_src/Processor.cpp
${CMAKE_SOURCE_DIR}/engine_src/sound_module.cpp
${CMAKE_SOURCE_DIR}/miniaudio/miniaudio.c
)

set(SoundTouch_src
${CMAKE_SOURCE_DIR}/soundtouch_src/AAFilter.cpp
${CMAKE_SOURCE_DIR}/soundtouch_src/BPMDetect.cpp
${CMAKE_SOURCE_DIR}/soundtouch_src/cpu_detect_x86.cpp
${CMAKE_SOURCE_DIR}/soundtouch_src/FIFOSampleBuffer.cpp
${CMAKE_SOURCE_DIR}/soundtouch_src/FIRFilter.cpp
${CMAKE_SOURCE_DIR}/soundtouch_src/InterpolateCubic.cpp
${CMAKE_SOURCE_DIR}/soundtouch_src/InterpolateLinear.cpp
${CMAKE_SOURCE_DIR}/soundtouch_src/InterpolateShannon.cpp
${CMAKE_SOURCE_DIR}/soundtouch_src/mmx_optimized.cpp
${CMAKE_SOURCE_DIR}/soundtouch_src/PeakFinder.cpp
${CMAKE_SOURCE_DIR}/soundtouch_src/RateTransposer.cpp
${CMAKE_SOURCE_DIR}/soundtouch_src/SoundTouch.cpp
${CMAKE_SOURCE_DIR}/soundtouch_src/sse_optimized.cpp
${CMAKE_SOURCE_DIR}/soundtouch_src/TDStretch.cpp
)


add_executable(${PROJECT_NAME} ${engine_src} ${SoundTouch_src})

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

target_include_directories(
	${PROJECT_NAME} PUBLIC
	${CMAKE_SOURCE_DIR}/engine_include
	${CMAKE_SOURCE_DIR}/engine_include/core_header
	${CMAKE_SOURCE_DIR}/engine_include/data_structs
	${CMAKE_SOURCE_DIR}/engine_include/Faust_code
	${CMAKE_SOURCE_DIR}/engine_include/wrapper
	${CMAKE_SOURCE_DIR}/miniaudio
	${CMAKE_SOURCE_DIR}/soundtouch_include

)
	#${CMAKE_SOURCE_DIR}/soundtouch_include/

add_compile_options(-O3)
#target_link_libraries(${PROJECT_NAME} SoundTouch_x64)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE})
