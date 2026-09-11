# PDJE Test Map

CTest labels are the stable module-level verification interface. Individual
doctest names are suitable only for a deliberately narrow rerun.

| Coverage | Build target | Stable CTest label / status | Availability | Wiring source |
| --- | --- | --- | --- | --- |
| full applicable unit suite | configured test tree | `unit` through the host test preset | host-dependent | `CMakePresets.json`, `cmakes/tests/UnitTestRoot.cmake` |
| core | `pdje_unit_core` | `core` | all hosts | `cmakes/tests/units/Module_Core.cmake` |
| input, including native-to-C event snapshot adapter | `pdje_unit_input` | `input` | `PDJE_DEVELOP_INPUT=ON` | `cmakes/tests/units/Module_Input.cmake` |
| judge | `pdje_unit_judge` | `judge` | `PDJE_DEVELOP_INPUT=ON` | `cmakes/tests/units/Module_Judge.cmake` |
| util and embedded-resource probe | `pdje_unit_util`, `pdje_util_cmrc_probe` | `util` | non-Apple | `cmakes/tests/units/Module_Util.cmake` |
| ONNX Runtime package smoke | `pdje_onnxruntime_smoke` | `onnxruntime` | all hosts | `cmakes/tests/units/Module_OnnxRuntime.cmake` |
| core C ABI doctests and pure-C symbols | `pdje_unit_cabi_core`, `pdje_cabi_core_smoke` | `cabi` | all hosts | `cmakes/tests/CAbiTests.cmake` |
| input/judge C ABI doctests and pure-C symbols | `pdje_unit_cabi_modules`, `pdje_cabi_input_smoke`, `pdje_cabi_judge_smoke` | `cabi` | `PDJE_DEVELOP_INPUT=ON` | `cmakes/tests/CAbiTests.cmake` |
| core/general manual programs | `testEditor`, `DBTester`, `gitTester`, `music_to_waveform_webp` | not registered with CTest | `PDJE_DEV_TEST=ON` | `cmakes/tests/DevTests.cmake` |
| util/inference manual programs | `testOnnxLoad`, `testBeatThis` | not registered with CTest | `PDJE_DEV_TEST=ON` | `cmakes/tests/DevTests.cmake` |
| input/judge manual programs | `testInput`, `testMIDI`, `testJudge` | not registered with CTest | `PDJE_DEV_TEST=ON` and `PDJE_DEVELOP_INPUT=ON` | `cmakes/tests/DevTests.cmake` |

Every registered row also carries the `unit` label. Doctest cases are
discovered after their binaries build, so pre-build test enumeration is not a
complete inventory. For manual targets, report compilation and meaningful
execution separately.
