# PDJE Judge System

This page covers `PDJE_JUDGE::JUDGE`, `Judge_Init`, rails, and note staging.

## Purpose

- consume synchronized core and input data lines
- map device and MIDI routes onto gameplay rails
- stage note objects for matching and miss detection
- run the judge loop and emit use/miss callbacks

## Primary Entry Points

- `include/judge/PDJE_Judge.hpp`
- `include/judge/PDJE_Judge.cpp`
- `include/judge/Init/PDJE_Judge_Init.hpp`
- `include/judge/PDJE_RAIL.hpp`
- `include/judge/NoteOBJ/PDJE_Note_OBJ.hpp`

## Owning Paths

- `include/judge/Init/`
- `include/judge/Loop/`
- `include/judge/InputParser/`
- `include/judge/NoteOBJ/`

## Lifecycle / Flow

1. Construct `PDJE_JUDGE::JUDGE`.
2. Fill `judge.inits` with:
   core line, input line, event rule, custom callbacks, rails, and note
   objects.
3. `SetCoreLine()` accepts only a fully populated core line.
4. `SetInputLine()` accepts only an input line with non-null `input_arena`.
5. `SetRail()` registers KB/mouse or MIDI routes into `RAIL_DB`.
6. `NoteObjectCollector()` converts frame positions to microseconds and fills
   the main or sub note buffers.
7. `Start()` validates prerequisites, sorts notes, creates `Judge_Loop`, and
   launches the worker thread.
8. `End()` stops the loop and clears all cached init state.

## Start Status Rules

- `CORE_LINE_IS_MISSING`: no accepted core line
- `INPUT_LINE_IS_MISSING`: no accepted input line
- `NOTE_OBJECT_IS_MISSING`: no staged note buffer
- `EVENT_RULE_IS_EMPTY`: no hit/miss window
- `INPUT_RULE_IS_EMPTY`: no rail mapping
- `OK`: loop created and started

## Contracts / Invariants

- judge is not valid without both core and input data lines.
- current input-line acceptance requires `input_arena`; MIDI-only lines do not
  satisfy `SetInputLine()`.
- `NOTE` timing is stored in microseconds, converted from 48 kHz frame space.
- `OBJ::Sort()` must happen before loop processing; `Start()` performs that.
- rails are the bridge from device keys or MIDI routes to logical note lanes.
- `End()` clears state aggressively; callers should not reuse old line or rule
  assumptions after teardown.

## Common Change Points

- init and prerequisite rules: `include/judge/Init/`
- rail hashing and lookup: `include/judge/PDJE_RAIL.*`
- note buffering: `include/judge/NoteOBJ/`
- match logic and callbacks: `include/judge/Loop/`

## Verify

- `ctest --test-dir build -R '^unit.judge::' --output-on-failure`
- `ctest --test-dir build -L unit --output-on-failure`

## Known Traps

- rail registration and note collection are independent steps; adding rails
  alone does not create note objects.
- `SetCoreLine()` rejects partially populated lines silently.
- current C ABI wrapper tests exist in source form, but unit wiring for them is
  not yet connected in the active module CMake lists.
