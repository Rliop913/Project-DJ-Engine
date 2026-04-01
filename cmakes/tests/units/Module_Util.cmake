
if(APPLE)
return()
endif()


add_executable(
  pdje_unit_util
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/main_doctest.cpp
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/util/public_headers.test.cpp
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/util/webp_writer.test.cpp
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/util/waveform_webp.test.cpp
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/util/stft_parallel.test.cpp
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/util/opencl_clhpp_probe.test.cpp
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/util/opencl_runtime_shim.test.cpp
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/util/db_relational_sqlite.test.cpp
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/util/db_keyvalue_rocksdb.test.cpp
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/util/db_nearest_annoy.test.cpp
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/util/status.test.cpp
# ${CORE_SRC_EXPORT}
# ${IPC_SRC_EXPORT}
)


target_include_directories(pdje_unit_util PRIVATE ${PDJE_INCLUDE_GLOBAL} ${PDJE_INCLUDE_CORE})
target_link_libraries(pdje_unit_util PRIVATE doctest::doctest GLOBAL_OBJ)
# Keep util unit tests decoupled from the platform-specific input stack.
target_link_libraries(pdje_unit_util PRIVATE PDJE_UTIL)
target_link_libraries(pdje_unit_util PRIVATE PDJE_UTIL_IMAGE_WEBP)
target_link_libraries(pdje_unit_util PRIVATE PDJE_UTIL_IMAGE_WAVEFORM)
target_link_libraries(pdje_unit_util PRIVATE PDJE_UTIL_DB)
setOpenCLCppReqLib(pdje_unit_util)
setOpenCLRuntimeShimReqLib(pdje_unit_util)


target_compile_definitions(pdje_unit_util PRIVATE PDJE_UNIT_TESTING)
target_compile_definitions(
  pdje_unit_util
  PRIVATE
    PDJE_OPENCL_CLHPP_SUPPORTS_NO_PROTOTYPES=$<BOOL:${PDJE_OPENCL_CLHPP_SUPPORTS_NO_PROTOTYPES}>
    PDJE_OPENCL_CLHPP_SUPPORTS_NO_CORE_PROTOTYPES=$<BOOL:${PDJE_OPENCL_CLHPP_SUPPORTS_NO_CORE_PROTOTYPES}>)

# AddDynamicDef(pdje_unit_util)
# setCoreReqs(pdje_unit_util)
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
target_link_libraries(pdje_unit_util PRIVATE PDJE_LOG_RUNTIME)

setCoreReqs(pdje_unit_util)

setSpdlogReqLib(pdje_unit_util)
# setPdjeLogRuntimeReqLib(pdje_unit_util)
setCapnpReqLib(pdje_unit_util)
setLibgit2ReqLib(pdje_unit_util)
PDJE_COMPILE_OPTION(pdje_unit_util)
SET_PROPERTIES(pdje_unit_util)
pdje_discover_unit_tests(pdje_unit_util core)

add_executable(
  pdje_util_cmrc_probe
  ${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/util/cmrc_resource_probe.cpp)
setCmrcReqLib(pdje_util_cmrc_probe)
PDJE_COMPILE_OPTION(pdje_util_cmrc_probe)
SET_PROPERTIES(pdje_util_cmrc_probe)
add_test(NAME unit.util::cmrc_resource_probe COMMAND pdje_util_cmrc_probe)
