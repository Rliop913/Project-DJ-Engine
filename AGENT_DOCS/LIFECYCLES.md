# PDJE Lifecycles

This page fixes the valid call order for the main runtime surfaces.

## PDJE Core Facade

| Step | Call | Result |
| --- | --- | --- |
| 1 | `PDJE(rootDir)` | opens `DBROOT` |
| 2 | `SearchMusic()` / `SearchTrack()` | resolves authored data |
| 3 | `InitPlayer()` | creates `player` |
| 4 | `PullOutDataLine()` | exposes core live-state pointers |
| 5 | `ResetPlayer()` | drops `player` ownership |

Editor branch:

| Step | Call | Result |
| --- | --- | --- |
| 1 | `InitEditor(auth_name, auth_email, projectRoot)` | creates `editor` |
| 2 | `GetEditorObject()` | returns the handler |
| 3 | editor operations | mutate, render, push, diff, history |
| 4 | `CloseEditor()` | drops `editor` ownership |

## PDJE_Input

| Step | State | Call |
| --- | --- | --- |
| 1 | `DEAD` | construct `PDJE_Input` |
| 2 | `DEVICE_CONFIG_STATE` | `Init()` |
| 3 | `INPUT_LOOP_READY` | `Config()` succeeds |
| 4 | `INPUT_LOOP_RUNNING` | `Run()` |
| 5 | `DEAD` | `Kill()` |

Failure rules:

- `Init()` from any state other than `DEAD` fails.
- `Config()` from any state other than `DEVICE_CONFIG_STATE` fails.
- `Run()` from any state other than `INPUT_LOOP_READY` fails.
- MIDI-only config reaches `INPUT_LOOP_READY` and then returns through
  `Kill()`, ending back at `DEAD`.

## PDJE_JUDGE::JUDGE

| Step | Call | Requirement |
| --- | --- | --- |
| 1 | construct `JUDGE` | empty init cache |
| 2 | `inits.SetCoreLine()` | fully populated core line |
| 3 | `inits.SetInputLine()` | input line with non-null `input_arena` |
| 4 | `inits.SetEventRule()` | hit/miss window |
| 5 | `inits.SetRail()` | at least one rail |
| 6 | `inits.NoteObjectCollector()` | note staging |
| 7 | `Start()` | validates all prerequisites |
| 8 | `End()` | stops loop and clears cached state |

`Start()` can return:

- `CORE_LINE_IS_MISSING`
- `INPUT_LINE_IS_MISSING`
- `NOTE_OBJECT_IS_MISSING`
- `EVENT_RULE_IS_EMPTY`
- `INPUT_RULE_IS_EMPTY`
- `OK`

## Wrapper Lifecycles

- `CPDJE`, `CPDJE_MODULE_INPUT`, and `CPDJE_MODULE_JUDGE` mirror the same core,
  input, and judge ordering rules.
- current explicit C ABI test sources exist, but active unit target wiring does
  not yet include them.

## Teardown Rules

- `ResetPlayer()` invalidates core data-line ownership.
- `CloseEditor()` invalidates the editor handle.
- `PDJE_Input::Kill()` always resets flags, contexts, and backend holders.
- `PDJE_JUDGE::End()` clears core line, input line, note objects, event rule,
  and rails.
