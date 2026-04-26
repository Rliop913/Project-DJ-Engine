# PDJE Architecture

This is the canonical architecture deep dive for `Project-DJ-Engine` (`PDJE`).
For current state, see [NOW.md](NOW.md). For commands, see
[VERIFY.md](VERIFY.md). For change routing, see
[CHANGE_MAP.md](CHANGE_MAP.md). For project terms, see
[GLOSSARY.md](GLOSSARY.md). For current contracts and valid call order, see
[DATA_CONTRACTS.md](DATA_CONTRACTS.md) and [LIFECYCLES.md](LIFECYCLES.md).

## Canonical Surfaces

PDJE exposes four main C++ integration surfaces and three C ABI facade layers.

| Surface | Kind | Entry point | Purpose |
| --- | --- | --- | --- |
| `PDJE` | C++ facade | `include/core/interface/PDJE_interface.hpp` | Root engine facade for DB search, playback startup, editor startup, note extraction, and core data-line access |
| `PDJE_Input` | C++ facade | `include/input/PDJE_Input.hpp` | Input enumeration, configuration, run/kill lifecycle, and input data-line access |
| `PDJE_JUDGE::JUDGE` | C++ facade | `include/judge/PDJE_Judge.hpp` | Rhythm-judge orchestration over synchronized core/input data and note/rule state |
| `PDJE_UTIL` | C++ umbrella header | `include/util/PDJE_Util.hpp` | Shared utility surface for status/result types, DB wrappers, and function helpers |
| `CPDJE` | C ABI library | `include/core/interface/CPDJE_interface.cpp` | C-facing engine facade and snapshots for music/track/core-line access |
| `CPDJE_MODULE_INPUT` | C ABI library | `include/input/CPDJE_Input.cpp` | C-facing input lifecycle, enumeration, and snapshot APIs |
| `CPDJE_MODULE_JUDGE` | C ABI library | `include/judge/CPDJE_Judge.cpp` | C-facing judge lifecycle and callback-based result delivery |

## Build Graph

The CMake graph is library-first. Most implementation code lives under
`include/`, including many `.cpp` files.

### Always-built targets

- `PDJE`: main C++ library for core playback/editor/DB functionality
- `CPDJE`: C ABI wrapper around the core engine
- `PDJE_LOG_RUNTIME`: logging runtime support
- `PDJE_UTIL`: shared utility library for status/result types, DB wrappers,
  and image/STFT helpers
- `pdje_capnp_codegen`: Cap'n Proto code generation target used by downstream
  targets

### Conditional targets

- `PDJE_MODULE_INPUT`, `PDJE_MODULE_JUDGE`: built when `PDJE_DEVELOP_INPUT=ON`
- `CPDJE_MODULE_INPUT`, `CPDJE_MODULE_JUDGE`: C ABI wrappers for input and
  judge, also gated by `PDJE_DEVELOP_INPUT=ON`
- `PDJE_MODULE_INPUT_PROCESS`: Windows-only subprocess target for the raw-input
  path
- `pdje_unit_core`, `pdje_unit_input`, `pdje_unit_judge`, `pdje_unit_util`:
  doctest binaries built when `PDJE_TEST=ON`
- `testEditor`, `DBTester`, `gitTester`, `testInput`, `testMIDI`, `testJudge`,
  `music_to_waveform_webp`: developer/manual executables built when
  `PDJE_DEV_TEST=ON`
- SWIG targets from `cmakes/ADD_SWIGS.cmake`: built when `PDJE_SWIG_BUILD=ON`

## Source Tree Map

| Path | Role |
| --- | --- |
| `include/core/` | Core engine: DB, playback, editor, interface facade |
| `include/input/` | Input engine: default devices, MIDI, IPC, run state |
| `include/judge/` | Judge engine: init data, loop, note objects, input parsing |
| `include/util/` | Utility surface: common types, DB abstractions, image/STFT helpers |
| `include/global/` | Shared cross-cutting infrastructure: logging, clocks, crypto, data lines |
| `tests/unit/` | Doctest unit suites registered with CTest |
| `include/tests/` | Legacy/manual test sources and helper programs |
| `cmakes/` | Build composition, package discovery, target assembly, test wiring |
| `docs/` | Redirecting HTML to the external documentation site |
| `BluePrint_PDJE/` | Archived design notes; not the current source of truth |

## Runtime Data Flow

### Playback Flow

