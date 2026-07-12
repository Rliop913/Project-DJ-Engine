# PDJE Architecture

Project-DJ-Engine is a modular C++20 engine for authored DJ/rhythm content. It
combines database-backed music and track data, audio playback and editing,
device/MIDI input, note judging, signal/image utilities, and C/language binding
boundaries.

## Runtime Shape

~~~text
authored music + track DB
          |
          v
       PDJE core -----> playback/editor
          |                  |
          | core data line   | project data/render/push
          v                  v
       judge <--------- PDJE_Input
          ^             input data line
          |
       rules, rails, notes, callbacks

PDJE_UTIL supports DB abstractions, inference, STFT, waveform/WebP, and small
text/scalar helpers; it is not part of the core/input data-line ownership chain.
~~~

## Public Surfaces

| Surface | Public declaration | Role |
| --- | --- | --- |
| `PDJE` | `include/core/interface/PDJE_interface.hpp` | root DB, search, playback, editor, core data line |
| `PDJE_Input` | `include/input/PDJE_Input.hpp` | device/MIDI discovery, configuration, capture, input data line |
| `PDJE_JUDGE::JUDGE` | `include/judge/PDJE_Judge.hpp` | rules, rails, note staging, judge loop |
| `PDJE_UTIL` | `include/util/PDJE_Util.hpp` and public leaf headers under `include/util/` | reusable DB, AI, signal, image, text, and scalar facilities |
| Core C ABI | `include/core/interface/CPDJE_interface.h` | opaque handles and C snapshots for core/editor/player |
| Input C ABI | `include/input/CPDJE_Input.h` | input lifecycle and event snapshots |
| Judge C ABI | `include/judge/CPDJE_Judge.h` | judge setup, lifecycle, and callbacks |

`PDJE_Util.hpp` is intentionally not exhaustive: it includes AI, generic DB
wrappers, fuzzy search, clamp, and slugify. STFT and image/WebP callers include
their public leaf headers directly.

## Build Graph

| Condition | Targets / source of truth |
| --- | --- |
| normal configure | `PDJE`, `CPDJE`, `PDJE_UTIL`, `PDJE_LOG_RUNTIME`; top-level `CMakeLists.txt` and `cmakes/src/*/*Pkg.cmake` |
| generated Cap'n Proto code | support target `pdje_capnp_codegen`; `cmakes/findPackages.cmake` |
| `PDJE_DEVELOP_INPUT=ON` | `PDJE_MODULE_INPUT`, `PDJE_MODULE_JUDGE`, their C ABI libraries; top-level `CMakeLists.txt` |
| Windows plus input enabled | `PDJE_MODULE_INPUT_PROCESS`; `cmakes/BuildSubProc.cmake` |
| `PDJE_TEST=ON` | core, ONNX Runtime, and core C ABI tests on every host; util tests on non-Apple; input/judge native and C ABI tests when input is enabled; `cmakes/tests/UnitTestRoot.cmake`, `cmakes/tests/CAbiTests.cmake` |
| `PDJE_DEV_TEST=ON` | manual/dev executables listed in `cmakes/tests/DevTests.cmake` |
| `PDJE_SWIG_BUILD=ON` | Python/C# targets from `cmakes/ADD_SWIGS.cmake` |

Production source membership is explicit in `cmakes/src/`. Unit source
membership is explicit in `cmakes/tests/units/` and
`cmakes/tests/CAbiTests.cmake`; files merely present under `tests/unit/` may
not be compiled.

## Repository Map

| Path | Responsibility |
| --- | --- |
| `include/core/` | DB, audio render/playback, editor, core C++/C interfaces |
| `include/input/` | device/MIDI backends, IPC/runtime state, input C++/C interfaces |
| `include/judge/` | initialization, rails, note objects, parsing, loop, C++/C interfaces |
| `include/util/` | reusable DB adapters, AI, STFT, image/WebP, scalar/text functions |
| `include/global/` | shared data lines, logging, IPC, crypto, exports, common runtime pieces |
| `tests/unit/` | doctest and standalone unit sources; CMake lists decide active coverage |
| `include/tests/` | manual/dev program sources; only listed files become targets |
| `cmakes/src/` | production source lists and target requirements |
| `cmakes/tests/` | unit and dev target wiring |
| `third_party/` | vendored/generated-input sources such as schemas and Faust code |
| `GenCodes/` | generated accelerator resources consumed by util builds |

Cross-module lifecycle and pointer ownership details are in
[RUNTIME_CONTRACTS.md](RUNTIME_CONTRACTS.md).
