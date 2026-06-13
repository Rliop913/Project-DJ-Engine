# PDJE Judge System

`PDJE_JUDGE::JUDGE` consumes core/input lines, maps rails, stages notes, and
runs use/miss callbacks.

## Surface

| Kind | Paths |
| --- | --- |
| facade | `include/judge/PDJE_Judge.hpp`, `.cpp` |
| init | `include/judge/Init/PDJE_Judge_Init.hpp`, `include/judge/Init/` |
| rails | `include/judge/PDJE_RAIL.hpp`, `PDJE_RAIL.*` |
| loop/notes | `include/judge/Loop/`, `include/judge/NoteOBJ/`, `include/judge/InputParser/` |

## Flow

1. Construct `PDJE_JUDGE::JUDGE`.
2. Fill `judge.inits`: core line, input line, event rule, callbacks, rails,
   and note objects.
3. `SetCoreLine()` requires all core line fields.
4. `SetInputLine()` currently requires non-null `input_arena`.
5. `SetRail()` maps keyboard/mouse or MIDI routes into `RAIL_DB`.
6. `NoteObjectCollector()` converts 48 kHz frame positions to microseconds.
7. `Start()` validates, sorts notes, creates `Judge_Loop`, and starts the
   worker thread.
8. `End()` stops the loop and clears cached init state.

## Start Status

| Status | Meaning |
| --- | --- |
| `CORE_LINE_IS_MISSING` | no accepted core line |
| `INPUT_LINE_IS_MISSING` | no accepted input line |
| `NOTE_OBJECT_IS_MISSING` | no staged note buffer |
| `EVENT_RULE_IS_EMPTY` | no hit/miss window |
| `INPUT_RULE_IS_EMPTY` | no rail mapping |
| `ALREADY_RUNNING` | loop exists; call `End()` before restart |
| `OK` | loop created and started |

## Contracts

- Judge requires accepted core and input data lines.
- MIDI-only input lines do not currently satisfy `SetInputLine()`.
- `Start()` performs required `OBJ::Sort()` before loop processing.
- Rail registration and note collection are independent.
- Do not reuse line/rule assumptions after `End()`.
- C ABI wrapper coverage is tracked in [TEST_MAP.md](TEST_MAP.md).

## Verify

- Use [TEST_MAP.md](TEST_MAP.md): judge module or full unit rows.