1. `PDJE` constructs and opens the root DB through `litedb`.
2. `SearchMusic()` and `SearchTrack()` resolve authored data from the DB.
3. `InitPlayer()` creates `audioPlayer` in one of three modes:
   `FULL_PRE_RENDER`, `HYBRID_RENDER`, or `FULL_MANUAL_RENDER`.
4. `PullOutDataLine()` exposes a non-owning `PDJE_CORE_DATA_LINE` snapshot to
   other modules.

Relevant implementation anchors:

- `include/core/interface/PDJE_interface.cpp`
- `include/core/MainObjects/audioPlayer/`
- `include/core/db/`
- deep-dive doc: [CORE_RUNTIME.md](CORE_RUNTIME.md)

### Editor Flow

1. `PDJE::InitEditor()` creates an `editorObject`.
2. The editor path spans `include/core/MainObjects/editorObject/` and
   `include/core/editor/`.
3. This layer handles project-local state, timeline/history logic, JSON working
   files, diff/linter helpers, and persistence back toward the DB.

Deep-dive doc: [EDITOR_SYSTEM.md](EDITOR_SYSTEM.md)

### Input Flow

1. `PDJE_Input::Init()` prepares platform device discovery and MIDI support.
2. `Config()` sanitizes selected devices and chooses the final backend outcome.
3. `Run()` starts the default device loop and optional MIDI ingestion.
4. `PullOutDataLine()` exposes a `PDJE_INPUT_DATA_LINE` containing the keyboard
   / mouse transport arena and optional MIDI buffer.

Relevant implementation anchors:

- `include/input/PDJE_Input.cpp`
- `include/input/DefaultDevs/`
- `include/input/midi/`
- `include/input/IPC/`
- deep-dive doc: [INPUT_SYSTEM.md](INPUT_SYSTEM.md)

### Judge Flow

1. `PDJE_JUDGE::JUDGE` receives init data through `Judge_Init`.
2. Required inputs are:
   core data line, input data line, note objects, event rule, and input-rail
   mappings.
3. `Start()` validates init state, sorts note objects, creates `Judge_Loop`,
   and launches the event loop thread.
4. `End()` tears down the loop and clears cached init state.

Relevant implementation anchors:

- `include/judge/PDJE_Judge.cpp`
- `include/judge/Init/`
- `include/judge/Loop/`
- `include/judge/NoteOBJ/`
- deep-dive doc: [JUDGE_SYSTEM.md](JUDGE_SYSTEM.md)

### Utility Flow

`PDJE_UTIL` is active code, not a roadmap-only namespace. The current utility
surface includes:

- status/result types under `include/util/common/`
- relational, key-value, and nearest-neighbor DB wrappers under
  `include/util/db/`
- generic ONNX Runtime session ownership under `include/util/ai/AI.hpp`
- Beat This ownership under `include/util/ai/beat_this/`
- image/WebP public entrypoints under `include/util/function/image/`, with
  pipeline ownership in `include/util/function/image/detail/`
- STFT public entrypoints under `include/util/function/stft/`, with runtime and
  backend ownership in `include/util/function/stft/detail/`

Deep-dive doc: [UTIL_SYSTEM.md](UTIL_SYSTEM.md)

## Data Bridges

These structs are non-owning integration seams. Callers must null-check fields
before use.

| Type | Header | Fields |
| --- | --- | --- |
| `PDJE_CORE_DATA_LINE` | `include/global/DataLines/PDJE_Core_DataLine.hpp` | `nowCursor`, `maxCursor`, `preRenderedData`, `syncD` |
| `PDJE_INPUT_DATA_LINE` | `include/global/DataLines/PDJE_Input_DataLine.hpp` | `input_arena`, `midi_datas` |

The C ABI layers mirror these through cached handle fields and snapshot structs
instead of exposing raw C++ owners directly.

## Platform Notes

- Linux: input and judge modules are supported and enabled by source default
- Windows: input and judge modules are supported and use a subprocess raw-input
  path when enabled
- macOS: `PDJE_DEVELOP_INPUT` is forced `OFF` in `cmakes/Options.cmake`; core
  and util still build

## Documentation Boundaries

- `AGENT_DOCS/` is the canonical Markdown control-doc surface
- public documentation is maintained outside this checkout
- `docs/` contains only redirecting HTML for the external documentation site
- `BluePrint_PDJE/` is retained as archived design context only
