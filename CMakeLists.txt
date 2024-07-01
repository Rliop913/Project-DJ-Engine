cmake_minimum_required(VERSION 3.0)
project("DJ_Engine" )

Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.4.0 # or a later release
)

FetchContent_MakeAvailable(Catch2)
enable_testing()


FetchContent_Declare(
	protobuf
	GIT_REPOSITORY https://github.com/protocolbuffers/protobuf.git
	GIT_TAG v27.0-rc3 
	)
	set(protobuf_BUILD_TESTS OFF)
	set(protobuf_INSTALL ON)
	set(protobuf_BUILD_EXAMPLES OFF)
	FetchContent_MakeAvailable(protobuf)
	#set(CMAKE_BUILD_TYPE Debug)
	set(CMAKE_VERBOSE_MAKEFILE true)
	set(cmake_cxx_standard 20)
	set(cmake_cxx_flags -std=c++20)
	set(cmake_c_compiler clang)
	set(cmake_cxx_compiler clang++)
	#add_subdirectory(${CMAKE_SOURCE_DIR}/soundtouch-master/soundtouch)
	set(Protobuf_INCLUDE_DIR ${protobuf_SOURCE_DIR}/src)
	set(Absl_INCLUDE_DIR ${protobuf_SOURCE_DIR}/third_party/abseil-cpp)
	if(WIN32)
	set(Protobuf_LIBRARIES ${protobuf_BINARY_DIR}/src/libprotobuf.lib)
	elseif(UNIX)
		add_definitions(-DFOR_LINUX_BUILD)
		set(Protobuf_LIBRARIES ${protobuf_BINARY_DIR}/src/libprotobuf.a)
	endif()


	include_directories(${Protobuf_INCLUDE_DIR} ${Absl_INCLUDE_DIR})
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
${CMAKE_SOURCE_DIR}/engine_src/data_loader.cpp
${CMAKE_SOURCE_DIR}/engine_src/PDJS.pb.cc


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

set(test_srcs
${CMAKE_SOURCE_DIR}/miniaudio/miniaudio.c
${CMAKE_SOURCE_DIR}/engine_src/data_loader.cpp
${CMAKE_SOURCE_DIR}/tests/t1.cpp
${CMAKE_SOURCE_DIR}/engine_src/PDJS.pb.cc
)

set(test_main_srcs
${CMAKE_SOURCE_DIR}/miniaudio/miniaudio.c
${CMAKE_SOURCE_DIR}/engine_src/data_loader.cpp
${CMAKE_SOURCE_DIR}/tests/tmain.cpp
${CMAKE_SOURCE_DIR}/engine_src/PDJS.pb.cc
)

add_executable(${PROJECT_NAME} ${engine_src} ${SoundTouch_src} ${PROTO_HDRS})
add_executable(data_test ${test_srcs})
add_executable(test_main ${test_main_srcs} ${PROTO_HDRS})



target_link_libraries(${PROJECT_NAME} ${Protobuf_LIBRARIES})
target_link_libraries(test_main ${Protobuf_LIBRARIES})

target_link_libraries(data_test PUBLIC Catch2::Catch2WithMain)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

target_include_directories(
	${PROJECT_NAME} PRIVATE
	${CMAKE_SOURCE_DIR}/engine_include
	${CMAKE_SOURCE_DIR}/engine_include/core_header
	${CMAKE_SOURCE_DIR}/engine_include/data_structs
	${CMAKE_SOURCE_DIR}/engine_include/Faust_code
	${CMAKE_SOURCE_DIR}/engine_include/wrapper
	${CMAKE_SOURCE_DIR}/miniaudio
	${CMAKE_SOURCE_DIR}/soundtouch_include
)
target_include_directories(
	data_test PRIVATE
	${CMAKE_SOURCE_DIR}/engine_include
	${CMAKE_SOURCE_DIR}/engine_include/core_header
	${CMAKE_SOURCE_DIR}/engine_include/data_structs
	${CMAKE_SOURCE_DIR}/engine_include/Faust_code
	${CMAKE_SOURCE_DIR}/engine_include/wrapper
	${CMAKE_SOURCE_DIR}/miniaudio
	${CMAKE_SOURCE_DIR}/soundtouch_include
)

target_include_directories(
	test_main PRIVATE
	${CMAKE_SOURCE_DIR}/engine_include
	${CMAKE_SOURCE_DIR}/engine_include/core_header
	${CMAKE_SOURCE_DIR}/engine_include/data_structs
	${CMAKE_SOURCE_DIR}/engine_include/Faust_code
	${CMAKE_SOURCE_DIR}/engine_include/wrapper
	${CMAKE_SOURCE_DIR}/miniaudio
	${CMAKE_SOURCE_DIR}/soundtouch_include
)
	#${CMAKE_SOURCE_DIR}/soundtouch_include/
include(CTest)
include(Catch)
catch_discover_tests(data_test)
add_compile_options(-O3)
#target_link_libraries(${PROJECT_NAME} SoundTouch_x64)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE})
