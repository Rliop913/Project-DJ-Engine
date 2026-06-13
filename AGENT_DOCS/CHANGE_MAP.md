# PDJE Change Map

Use this when you know what you need to change, but not which repository slice
owns it.

| Change Area | Read First | Likely Edit Paths | Verify With | Caveats |
| --- | --- | --- | --- | --- |
| Core facade and playback | `CORE_RUNTIME.md` | `include/core/interface/`, `include/core/MainObjects/audioPlayer/`, `include/core/db/` | `TEST_MAP.md` -> full unit or core | play modes and core data lines cross module boundaries |
| Editor, timeline, diff, linter | `EDITOR_SYSTEM.md`, `LIFECYCLES.md` | `include/core/editor/`, `include/core/MainObjects/editorObject/` | `TEST_MAP.md` -> core | diff behavior is covered by core tests, not a separate editor binary |
| Input device mapping and runtime | `INPUT_SYSTEM.md`, `DATA_CONTRACTS.md` | `include/input/DefaultDevs/`, `include/input/runner/`, `include/input/IPC/`, `include/input/midi/` | `TEST_MAP.md` -> input | `PDJE_DEVELOP_INPUT` is forced off on macOS |
| Judge timing, rails, note objects | `JUDGE_SYSTEM.md`, `DATA_CONTRACTS.md`, `GLOSSARY.md` | `include/judge/Init/`, `include/judge/Loop/`, `include/judge/NoteOBJ/`, `include/judge/PDJE_RAIL.*` | `TEST_MAP.md` -> judge | judge setup depends on both core and input data lines |
| Utility DB wrappers | `UTIL_SYSTEM.md` | `include/util/db/`, `include/util/common/` | `TEST_MAP.md` -> util db and status | util doctest cases are registered under `unit.util::...`; ONNX smoke is `unit.onnxruntime::smoke` |
| Utility STFT, OpenCL, waveform, WebP | `UTIL_SYSTEM.md`, `GLOSSARY.md` | `include/util/function/stft/`, `include/util/function/stft/detail/`, `include/util/function/image/`, `include/util/function/image/detail/` | `TEST_MAP.md` -> util signal and image | runtime backend fallback behavior lives in util tests, not in the core facade; public leaves are thin and internal detail headers are included directly without forwarding shims |
| C ABI wrappers | `LIFECYCLES.md`, `TEST_MAP.md`, `DECISIONS.md` | `include/core/interface/CPDJE_interface.cpp`, `include/input/CPDJE_Input.cpp`, `include/judge/CPDJE_Judge.cpp` | `TEST_MAP.md` -> C ABI caveat | C ABI test files exist, but source-list wiring is partial |
| CMake options and test wiring | `INVARIANTS.md`, `VERIFY.md`, `DECISIONS.md` | `cmakes/Options.cmake`, `cmakes/tests/`, `CMakeLists.txt` | `VERIFY.md` -> source defaults and cache inspection | source defaults and local build cache are different sources of truth |
| Control docs and public docs | `INDEX.md`, `INVARIANTS.md`, `VERIFY.md` | `AGENT_DOCS/`, `README.md` | `VERIFY.md` quick doc checks | public docs are external, `docs/` is redirect-only, and `BluePrint_PDJE/` is archive only |
