add_executable(
  pdje_onnxruntime_smoke
  ${CMAKE_CURRENT_SOURCE_DIR}/tests/unit/onnxruntime/onnxruntime_smoke.cpp
)

setOnnxRuntimeReqLib(pdje_onnxruntime_smoke)
pdje_copy_onnxruntime_runtime(pdje_onnxruntime_smoke)
PDJE_COMPILE_OPTION(pdje_onnxruntime_smoke)
SET_PROPERTIES(pdje_onnxruntime_smoke)

add_test(NAME unit.onnxruntime::smoke COMMAND pdje_onnxruntime_smoke)
set_tests_properties(
  unit.onnxruntime::smoke
  PROPERTIES LABELS "unit;onnxruntime;${PDJE_TEST_PLATFORM_LABEL}"
)
