

add_executable(
pdje_unit_util
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/main_doctest.cpp
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/util/public_headers.test.cpp
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/util/png_writer.test.cpp
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/util/waveform_png.test.cpp
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/util/db_relational_sqlite.test.cpp
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/util/db_keyvalue_rocksdb.test.cpp
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/util/db_nearest_annoy.test.cpp
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/util/status.test.cpp
)


target_include_directories(pdje_unit_util PRIVATE ${PDJE_INCLUDE_GLOBAL} ${PDJE_INCLUDE_CORE})
target_link_libraries(pdje_unit_util PRIVATE doctest::doctest)
target_link_libraries(pdje_unit_util PRIVATE PDJE_UTIL)
target_link_libraries(pdje_unit_util PRIVATE PDJE_UTIL_IMAGE_PNG)
target_link_libraries(pdje_unit_util PRIVATE PDJE_UTIL_IMAGE_WAVEFORM)
target_link_libraries(pdje_unit_util PRIVATE PDJE_UTIL_DB)
target_link_libraries(pdje_unit_util PRIVATE PDJE)

target_compile_definitions(pdje_unit_util PRIVATE PDJE_UNIT_TESTING)

AddDynamicDef(pdje_unit_util)

if(WIN32)
set(_pdje_unit_util_zlib_runtime
    "$<$<CONFIG:Debug>:${zlib_BIN_DIRS_DEBUG}/zlib1.dll>$<$<CONFIG:Release>:${zlib_BIN_DIRS_RELEASE}/zlib1.dll>$<$<CONFIG:RelWithDebInfo>:${zlib_BIN_DIRS_RELEASE}/zlib1.dll>$<$<CONFIG:MinSizeRel>:${zlib_BIN_DIRS_RELEASE}/zlib1.dll>")
add_custom_command(
    TARGET pdje_unit_util
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${_pdje_unit_util_zlib_runtime}"
    $<TARGET_FILE_DIR:pdje_unit_util>
    VERBATIM)
endif()
setSpdlogReqLib(pdje_unit_util)
setPdjeLogRuntimeReqLib(pdje_unit_util)
setCapnpReqLib(pdje_unit_util)
setLibgit2ReqLib(pdje_unit_util)
PDJE_COMPILE_OPTION(pdje_unit_util)
SET_PROPERTIES(pdje_unit_util)
pdje_discover_unit_tests(pdje_unit_util core)