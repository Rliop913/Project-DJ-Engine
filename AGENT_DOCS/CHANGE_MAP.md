# PDJE Change Map

Use this map to locate the owning code, explicit source list, and stable test
route. Read the implementation before relying on the summary.

| Change area | Primary code | Build/source wiring | Verification route | Important boundary |
| --- | --- | --- | --- | --- |
| core facade, DB search, playback | `include/core/interface/`, `include/core/MainObjects/audioPlayer/`, `include/core/audioRender/`, `include/core/db/` | `cmakes/src/CORE/` | `core` label | playback exposes a non-owning core data line |
| editor, timeline, diff, lint, render | `include/core/MainObjects/editorObject/`, `include/core/editor/` | `cmakes/src/CORE/EDITORsrc.cmake` | `core` label; relevant manual `testEditor` flow only when requested | editor state and project files are owned below `editorObject` |
| shared logging, data lines, IPC, crypto | `include/global/` | `cmakes/src/GLOBAL/`, `cmakes/BuildLog.cmake` | all affected module labels | these types cross native and C ABI boundaries |
| input discovery, state, device/MIDI runtime | `include/input/` | `cmakes/src/INPUT/`, `cmakes/BuildSubProc.cmake` | `input` label | target exists only when `PDJE_DEVELOP_INPUT=ON`; subprocess target is Windows-only |
| judge rules, rails, notes, loop | `include/judge/` | `cmakes/src/JUDGE/` | `judge` label | consumes both core and input data lines |
| util DB adapters | `include/util/db/` | `cmakes/src/UTIL/UTILsrc.cmake` | `util` label | generic wrappers and concrete backends have separate headers |
| util AI / Beat This | `include/util/ai/` | `cmakes/src/UTIL/UTILsrc.cmake` | `util` plus `onnxruntime` labels | ONNX package smoke is a separate target |
| util STFT, OpenCL, image/WebP | `include/util/function/stft/`, `include/util/function/image/`, `GenCodes/OKL/` | `cmakes/src/UTIL/UTILsrc.cmake`, `cmakes/findPackages.cmake` | `util` label | these are public leaf headers, not exports of `PDJE_Util.hpp` |
| util fuzzy/scalar/text | `include/util/function/fuzzy/`, `include/util/function/scalar/`, `include/util/function/text/` | `cmakes/src/UTIL/UTILsrc.cmake` for non-header-only code | `util` label | preserve header-only status where appropriate |
| C ABI | `include/core/interface/CPDJE_interface.h`, `include/input/CPDJE_Input.h`, `include/judge/CPDJE_Judge.h` and adjacent `.cpp` files | top-level `CMakeLists.txt`, `cmakes/tests/CAbiTests.cmake` | `cabi` label, then the full applicable suite | ABI/versioned structs require compatibility review |
| Cap'n Proto schema/translation | `third_party/Capnp/`, `include/core/db/Capnp/` | `cmakes/findPackages.cmake` and core source lists | `core` label | generated files come from `pdje_capnp_codegen` |
| Python/C# bindings | `PDJE_swig.i`, `swig_python/`, `swig_csharp/` | `cmakes/ADD_SWIGS.cmake` | explicitly approved SWIG build | gated by `PDJE_SWIG_BUILD` |
| options, dependencies, targets | `cmakes/Options.cmake`, `cmakes/findPackages.cmake`, top-level `CMakeLists.txt` | `CMakePresets.json` and bootstrap scripts | approved host preset; full applicable suite | source defaults, presets, and caches are distinct |
| unit or manual test wiring | `tests/unit/`, `include/tests/` | `cmakes/tests/units/`, `cmakes/tests/DevTests.cmake` | [TEST_MAP.md](TEST_MAP.md) | files are not auto-discovered into targets |
| agent documentation | `AGENTS.md`, `AGENT_DOCS/` | [MAINTENANCE.md](MAINTENANCE.md) | static link/path/diff checks | document stable contracts and source anchors, not snapshots |

When a new `.cpp` or test file is added, inspect the listed CMake owner in the
same change. See [RUNTIME_CONTRACTS.md](RUNTIME_CONTRACTS.md) before changing a
cross-module lifecycle or pointer contract.
