# Agent Memo: PDJE Deep-Dive (Code-Verified)

Last verified against repository state: 2026-02-16

## 1) Why this memo exists
This memo is the deeper companion to `agents.md`.
It is intended for agents doing non-trivial work (feature edits, refactors, CI/build fixes, documentation updates).

## 2) Quick correction log (old assumptions vs current code)
- Old: "`agents.md` should be recursively updated via `gemini.md` workflow."
- Current: Not aligned with practical repository work. This was removed from agent guidance.

- Old: "Utility Engine exists as a normal module."
- Current: No active utility source module exists under `include/`. Treat utility as roadmap/placeholder, not build target.

- Old: "`ctest` executes project tests in CI."
- Current: CI runs `ctest`, but `enable_testing()` and `add_test()` are commented out in `CMakeLists.txt`. Tests are mostly build-only today.

- Old: "Engine is only a library; no executables."
- Current: Main deliverable is library-centric, but repository also builds executables (`test*`, and `PDJE_MODULE_INPUT_PROCESS` on Windows when enabled).

- Old: "Input is generally cross-platform in equal maturity."
- Current: Linux and Windows have concrete `DefaultDevs` implementations. macOS input path is not wired as an active implementation.

- Old: "SWIG is only needed when SWIG build option is ON."
- Current: `find_package(SWIG REQUIRED)` is currently unconditional in `cmakes/findPackages.cmake`.

## 3) Ground truth: build system behavior

### Core facts
- Root build file: `CMakeLists.txt`.
- Minimum CMake version: `3.12`.
- Language standard: C++20.
- Main always-built target: `PDJE`.

### Key options and defaults
- `PDJE_BUILD_TESTS=ON`.
- `PDJE_DEVELOP_INPUT=ON` on Windows, `OFF` on Linux/macOS.
- `PDJE_SWIG_BUILD=OFF`.
- `PDJE_DYNAMIC=OFF`.

### Target conditions
- `PDJE`
- Always built.
- Contains core/editor/audio/db/global code.

- `PDJE_MODULE_INPUT`
- Built only when `PDJE_DEVELOP_INPUT=ON`.

- `PDJE_MODULE_JUDGE`
- Built only when `PDJE_DEVELOP_INPUT=ON`.

- `PDJE_MODULE_INPUT_PROCESS`
- Built only on Windows with `PDJE_DEVELOP_INPUT=ON` (subprocess raw input service).

- SWIG wrapper targets
- Built only when `PDJE_SWIG_BUILD=ON` (`pdje_python`, `pdje_csharp`).

### Test status reality
- CMake creates test executables when `PDJE_BUILD_TESTS=ON`:
- `testEditor`, `DBTester`, `gitTester`
- plus `testInput`, `testMIDI`, `testJudge` when `PDJE_DEVELOP_INPUT=ON`
- But no active `enable_testing/add_test` registration currently.

## 4) Dependency model (actual)

### Conan (`conanfile.txt`)
- `spdlog/1.15.3`
- `annoy/1.17.3`
- `capnproto/1.1.0`
- `highway/1.2.0`
- `libgit2/1.8.4`
- `zlib/1.2.13`
- `rocksdb/10.5.1`
- `botan/3.10.0`

### FetchContent (`cmakes/findPackages.cmake`)
- `miniaudio`
- `nlohmann/json`
- `sqlite amalgamation`
- `cppcodec`
- `libremidi`

### Platform extras
- Linux input path links `libevdev` and `numa` through `pkg-config`.

## 5) Source tree map (what matters first)

### Core API and orchestration
- `include/core/interface/PDJE_interface.hpp`
- Main facade `PDJE`.
- Entry flow: open DB -> search track/music -> init player or editor -> pull data lines.

- `include/core/interface/PDJE_interface.cpp`
- Implements `SearchMusic`, `SearchTrack`, `InitPlayer`, `InitEditor`, `GetNoteObjects`, `PullOutDataLine`.

### Audio path
- `include/core/MainObjects/audioPlayer/audioPlayer.hpp`
- `include/core/MainObjects/audioPlayer/audioPlayer.cpp`
- `include/core/MainObjects/audioPlayer/audioCallbacks.cpp`
- Three render modes:
- `FULL_PRE_RENDER`
- `HYBRID_RENDER`
- `FULL_MANUAL_RENDER`
- Sync via `audioSyncData` and high-resolution clocks.

### DB and serialization path
- `include/core/db/dbRoot.hpp`: `litedb` wrapper over SQLite + RocksDB.
- `include/core/db/musicDB.hpp`, `include/core/db/trackDB.hpp`: metadata models and SQL statement generators.
- `include/core/db/Capnp/Translators/`: frame calc + mix/music/note translators.

