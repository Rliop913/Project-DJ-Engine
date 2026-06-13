# PDJE Architecture

Concise map for `Project-DJ-Engine` (`PDJE`). Commands: [VERIFY.md](VERIFY.md).
Contracts: [DATA_CONTRACTS.md](DATA_CONTRACTS.md). Lifecycles:
[LIFECYCLES.md](LIFECYCLES.md).

## Surfaces

| Surface | Kind | Entry point | Role |
| --- | --- | --- | --- |
| `PDJE` | C++ facade | `include/core/interface/PDJE_interface.hpp` | DB search, playback, editor, core line |
| `PDJE_Input` | C++ facade | `include/input/PDJE_Input.hpp` | devices, config, run/kill, input line |
| `PDJE_JUDGE::JUDGE` | C++ facade | `include/judge/PDJE_Judge.hpp` | note/rule judging over core + input lines |
| `PDJE_UTIL` | C++ umbrella | `include/util/PDJE_Util.hpp` | status/result, DB wrappers, image/STFT helpers |
| `CPDJE` | C ABI | `include/core/interface/CPDJE_interface.cpp` | engine snapshots and handles |
| `CPDJE_MODULE_INPUT` | C ABI | `include/input/CPDJE_Input.cpp` | input lifecycle and snapshots |
| `CPDJE_MODULE_JUDGE` | C ABI | `include/judge/CPDJE_Judge.cpp` | judge lifecycle and callbacks |

## Build Graph

Implementation code mostly lives under `include/`, including many `.cpp` files.

| Target group | Members / gate |
| --- | --- |
| Always built | `PDJE`, `CPDJE`, `PDJE_LOG_RUNTIME`, `PDJE_UTIL`, `pdje_capnp_codegen` |
| Input/judge modules | `PDJE_MODULE_INPUT`, `PDJE_MODULE_JUDGE`, C ABI wrappers; `PDJE_DEVELOP_INPUT=ON` |
| Windows input process | `PDJE_MODULE_INPUT_PROCESS`; Windows raw-input path |
| Unit tests | `pdje_unit_core`, `pdje_unit_input`, `pdje_unit_judge`, `pdje_unit_util`; `PDJE_TEST=ON` |
| Dev/manual binaries | `testEditor`, `DBTester`, `gitTester`, `testInput`, `testMIDI`, `testJudge`, `music_to_waveform_webp`; `PDJE_DEV_TEST=ON` |
| SWIG | targets from `cmakes/ADD_SWIGS.cmake`; `PDJE_SWIG_BUILD=ON` |

## Source Tree

| Path | Role |
| --- | --- |
| `include/core/` | DB, playback, editor, root interface |
| `include/input/` | default devices, MIDI, IPC, run state |
| `include/judge/` | init data, loop, notes, input parsing |
| `include/util/` | common types, DB abstractions, image/STFT/AI helpers |
| `include/global/` | logging, clocks, crypto, data lines |
| `tests/unit/` | doctest suites registered with CTest |
| `include/tests/` | legacy/manual test sources and helper programs |
| `cmakes/` | build composition, package discovery, target/test wiring |
| `docs/` | redirecting HTML for the external docs site |
| `BluePrint_PDJE/` | archived design notes |

## Runtime Map

| Area | Flow | Deep dive |
| --- | --- | --- |
| Core playback | `PDJE` opens `DBROOT`, searches authored data, `InitPlayer()` creates `audioPlayer`, `PullOutDataLine()` exposes non-owning core state. | [CORE_RUNTIME.md](CORE_RUNTIME.md) |
| Editor | `InitEditor()` creates `editorObject`; editor paths own project-local state, history/diff/linter logic, render, and DB push. | [EDITOR_SYSTEM.md](EDITOR_SYSTEM.md) |
| Input | `Init()` prepares devices/MIDI, `Config()` chooses backend outcome, `Run()` starts capture, `PullOutDataLine()` exposes input/MIDI buffers. | [INPUT_SYSTEM.md](INPUT_SYSTEM.md) |
| Judge | `JUDGE` receives core/input lines, rules, rails, and notes; `Start()` validates/sorts/launches; `End()` clears cached init state. | [JUDGE_SYSTEM.md](JUDGE_SYSTEM.md) |
| Utility | `PDJE_UTIL` owns common result/status, DB wrappers, ONNX/Beat This, image/WebP, and STFT public surfaces plus internals. | [UTIL_SYSTEM.md](UTIL_SYSTEM.md) |

## Data Bridges

Data lines are non-owning; callers null-check every field.

| Type | Header | Fields |
| --- | --- | --- |
| `PDJE_CORE_DATA_LINE` | `include/global/DataLines/PDJE_Core_DataLine.hpp` | `nowCursor`, `maxCursor`, `preRenderedData`, `syncD` |
| `PDJE_INPUT_DATA_LINE` | `include/global/DataLines/PDJE_Input_DataLine.hpp` | `input_arena`, `midi_datas` |

C ABI layers mirror these through cached handles and snapshots, not raw owner
transfer.

## Platform Notes

- Linux and Windows support input/judge modules; Windows uses a subprocess
  raw-input path when enabled.
- macOS forces `PDJE_DEVELOP_INPUT=OFF`; core and util still build.

## Documentation Boundaries

- `AGENT_DOCS/` is the canonical Markdown control-doc surface.
- Public documentation lives outside this checkout.
- `docs/` redirects to the external docs site.
- `BluePrint_PDJE/` is archive-only.
