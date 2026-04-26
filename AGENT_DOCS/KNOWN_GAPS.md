# PDJE Known Gaps

This page is the intentional list of current documentation and wiring gaps.

## Active Gaps

- `tests/unit/core/cpdje_interface_c_api.test.cpp` exists, but
  `cmakes/tests/units/Module_Core.cmake` does not wire it into `pdje_unit_core`.
- `tests/unit/input/cpdje_input_c_api.test.cpp` exists, but
  `cmakes/tests/units/Module_Input.cmake` does not wire it into
  `pdje_unit_input`.
- `tests/unit/judge/cpdje_judge_c_api.test.cpp` exists, but
  `cmakes/tests/units/Module_Judge.cmake` does not wire it into
  `pdje_unit_judge`.
- `PDJE_Input` can run MIDI-only, but `Judge_Init::SetInputLine()` currently
  requires `input_arena`, so judge setup still assumes keyboard/mouse transport
  presence.
- `windows_conf_and_build.bat` wraps Windows environment activation plus the
  matching preset configure/build, but it still assumes the matching Conan
  generated scripts already exist under `conan_cmakes/`.
- The shared presets do not run `conan install`. The caller must still prepare
  `conan_cmakes/` with the matching platform, compiler, and build type before
  running `cmake --preset ...`.
- The generated `conanbuild.bat` and `conanrun.bat` track the last Conan
  install mode, so mode-specific env scripts are still safer on Windows when
  switching between `Release` and `RelWithDebInfo`.
- `AGENT_DOCS` is now the maintenance doc harness. Public documentation is
  external; `document_sources/` is not part of the required maintenance reading
  flow.
- `docs/` contains only redirecting HTML for the external documentation site.
- `BluePrint_PDJE/` can still appear in search results even though it is
  archive-only material.

## What To Do About Them

- if you touch `CPDJE*`, inspect `cmakes/tests/units/` before claiming wrapper
  coverage is exercised
- if you touch judge/input coupling, check whether the `input_arena` contract
  should stay strict
- if you touch Windows build entrypoints, prefer a repo-owned preset or a
  thin wrapper over a repo-owned preset, and keep MSVC shell bootstrap explicit
- if you change the preset matrix, update the matching Conan bootstrap commands
  in `README.md`, `VERIFY.md`, and the preset skill references
- if you extend AGENT docs, keep new maintenance guidance in `AGENT_DOCS/`
  first instead of reviving root or archive docs
