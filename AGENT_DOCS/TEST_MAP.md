# PDJE Test Map

These are the smallest stable verification routes in the current checkout.

| Area | Source Tests | Preferred Command | Optional Binary | Caveats |
| --- | --- | --- | --- | --- |
| Full unit suite | `tests/unit/*` | `ctest --test-dir build -L unit --output-on-failure` | `./build/pdje_unit_core`, `./build/pdje_unit_input`, `./build/pdje_unit_judge`, `./build/pdje_unit_util` | best default after cross-cutting changes |
| Core timelines, diff, BPM, formatting, linter | `tests/unit/core/*.test.cpp` from `Module_Core.cmake` | `ctest --test-dir build -R '^unit.core::core:' --output-on-failure` | `./build/pdje_unit_core` | covers editor/timeline behavior in current registration |
| Core PCM facade | `tests/unit/core/pdje_interface_pcm.test.cpp` | `ctest --test-dir build -R '^unit.core::PDJE GetPCMFromMusData' --output-on-failure` | `./build/pdje_unit_core` | narrow core facade coverage only |
| Input module | `tests/unit/input/input_sanity.test.cpp`, `pdje_input_state_logic.test.cpp`, Linux input tests | `ctest --test-dir build -R '^unit.input::' --output-on-failure` | `./build/pdje_unit_input` | Linux-specific cases are present only when built on Linux |
| Judge module | `tests/unit/judge/judge_algorithms.test.cpp`, `rail_db.test.cpp`, `note_obj.test.cpp` | `ctest --test-dir build -R '^unit.judge::' --output-on-failure` | `./build/pdje_unit_judge` | current registration focuses on algorithms, rails, and note objects |
| Util DB and status | `tests/unit/util/db_*.test.cpp`, `status.test.cpp`, `public_headers.test.cpp` | `ctest --test-dir build -R 'unit.core::(rocksdb|annoy|sqlite|util)' --output-on-failure` | `./build/pdje_unit_util` | util tests are registered under the `unit.core::...` prefix |
| Util signal, runtime, image, WebP | `tests/unit/util/opencl_*.test.cpp`, `stft_parallel.test.cpp`, `waveform_webp.test.cpp`, `webp_writer.test.cpp` | `ctest --test-dir build -R 'unit.core::(backendless|post process rgb mode|stft|mel filter bank|waveform|encode_waveform_webps|encode_webp|write_webp)' --output-on-failure` | `./build/pdje_unit_util` | use the util binary when the regex becomes too brittle |
| Dev/manual executables | `include/tests/`, `cmakes/tests/DevTests.cmake` | `ctest --test-dir build -L unit --output-on-failure` first, then run the manual executable you changed | `./build/testEditor`, `./build/testInput`, `./build/testMIDI`, `./build/testJudge`, `./build/DBTester`, `./build/gitTester` | requires `PDJE_DEV_TEST=ON`; not a substitute for unit coverage |

## Current C ABI Wiring Caveat

- `tests/unit/core/cpdje_interface_c_api.test.cpp` exists in the tree.
- `tests/unit/input/cpdje_input_c_api.test.cpp` exists in the tree.
- `tests/unit/judge/cpdje_judge_c_api.test.cpp` exists in the tree.
- The current unit target source lists under `cmakes/tests/units/` do not wire
  those files into `ctest --test-dir build -L unit`.
- If you touch `CPDJE*` wrappers in this checkout, run the owning unit binaries
  and re-check the module CMake wiring before claiming explicit C ABI coverage.
- Track this caveat through [KNOWN_GAPS.md](KNOWN_GAPS.md).
