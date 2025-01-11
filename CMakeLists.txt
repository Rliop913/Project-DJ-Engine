set(cmake_cxx_standard 20)
set(cmake_cxx_flags -std=c++20)
set(cmake_c_compiler clang)
set(cmake_cxx_compiler clang++)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

cmake_minimum_required(VERSION 3.0)
project(
  PDJE
  VERSION 0.1
  LANGUAGES C CXX)

Include(FetchContent)

FetchContent_Declare(
  capnproto
  GIT_REPOSITORY https://github.com/capnproto/capnproto.git
  GIT_TAG v1.0.2
)

FetchContent_Declare(
  miniaudio
  GIT_REPOSITORY https://github.com/mackron/miniaudio.git
  GIT_TAG 0.11.21
)

FetchContent_Declare(
  sqlite
  GIT_REPOSITORY https://github.com/sqlite/sqlite.git
  GIT_TAG release
)

FetchContent_MakeAvailable(capnproto)
FetchContent_MakeAvailable(miniaudio)
FetchContent_MakeAvailable(sqlite)

link_libraries(sqlite3)

# set(engine_src
# ${CMAKE_SOURCE_DIR}/src/ALBUM.cpp
# ${CMAKE_SOURCE_DIR}/src/beat_compiler_extension.cpp
# ${CMAKE_SOURCE_DIR}/src/CURSOR.cpp
# ${CMAKE_SOURCE_DIR}/src/DeckWorker.cpp
# ${CMAKE_SOURCE_DIR}/src/Engine_Main.cpp
# ${CMAKE_SOURCE_DIR}/src/MiniAudioWrapper.cpp
# ${CMAKE_SOURCE_DIR}/src/Processor.cpp
# ${CMAKE_SOURCE_DIR}/src/sound_module.cpp
# ${CMAKE_SOURCE_DIR}/src/data_loader.cpp

# ${CMAKE_SOURCE_DIR}/src/dbsrc/dbRoot.cpp
# ${CMAKE_SOURCE_DIR}/include/db/musicDB.cpp


# )

set(miniaudio_src
${miniaudio_SOURCE_DIR}/extras/miniaudio_split/miniaudio.c
)

set(SoundTouch_src
${CMAKE_SOURCE_DIR}/third_party/SoundTouch/soundtouch_src/AAFilter.cpp
${CMAKE_SOURCE_DIR}/third_party/SoundTouch/soundtouch_src/BPMDetect.cpp
${CMAKE_SOURCE_DIR}/third_party/SoundTouch/soundtouch_src/cpu_detect_x86.cpp
${CMAKE_SOURCE_DIR}/third_party/SoundTouch/soundtouch_src/FIFOSampleBuffer.cpp
${CMAKE_SOURCE_DIR}/third_party/SoundTouch/soundtouch_src/FIRFilter.cpp
${CMAKE_SOURCE_DIR}/third_party/SoundTouch/soundtouch_src/InterpolateCubic.cpp
${CMAKE_SOURCE_DIR}/third_party/SoundTouch/soundtouch_src/InterpolateLinear.cpp
${CMAKE_SOURCE_DIR}/third_party/SoundTouch/soundtouch_src/InterpolateShannon.cpp
${CMAKE_SOURCE_DIR}/third_party/SoundTouch/soundtouch_src/mmx_optimized.cpp
${CMAKE_SOURCE_DIR}/third_party/SoundTouch/soundtouch_src/PeakFinder.cpp
${CMAKE_SOURCE_DIR}/third_party/SoundTouch/soundtouch_src/RateTransposer.cpp
${CMAKE_SOURCE_DIR}/third_party/SoundTouch/soundtouch_src/SoundTouch.cpp
${CMAKE_SOURCE_DIR}/third_party/SoundTouch/soundtouch_src/sse_optimized.cpp
${CMAKE_SOURCE_DIR}/third_party/SoundTouch/soundtouch_src/TDStretch.cpp

)

