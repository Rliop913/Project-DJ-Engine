set(cmake_cxx_standard 20)
set(cmake_cxx_flags -std=c++20)
set(cmake_c_compiler clang)
set(cmake_cxx_compiler clang++)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g -O1 -fno-omit-frame-pointer")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -g -O1 -fno-omit-frame-pointer")
set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fsanitize=address")

set_property(SOURCE PDJE_swig.i PROPERTY CPLUSPLUS ON)

cmake_minimum_required(VERSION 3.0)
project(
  PDJE
  VERSION 0.1
  LANGUAGES C CXX)

Include(FetchContent)

find_package(SWIG REQUIRED)
include(UseSWIG)


FetchContent_Declare(
  capnproto
  GIT_REPOSITORY https://github.com/capnproto/capnproto.git
  GIT_TAG v1.1.0
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

FetchContent_Declare(
  highway
  GIT_REPOSITORY https://github.com/google/highway.git
  GIT_TAG 1.2.0
)

FetchContent_MakeAvailable(capnproto)
FetchContent_MakeAvailable(miniaudio)
FetchContent_MakeAvailable(sqlite)
FetchContent_MakeAvailable(highway)

link_libraries(sqlite3)


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
  ${CMAKE_SOURCE_DIR}/include/db
  ${CMAKE_SOURCE_DIR}/include/audioRender
  ${CMAKE_SOURCE_DIR}/include/audioRender/MixMachine
  ${CMAKE_SOURCE_DIR}/include/audioRender/MixMachine/EFFECT
  ${CMAKE_SOURCE_DIR}/include/audioRender/MixMachine/FAUST_VALS
  
  ${CMAKE_SOURCE_DIR}/include/audioRender/MixMachine/MUSIC_CTR
  
  ${CMAKE_SOURCE_DIR}/include/audioRender/MixMachine/MiniaudioObjects
  
  ${CMAKE_SOURCE_DIR}/include/audioRender/MixMachine/INTERPOLATOR

  ${CMAKE_SOURCE_DIR}/include/db/Capnp/CapnpBinary
  ${CMAKE_SOURCE_DIR}/include/db/Capnp/Translators
  ${CMAKE_SOURCE_DIR}/include/db/Capnp/Translators/MixTranslator
  ${CMAKE_SOURCE_DIR}/include/db/Capnp/Translators/MusicTranslator
  ${CMAKE_SOURCE_DIR}/include/db/Capnp/Translators/NoteTranslator

  ${CMAKE_SOURCE_DIR}/third_party/Capnp
  ${CMAKE_SOURCE_DIR}/include/ERR
	${miniaudio_SOURCE_DIR}/extras/miniaudio_split
  ${sqlite_INCLUDE_DIR}
  ${highway_INCLUDE_DIR}

	${CMAKE_SOURCE_DIR}/third_party/SoundTouch/soundtouch_include
	${CMAKE_SOURCE_DIR}/third_party/Faust/compiled
  
)


set(dbSource
${CMAKE_SOURCE_DIR}/include/db/musicDB.cpp
${CMAKE_SOURCE_DIR}/include/db/trackDB.cpp
${CMAKE_SOURCE_DIR}/include/db/dbRoot.cpp
${CMAKE_SOURCE_DIR}/include/db/dbState.cpp

${CMAKE_SOURCE_DIR}/include/db/Capnp/Translators/MixTranslator/Bpm.cpp
${CMAKE_SOURCE_DIR}/include/db/Capnp/Translators/MixTranslator/Mix.cpp
${CMAKE_SOURCE_DIR}/include/db/Capnp/Translators/MixTranslator/MixTranslator.cpp
${CMAKE_SOURCE_DIR}/include/db/Capnp/Translators/MusicTranslator/MusicTranslator.cpp
${CMAKE_SOURCE_DIR}/include/db/Capnp/Translators/NoteTranslator/NoteTranslator.cpp

${CMAKE_SOURCE_DIR}/include/db/Capnp/Translators/FrameCalc.cpp

${CMAKE_SOURCE_DIR}/third_party/Capnp/MixBinary.capnp.c++
${CMAKE_SOURCE_DIR}/third_party/Capnp/MusicBinary.capnp.c++
${CMAKE_SOURCE_DIR}/third_party/Capnp/NoteBinary.capnp.c++
)

set(audioRenderSource
# ${CMAKE_SOURCE_DIR}/include/audioRender/MiniAudioWrapper.cpp



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




${CMAKE_SOURCE_DIR}/include/audioRender/MixMachine/THREADS/RenderThreads.cpp


)



add_executable(testdb ${CMAKE_SOURCE_DIR}/include/tests/dbTest.cpp ${dbSource})
add_executable(testaudio ${CMAKE_SOURCE_DIR}/include/tests/audioTest.cpp ${miniaudio_src} ${SoundTouch_src} ${dbSource} ${audioRenderSource})
add_executable(testCapnp ${CMAKE_SOURCE_DIR}/include/tests/CapnpTest.cpp ${miniaudio_src} ${SoundTouch_src} ${dbSource} ${audioRenderSource})

# find_package(HWY REQUIRED)
add_executable(testHighway include/tests/highway_Test.cpp)

# target_link_libraries(testHighway PRIVATE hwy)

swig_add_library(pdje_csharp
  TYPE STATIC
  LANGUAGE CSharp
  OUTPUT_DIR ${CMAKE_SOURCE_DIR}/swig_csharp
  SOURCES PDJE_swig.i ${miniaudio_src} ${SoundTouch_src} ${dbSource} ${audioRenderSource}
)
find_package(Python REQUIRED COMPONENTS Interpreter Development)

swig_add_library(pdje_python
  TYPE STATIC
  LANGUAGE Python
  OUTPUT_DIR ${CMAKE_SOURCE_DIR}/swig_python
  SOURCES PDJE_swig.i ${miniaudio_src} ${SoundTouch_src} ${dbSource} ${audioRenderSource}
)

swig_add_library(pdje_go
  TYPE STATIC
  LANGUAGE go
  OUTPUT_DIR ${CMAKE_SOURCE_DIR}/swig_go
  SOURCES PDJE_swig.i ${miniaudio_src} ${SoundTouch_src} ${dbSource} ${audioRenderSource}
)

target_link_libraries(testaudio PRIVATE capnp capnp-rpc kj)
target_link_libraries(testdb PRIVATE capnp capnp-rpc kj)
target_link_libraries(testCapnp PRIVATE capnp capnp-rpc kj)

target_link_libraries(${SWIG_MODULE_pdje_csharp_REAL_NAME} PRIVATE capnp capnp-rpc kj)
target_link_libraries(${SWIG_MODULE_pdje_python_REAL_NAME} PRIVATE capnp capnp-rpc kj)
target_link_libraries(${SWIG_MODULE_pdje_go_REAL_NAME} PRIVATE capnp capnp-rpc kj)

target_link_libraries(${SWIG_MODULE_pdje_python_REAL_NAME} PRIVATE Python::Python)

enable_testing()



add_test(NAME DB_TEST COMMAND testdb)
add_test(NAME AUDIO_TEST COMMAND testaudio)