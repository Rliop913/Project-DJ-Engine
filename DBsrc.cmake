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