include_directories(
  ${CMAKE_SOURCE_DIR}/include
	# ${CMAKE_SOURCE_DIR}/include/core_header
	# ${CMAKE_SOURCE_DIR}/include/data_structs
	# ${CMAKE_SOURCE_DIR}/include/wrapper
  ${CMAKE_SOURCE_DIR}/include/db
  ${CMAKE_SOURCE_DIR}/include/audioRender/MixBinary
  ${CMAKE_SOURCE_DIR}/include/audioRender/MixTranslator
  
  ${CMAKE_SOURCE_DIR}/third_party/Capnp
  ${CMAKE_SOURCE_DIR}/include/ERR
	${miniaudio_SOURCE_DIR}/extras/miniaudio_split
  ${sqlite_INCLUDE_DIR}
  

	${CMAKE_SOURCE_DIR}/third_party/SoundTouch/soundtouch_include
	${CMAKE_SOURCE_DIR}/third_party/Faust/compiled
  
)

# set(errorPdje
# ${CMAKE_SOURCE_DIR}/include/ERR/errorTable.cpp
# )

set(dbSource
${CMAKE_SOURCE_DIR}/include/db/musicDB.cpp
${CMAKE_SOURCE_DIR}/include/db/trackDB.cpp
${CMAKE_SOURCE_DIR}/include/db/dbRoot.cpp
${CMAKE_SOURCE_DIR}/include/db/dbState.cpp
)

set(audioRenderSource
# ${CMAKE_SOURCE_DIR}/include/audioRender/MiniAudioWrapper.cpp
${CMAKE_SOURCE_DIR}/third_party/Capnp/MixBinary.capnp.c++
${CMAKE_SOURCE_DIR}/include/audioRender/MixBinary/MixBinary_Read.cpp
${CMAKE_SOURCE_DIR}/include/audioRender/MixBinary/MixBinary_Write.cpp
${CMAKE_SOURCE_DIR}/include/audioRender/MixTranslator/Bpm.cpp
${CMAKE_SOURCE_DIR}/include/audioRender/MixTranslator/Mix.cpp
${CMAKE_SOURCE_DIR}/include/audioRender/MixTranslator/MixTranslator.cpp
${CMAKE_SOURCE_DIR}/include/audioRender/MixTranslator/Structs.cpp
)

# add_executable(${PROJECT_NAME} ${engine_src} ${SoundTouch_src})

# target_include_directories(
# 	${PROJECT_NAME} PRIVATE
# 	${CMAKE_SOURCE_DIR}/include
# 	${CMAKE_SOURCE_DIR}/include/core_header
# 	${CMAKE_SOURCE_DIR}/include/data_structs
# 	${CMAKE_SOURCE_DIR}/include/wrapper
#   ${CMAKE_SOURCE_DIR}/include/db
  
# 	${CMAKE_SOURCE_DIR}/miniaudio
# 	${CMAKE_SOURCE_DIR}/third_party/SoundTouch/soundtouch_include
# 	${CMAKE_SOURCE_DIR}/third_party/Faust/compiled
# 	${miniaudio_SOURCE_DIR}/extras/miniaudio_split
# )


add_executable(testdb ${CMAKE_SOURCE_DIR}/include/tests/dbTest.cpp ${dbSource})
add_executable(testaudio ${CMAKE_SOURCE_DIR}/include/tests/audioTest.cpp ${audioRenderSource})
target_link_libraries(testaudio PRIVATE capnp capnp-rpc kj)
enable_testing()

# target_include_directories(testdb
#   PRIVATE
#   ${CMAKE_SOURCE_DIR}/include
# 	${CMAKE_SOURCE_DIR}/include/core_header
# 	${CMAKE_SOURCE_DIR}/include/data_structs
# 	${CMAKE_SOURCE_DIR}/include/wrapper
#   ${CMAKE_SOURCE_DIR}/include/db
  
# 	${CMAKE_SOURCE_DIR}/miniaudio
# 	${CMAKE_SOURCE_DIR}/third_party/SoundTouch/soundtouch_include
# 	${CMAKE_SOURCE_DIR}/third_party/Faust/compiled
# 	${miniaudio_SOURCE_DIR}/extras/miniaudio_split
#   ${sqlite_INCLUDE_DIR}
# )



add_test(NAME DB_TEST COMMAND testdb)
add_test(NAME AUDIO_TEST COMMAND testaudio)