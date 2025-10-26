set(dbSource
${CMAKE_CURRENT_SOURCE_DIR}/include/core/db/musicDB.cpp
${CMAKE_CURRENT_SOURCE_DIR}/include/core/db/trackDB.cpp
${CMAKE_CURRENT_SOURCE_DIR}/include/core/db/dbRoot.cpp
${CMAKE_CURRENT_SOURCE_DIR}/include/core/db/dbState.cpp

${CMAKE_CURRENT_SOURCE_DIR}/include/core/db/Capnp/Translators/MixTranslator/Bpm.cpp
${CMAKE_CURRENT_SOURCE_DIR}/include/core/db/Capnp/Translators/MixTranslator/Mix.cpp
${CMAKE_CURRENT_SOURCE_DIR}/include/core/db/Capnp/Translators/MixTranslator/MixTranslator.cpp
${CMAKE_CURRENT_SOURCE_DIR}/include/core/db/Capnp/Translators/MusicTranslator/MusicTranslator.cpp
${CMAKE_CURRENT_SOURCE_DIR}/include/core/db/Capnp/Translators/NoteTranslator/NoteTranslator.cpp

${CMAKE_CURRENT_SOURCE_DIR}/include/core/db/Capnp/Translators/FrameCalc.cpp

${CAPNP_SRCS}
# ${CMAKE_CURRENT_SOURCE_DIR}/third_party/Capnp/MixBinary.capnp.c++
# ${CMAKE_CURRENT_SOURCE_DIR}/third_party/Capnp/MusicBinary.capnp.c++
# ${CMAKE_CURRENT_SOURCE_DIR}/third_party/Capnp/NoteBinary.capnp.c++
)