# PDJE Lifecycles

Valid call order for main runtime surfaces.

## Core And Editor

| Surface | Order |
| --- | --- |
| `PDJE` playback | `PDJE(rootDir)` -> `SearchMusic()` / `SearchTrack()` -> `InitPlayer()` -> `PullOutDataLine()` -> `ResetPlayer()` |
| editor branch | `InitEditor(auth_name, auth_email, projectRoot)` -> `GetEditorObject()` -> editor operations -> `CloseEditor()` |

## Input

| State | Call |
| --- | --- |
| `DEAD` | construct `PDJE_Input`; `Init()` is allowed |
| `DEVICE_CONFIG_STATE` | `Config()` is allowed |
| `INPUT_LOOP_READY` | `Run()` is allowed |
| `INPUT_LOOP_RUNNING` | input loop active |
| `DEAD` | `Kill()` returns here; MIDI-only config also returns here after readiness |

Invalid state calls fail.

## Judge

| Step | Call | Requirement |
| --- | --- | --- |
| 1 | construct `JUDGE` | empty init cache |
| 2 | `inits.SetCoreLine()` | fully populated core line |
| 3 | `inits.SetInputLine()` | input line with non-null `input_arena` |
| 4 | `inits.SetEventRule()` | hit/miss window |
| 5 | `inits.SetRail()` | at least one rail |
| 6 | `inits.NoteObjectCollector()` | note staging |
| 7 | `Start()` | validates prerequisites; may return `CORE_LINE_IS_MISSING`, `INPUT_LINE_IS_MISSING`, `NOTE_OBJECT_IS_MISSING`, `EVENT_RULE_IS_EMPTY`, `INPUT_RULE_IS_EMPTY`, `ALREADY_RUNNING`, or `OK` |
| 8 | `End()` | stops loop and clears cached state |

## Wrappers And Teardown

- `CPDJE`, `CPDJE_MODULE_INPUT`, and `CPDJE_MODULE_JUDGE` mirror the native
  ordering rules.
- Explicit C ABI test sources exist, but active unit target wiring does not yet
  include them.
- `ResetPlayer()`, `CloseEditor()`, `PDJE_Input::Kill()`, and
  `PDJE_JUDGE::End()` invalidate their owned handles/lines/state.
