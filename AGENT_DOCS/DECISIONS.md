# PDJE Decisions

This is the compact rationale ledger for repository-level choices that are easy
to accidentally "clean up" into breakage.

| Decision | Current Rule | Why | If This Changes, Also Update |
| --- | --- | --- | --- |
| Markdown control-doc home | `AGENT_DOCS/` is canonical | keeps agent-facing control docs separate from external public docs | `INDEX.md`, `AGENTS.md`, `README.md` |
| External-doc boundary | public documentation is external, and `docs/` contains redirecting HTML only | keeps maintenance edits away from redirect stubs | `INVARIANTS.md`, `VERIFY.md`, external-doc references |
| Blueprint vault boundary | `BluePrint_PDJE/` is archive only | historical notes still help search, but are not source of truth | archive landing pages and cross-links |
| Source defaults | defaults come from `cmakes/Options.cmake` | local `build/` caches can drift | `NOW.md`, `VERIFY.md`, `README.md` |
| Shared preset matrix | use root `CMakePresets.json` with repo-root `/build`, `Release` and `RelWithDebInfo`, and `PDJE_DYNAMIC=ON` everywhere | keeps cache location and mode names stable across platforms | `INVARIANTS.md`, `NOW.md`, `VERIFY.md`, `PROJECT_SKILLS.md`, `README.md` |
| Test gating in presets | only `*-relwithdebinfo` presets enable `PDJE_TEST` and `PDJE_DEV_TEST` | keeps compile-only `Release` separate from the heavier verification build | `INVARIANTS.md`, `VERIFY.md`, `README.md`, skill docs |
| Platform compiler locks | presets fix Windows=`cl`, Linux=`clang`, macOS=`clang` expecting AppleClang | keeps build identity explicit instead of inheriting an arbitrary shell compiler | `CMakePresets.json`, `VERIFY.md`, `README.md` |
| Windows preset wrapper | `windows_conf_and_build.bat` activates the Conan/MSVC environment and runs the matching Windows preset configure+build in one `cmd` process, with an explicit `on/off` switch for `--fresh` | avoids shell-environment loss while still letting callers choose whether to rebuild configure state from scratch | `KNOWN_GAPS.md`, `VERIFY.md`, Windows helper scripts, future preset docs |
| Input platform gate | `PDJE_DEVELOP_INPUT=ON` on Linux and Windows, forced `OFF` on macOS | current source tree only supports input and judge on Linux and Windows | `NOW.md`, `INVARIANTS.md`, `README.md` |
| SWIG requirement | SWIG is optional unless `PDJE_SWIG_BUILD=ON` | most builds do not need wrapper generation | `README.md`, `INVARIANTS.md`, external build docs |
| `include/` layout | headers and implementation `.cpp` files both live under `include/` | this repo is library-first and not split into a separate `src/` tree | `ARCHITECTURE.md`, onboarding docs, file-path examples |
| Unit test wiring | module unit binaries use explicit source lists under `cmakes/tests/units/` | current test coverage is shaped by CMake lists, not by auto-globbing | `TEST_MAP.md`, `CHANGE_MAP.md`, test maintenance work |
