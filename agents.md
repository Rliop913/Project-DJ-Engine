# PDJE Agent Guide (Code-Verified)

Last verified against source: 2026-02-16

## 1) Purpose of this file
This is a fast onboarding guide for coding agents working in `Project_DJ_Engine`.
It records only facts verified from current code/build files and highlights stale areas.

## 2) One-screen summary
- Project: `Project-DJ-Engine` (`PDJE`), C++20 engine for DJ/rhythm workflows.
- Code layout: all C++ headers and implementations are under `include/`.
- Main API: `include/core/interface/PDJE_interface.hpp` (`class PDJE`).
- Always-built target: `PDJE` (core library).
- Conditional module targets: `PDJE_MODULE_INPUT`, `PDJE_MODULE_JUDGE`.
- Optional Windows subprocess target: `PDJE_MODULE_INPUT_PROCESS`.
- Docs pipeline: Doxygen + Sphinx (`DOCUMENT_GENERATOR.sh`).

## 3) Build truth from `CMakeLists.txt`
- CMake minimum: `3.12`.
- C++ standard: `20`.
- Default options:
- `PDJE_BUILD_TESTS=ON`
- `PDJE_DEVELOP_INPUT=ON` on Windows, `OFF` on Linux/macOS
- `PDJE_SWIG_BUILD=OFF`
- `PDJE_DYNAMIC=OFF`

### What each option really changes
- `PDJE_DYNAMIC`
- `OFF`: static libraries (default).
- `ON`: shared libraries (`PDJE`, and module libs if enabled).
- `PDJE_DEVELOP_INPUT`
- Enables building input/judge module libraries.
- On Windows also enables subprocess executable generation (`PDJE_MODULE_INPUT_PROCESS`) via `PDJE_ENABLE_SUBPROC`.
- `PDJE_SWIG_BUILD`
- Adds SWIG wrapper targets via `cmakes/ADD_SWIGS.cmake`.
- Note: `cmakes/findPackages.cmake` currently calls `find_package(SWIG REQUIRED)` unconditionally.

## 4) Target map
- `PDJE`
- Core engine library from `CORE_SRCS`.
- Includes DB, audio render, editor, interface, global utilities.
- `PDJE_MODULE_INPUT` (conditional)
- Built only when `PDJE_DEVELOP_INPUT=ON`.
- `PDJE_MODULE_JUDGE` (conditional)
- Built only when `PDJE_DEVELOP_INPUT=ON`.
- `PDJE_MODULE_INPUT_PROCESS` (conditional, Windows)
- Built only on Windows when `PDJE_DEVELOP_INPUT=ON`.
- Runs raw input loop as a child process.
- Test executables (conditional)
- Always (if `PDJE_BUILD_TESTS=ON`): `testEditor`, `DBTester`, `gitTester`.
- Also when `PDJE_DEVELOP_INPUT=ON`: `testInput`, `testMIDI`, `testJudge`.

## 5) Test reality (important)
- CI workflow runs `ctest` (`.github/workflows/cmake-multi-platform.yml`).
- But in `CMakeLists.txt`, `enable_testing()` and `add_test(...)` are commented out.
- Current state: test binaries are built, but `ctest` registration is effectively missing.

## 6) Module architecture (source-backed)
- Core engine: `include/core/`
- Main facade: `PDJE` (`SearchMusic`, `SearchTrack`, `InitPlayer`, `InitEditor`, `PullOutDataLine`).
- DB root: `litedb` (`include/core/db/dbRoot.hpp`) using SQLite + RocksDB.
- Binary translators: Cap'n Proto translators in `include/core/db/Capnp/Translators/`.
- Player path: `audioPlayer` + callbacks in `include/core/MainObjects/audioPlayer/`.
- Editor path: `editorObject` + timeline/git/json bridge in `include/core/editor/`.

- Input engine: `include/input/`
- Front API: `PDJE_Input`.
- Linux default devices: `include/input/DefaultDevs/linux/` (libevdev).
- Windows default devices: `include/input/DefaultDevs/windows/` (Raw Input + IPC + child process).
- MIDI path: `include/input/midi/` (`libremidi`).
- IPC path: `include/input/IPC/memory/` and `Secured_IPC_TX_RX`.

- Judge engine: `include/judge/`
- Front API: `PDJE_JUDGE::JUDGE`.
- Uses `PDJE_CORE_DATA_LINE` + `PDJE_INPUT_DATA_LINE` + note objects + rail/event rules.
- Matching logic currently includes TODO to choose closest note (not first match).

- Global utilities: `include/global/`
- Data lines, crypto (Botan), high-res clocks, RAII wrappers, logging helpers.

## 7) Known stale or misleading assumptions to avoid
- There is no active `Utility Engine` source module in the current tree.
- Do not assume `ctest` means real tests are executing.
- `includeDIRS.cmake` still references some legacy paths (for example `include/input/host`, `include/input/IPC/transmission`) that are not active source roots in current build lists.
- `input/trashbin/` is legacy; prefer non-trashbin paths.

## 8) Dependencies (current)
- Conan (`conanfile.txt`): `spdlog`, `annoy`, `capnproto`, `highway`, `libgit2`, `zlib`, `rocksdb`, `botan`.
- FetchContent (`cmakes/findPackages.cmake`): `miniaudio`, `nlohmann/json`, `sqlite amalgamation`, `cppcodec`, `libremidi`.
- Platform extras:
- Linux input build needs `libevdev` and `numa` via `pkg-config`.

## 9) Agent workflow checklist for safe edits
- Confirm whether the task touches `core` only or also `input/judge` modules.
- If changing build/test behavior, check `CMakeLists.txt` and `.github/workflows/cmake-multi-platform.yml` together.
- If changing docs, update `document_sources/` (not `docs/` generated output).
- If touching input code, verify Linux and Windows implementations separately.
- If adding tests, ensure they are registered (`enable_testing` + `add_test`) if `ctest` visibility is needed.

## 10) High-value file anchors
- `CMakeLists.txt`
- `cmakes/findPackages.cmake`
- `cmakes/includeDIRS.cmake`
- `include/core/interface/PDJE_interface.hpp`
- `include/core/interface/PDJE_interface.cpp`
- `include/core/MainObjects/audioPlayer/audioPlayer.cpp`
- `include/input/PDJE_Input.hpp`
- `include/input/PDJE_Input.cpp`
- `include/judge/PDJE_Judge.hpp`
- `include/judge/PDJE_Judge.cpp`
- `.github/workflows/cmake-multi-platform.yml`
- `DOCUMENT_GENERATOR.sh`
