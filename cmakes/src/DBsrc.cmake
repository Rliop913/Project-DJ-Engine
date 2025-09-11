set(dbSource
${CMAKE_CURRENT_SOURCE_DIR}/core_include/db/musicDB.cpp
${CMAKE_CURRENT_SOURCE_DIR}/core_include/db/trackDB.cpp
${CMAKE_CURRENT_SOURCE_DIR}/core_include/db/dbRoot.cpp
${CMAKE_CURRENT_SOURCE_DIR}/core_include/db/dbState.cpp

${CMAKE_CURRENT_SOURCE_DIR}/core_include/db/Capnp/Translators/MixTranslator/Bpm.cpp
${CMAKE_CURRENT_SOURCE_DIR}/core_include/db/Capnp/Translators/MixTranslator/Mix.cpp
${CMAKE_CURRENT_SOURCE_DIR}/core_include/db/Capnp/Translators/MixTranslator/MixTranslator.cpp
${CMAKE_CURRENT_SOURCE_DIR}/core_include/db/Capnp/Translators/MusicTranslator/MusicTranslator.cpp
${CMAKE_CURRENT_SOURCE_DIR}/core_include/db/Capnp/Translators/NoteTranslator/NoteTranslator.cpp

${CMAKE_CURRENT_SOURCE_DIR}/core_include/db/Capnp/Translators/FrameCalc.cpp
${CMAKE_CURRENT_SOURCE_DIR}/core_include/db/fileNameSanitizer.cpp

${CMAKE_CURRENT_SOURCE_DIR}/third_party/Capnp/MixBinary.capnp.c++
${CMAKE_CURRENT_SOURCE_DIR}/third_party/Capnp/MusicBinary.capnp.c++
${CMAKE_CURRENT_SOURCE_DIR}/third_party/Capnp/NoteBinary.capnp.c++
)