### Editor path
- `include/core/MainObjects/editorObject/editorObject.hpp`
- `include/core/editor/editor.hpp`
- `include/core/editor/TimeLine/TimeLine.hpp`
- Timeline + Git-backed history + JSON wrappers + project-local temp DB.

### Input path
- Front API: `include/input/PDJE_Input.hpp`.
- Linux devices: `include/input/DefaultDevs/linux/`.
- Windows devices: `include/input/DefaultDevs/windows/`.
- IPC security/data movement: `include/input/IPC/memory/` + `Secured_IPC_TX_RX`.
- MIDI: `include/input/midi/`.

### Judge path
- Front API: `include/judge/PDJE_Judge.hpp`.
- Init and rules: `include/judge/Init/`.
- Loop + preprocess + matching: `include/judge/Loop/`.
- Note object store: `include/judge/NoteOBJ/`.
- `include/judge/AxisModel/AxisModel.hpp` currently placeholder.

### Global shared utilities
- `include/global/DataLines/`
- `include/global/Crypto/`
- `include/global/Highres_Clock/`

## 6) Data-line contracts (important integration seam)
- `PDJE_CORE_DATA_LINE` (`include/global/DataLines/PDJE_Core_DataLine.hpp`)
- Pointers to cursor/sync/prerender buffers for judge or external runtime use.

- `PDJE_INPUT_DATA_LINE` (`include/global/DataLines/PDJE_Input_DataLine.hpp`)
- Pointers to input transfer arena and MIDI double buffer.

These structs are pointer-heavy and can be null depending on initialization path.

## 7) CI and release automation facts
- Main CI workflow: `.github/workflows/cmake-multi-platform.yml`.
- Matrix: `windows-latest`, `ubuntu-latest`, `macOS-14`.
- Conan cache and ccache are used.
- CI config currently builds with `-DPDJE_DYNAMIC=OFF`.
- CI runs `ctest` after build (see testing caveat above).
- Auto-tag workflow: `.github/workflows/autoTag.yml` reads `PDJE_VERSION` and creates git tag.
- Current `PDJE_VERSION`: `0.7.4`.

## 8) Documentation system facts
- Source docs: `document_sources/`.
- Generated docs output: `docs/`.
- Scripted generation: `DOCUMENT_GENERATOR.sh` (`doxygen` then `sphinx-build`).
- Root `Makefile` appears to be default Sphinx template (`SOURCEDIR=source`) and does not match active `document_sources/` layout.

## 9) Known technical debt and risky zones
- Testing visibility gap: test binaries exist, but `ctest` registration is commented out.
- Input build gating: Linux/macOS default disables input/judge modules (`PDJE_DEVELOP_INPUT=OFF`).
- SWIG requirement is unconditional at configure time.
- `cmakes/includeDIRS.cmake` includes legacy path references not represented as active current source trees.
- `include/input/trashbin/` and deprecated transmission paths can mislead navigation.
- Judge matching algorithm has explicit TODO in `include/judge/Loop/Match/PDJE_Match.cpp` (closest-note logic).
- Axis model is placeholder (`include/judge/AxisModel/AxisModel.hpp`).

## 10) Agent-ready task recipes

### If asked to fix tests/CI
- Decide whether goal is build-only tests or executable test runs.
- If executable runs are required, wire `enable_testing()` and `add_test(...)` explicitly.
- Re-check CI `ctest` step after any CMake test wiring changes.

### If asked to touch input/judge on Linux/macOS
- Verify `PDJE_DEVELOP_INPUT` behavior first.
- For Linux, ensure `libevdev` and `numa` assumptions are still valid.
- For macOS, assume missing default device implementation until proven otherwise.

### If asked to update docs
- Edit files in `document_sources/`.
- Treat `docs/` as generated artifact.
- Validate discrepancies against source files, not historical memo text.

### If asked to extend public API
- Start at `PDJE_interface.hpp/.cpp`.
- Check downstream impacts on data lines and editor/audio initialization order.

## 11) Minimal file watchlist for large refactors
- `CMakeLists.txt`
- `cmakes/findPackages.cmake`
- `cmakes/includeDIRS.cmake`
- `include/core/interface/PDJE_interface.hpp`
- `include/core/interface/PDJE_interface.cpp`
- `include/core/MainObjects/audioPlayer/audioPlayer.cpp`
- `include/input/PDJE_Input.cpp`
- `include/input/DefaultDevs/linux/DefaultDevs.cpp`
- `include/input/DefaultDevs/windows/DefaultDevs.cpp`
- `include/judge/PDJE_Judge.cpp`
- `.github/workflows/cmake-multi-platform.yml`

## 12) Practical boundaries for claims in future updates
When updating this memo, keep claims in one of these categories:
- Verified directly from source/build files.
- Inferred with explicit wording.
- Unknown/open and listed as unresolved.

Avoid carrying forward assumptions from older docs unless re-verified in code.
