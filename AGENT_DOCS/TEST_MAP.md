# PDJE Test Map

CTest labels are the stable module-level interface. Individual doctest names
are implementation detail and are appropriate only for a deliberately narrow
rerun.

## Unit Targets

All commands require the approved `<host>-relwithdebinfo` flow.

| Area | Build target | Stable run | Availability | Wiring source |
| --- | --- | --- | --- | --- |
| full applicable unit suite | all configured targets | `ctest --preset <host>-relwithdebinfo` | host-dependent | `cmakes/tests/UnitTestRoot.cmake` |
| core | `pdje_unit_core` | `ctest --test-dir ./build -L core --output-on-failure` | all hosts | `cmakes/tests/units/Module_Core.cmake` |
| input | `pdje_unit_input` | `ctest --test-dir ./build -L input --output-on-failure` | `PDJE_DEVELOP_INPUT=ON` | `cmakes/tests/units/Module_Input.cmake` |
| judge | `pdje_unit_judge` | `ctest --test-dir ./build -L judge --output-on-failure` | `PDJE_DEVELOP_INPUT=ON` | `cmakes/tests/units/Module_Judge.cmake` |
| util | `pdje_unit_util` and `pdje_util_cmrc_probe` | `ctest --test-dir ./build -L util --output-on-failure` | non-Apple | `cmakes/tests/units/Module_Util.cmake` |
| ONNX Runtime package smoke | `pdje_onnxruntime_smoke` | `ctest --test-dir ./build -L onnxruntime --output-on-failure` | all hosts | `cmakes/tests/units/Module_OnnxRuntime.cmake` |
| C ABI | `pdje_unit_cabi_core`, `pdje_cabi_core_smoke`; module targets below | `ctest --test-dir ./build -L cabi --output-on-failure` | core on all hosts; input/judge when `PDJE_DEVELOP_INPUT=ON` | `cmakes/tests/CAbiTests.cmake` |

Every registered row also carries the `unit` label, so
`ctest --test-dir ./build -L unit --output-on-failure` runs the applicable
combined suite.

## C ABI Targets

The C ABI is verified against the shared facade libraries rather than only the
native object targets:

- `pdje_unit_cabi_core` runs the core C ABI doctests against `CPDJE`.
- `pdje_unit_cabi_modules` runs the input and judge C ABI doctests against
  `CPDJE_MODULE_INPUT`, `CPDJE_MODULE_JUDGE`, and `CPDJE`; it exists only
  when input development is enabled.
- `pdje_cabi_core_smoke`, `pdje_cabi_input_smoke`, and
  `pdje_cabi_judge_smoke` compile their sources as C and touch the exported
  V1 symbols. The module smokes follow the input-development gate.

The `cabi` label runs all applicable doctests and pure-C smokes. The existing
`tests/unit/input/cpdje_input_snapshot_event_view.test.cpp` remains part of
`pdje_unit_input` and covers the native-to-C event snapshot adapter.

## Manual/Dev Targets

`PDJE_DEV_TEST=ON` creates standalone targets from
`cmakes/tests/DevTests.cmake`:

- core/general: `testEditor`, `DBTester`, `gitTester`,
  `music_to_waveform_webp`;
- util/inference: `testOnnxLoad`, `testBeatThis`;
- input-enabled only: `testInput`, `testMIDI`, `testJudge`.

These targets are not registered as the unit suite. Record separately whether a
target only compiled or its executable was run with meaningful input.
