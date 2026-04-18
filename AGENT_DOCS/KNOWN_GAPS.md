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
- `AGENT_DOCS` is now the maintenance doc harness. `document_sources/` is not
  part of the required maintenance reading flow.
- `BluePrint_PDJE/` can still appear in search results even though it is
  archive-only material.

## What To Do About Them

- if you touch `CPDJE*`, inspect `cmakes/tests/units/` before claiming wrapper
  coverage is exercised
- if you touch judge/input coupling, check whether the `input_arena` contract
  should stay strict
- if you extend AGENT docs, keep new maintenance guidance in `AGENT_DOCS/`
  first instead of reviving root or archive docs
