# PDJE Change Map

Use this table to find the owning code, explicit CMake wiring, and stable
verification surface. Target and label availability is defined by
[TEST_MAP.md](TEST_MAP.md). Rows marked `runtime` also require
[RUNTIME_CONTRACTS.md](RUNTIME_CONTRACTS.md).

| Change area | Primary code | CMake owner | Verification | Contract review |
| --- | --- | --- | --- | --- |
| core facade, DB search, playback | `include/core/interface/`, `include/core/MainObjects/audioPlayer/`, `include/core/audioRender/`, `include/core/db/` | `cmakes/src/CORE/` | core | runtime |
| editor, timeline, diff, lint, render | `include/core/MainObjects/editorObject/`, `include/core/editor/` | `cmakes/src/CORE/EDITORsrc.cmake` | core; approved `testEditor` scope | runtime |
| shared logging, data lines, IPC, crypto | `include/global/` | `cmakes/src/GLOBAL/`, `cmakes/BuildLog.cmake` | all affected labels | runtime for shared data |
| input discovery, state, device/MIDI runtime | `include/input/` | `cmakes/src/INPUT/`, `cmakes/BuildSubProc.cmake` | input | runtime |
| judge rules, rails, notes, loop | `include/judge/` | `cmakes/src/JUDGE/` | judge | runtime |
| util DB adapters | `include/util/db/` | `cmakes/src/UTIL/UTILsrc.cmake` | util | public API changes only |
| util AI / Beat This | `include/util/ai/` | `cmakes/src/UTIL/UTILsrc.cmake` | util, onnxruntime | public API changes only |
| util STFT, OpenCL, image/WebP | `include/util/function/stft/`, `include/util/function/image/`, `GenCodes/OKL/` | `cmakes/src/UTIL/UTILsrc.cmake`, `cmakes/findPackages.cmake` | util | public API changes only |
| util fuzzy/scalar/text | `include/util/function/fuzzy/`, `include/util/function/scalar/`, `include/util/function/text/` | `cmakes/src/UTIL/UTILsrc.cmake` when compiled | util | public API changes only |
| C ABI | `include/core/interface/CPDJE_interface.h`, `include/input/CPDJE_Input.h`, `include/judge/CPDJE_Judge.h`, and adjacent `.cpp` files | top-level `CMakeLists.txt`, `cmakes/tests/CAbiTests.cmake` | cabi, then affected labels | runtime |
| Cap'n Proto schema/translation | `third_party/Capnp/`, `include/core/db/Capnp/` | `cmakes/findPackages.cmake`, core source lists | core | schema compatibility |
| Python/C# bindings | `PDJE_swig.i`, `swig_python/`, `swig_csharp/` | `cmakes/ADD_SWIGS.cmake` | approved SWIG build | wrapped public API |
| options, dependencies, targets | `cmakes/Options.cmake`, `cmakes/findPackages.cmake`, top-level `CMakeLists.txt`, `CMakePresets.json` | changed CMake or bootstrap file | approved host suite | none |
| unit or manual test wiring | `tests/unit/`, `include/tests/` | `cmakes/tests/units/`, `cmakes/tests/CAbiTests.cmake`, `cmakes/tests/DevTests.cmake` | owning label or manual target | none |
| agent documentation | `AGENTS.md`, `AGENT_DOCS/` | [MAINTENANCE.md](MAINTENANCE.md) | static documentation checks | none |

When adding a `.cpp` or test file, update or confirm the owning explicit CMake
list in the same change.
