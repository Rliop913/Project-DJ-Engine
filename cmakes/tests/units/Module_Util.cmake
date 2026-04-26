
if(APPLE)
return()
endif()


add_executable(
  pdje_unit_util
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/main_doctest.cpp
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/util/public_headers.test.cpp
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/util/ai_beat_this.test.cpp
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/util/webp_writer.test.cpp
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/util/waveform_webp.test.cpp
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/util/stft_parallel.test.cpp
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/util/opencl_clhpp_probe.test.cpp
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/util/opencl_runtime_shim.test.cpp
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/util/db_relational_sqlite.test.cpp
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/util/db_keyvalue_rocksdb.test.cpp
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/util/db_nearest_annoy.test.cpp
${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/util/status.test.cpp
${UTIL_SRC_EXPORT}
)


target_link_libraries(pdje_unit_util PRIVATE doctest::doctest UTIL_OBJ)


target_compile_definitions(pdje_unit_util PRIVATE PDJE_UNIT_TESTING)
target_compile_definitions(
  pdje_unit_util
  PRIVATE
    PDJE_OPENCL_CLHPP_SUPPORTS_NO_PROTOTYPES=$<BOOL:${PDJE_OPENCL_CLHPP_SUPPORTS_NO_PROTOTYPES}>
    PDJE_OPENCL_CLHPP_SUPPORTS_NO_CORE_PROTOTYPES=$<BOOL:${PDJE_OPENCL_CLHPP_SUPPORTS_NO_CORE_PROTOTYPES}>)

setUtilReqs(pdje_unit_util)
pdje_copy_zlib_runtime(pdje_unit_util)
if(WIN32)
  pdje_copy_onnxruntime_runtime(pdje_unit_util)
endif()
pdje_discover_unit_tests(pdje_unit_util util)

add_executable(
  pdje_util_cmrc_probe
  ${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/util/cmrc_resource_probe.cpp)
setCmrcReqLib(pdje_util_cmrc_probe)
PDJE_COMPILE_OPTION(pdje_util_cmrc_probe)
SET_PROPERTIES(pdje_util_cmrc_probe)
add_test(NAME unit.util::cmrc_resource_probe COMMAND pdje_util_cmrc_probe)
set_tests_properties(
  unit.util::cmrc_resource_probe
  PROPERTIES LABELS "unit;util;${PDJE_TEST_PLATFORM_LABEL}")
