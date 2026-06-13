# PDJE Editor System

`editorObject` is the authoring API behind `PDJE::InitEditor()`.

## Surface

| Kind | Paths |
| --- | --- |
| facade | `include/core/MainObjects/editorObject/editorObject.hpp`, `editorObject/` |
| timeline/diff/lint | `include/core/editor/`, `include/core/editor/TimeLine/TimeLine.hpp` |
| serialization | `include/core/db/Capnp/` |

## Flow

1. `PDJE::InitEditor()` constructs `editorObject`.
2. `editorObject::Open(projectRoot, auth_name, auth_email)` initializes the
   project-local store and editor core.
3. Typed `AddLine`, `deleteLine`, `Undo`, `Redo`, `Go`, and `getAll` operations
   mutate note, mix, key-value, and music data.
4. `render()` prepares track-facing output; `pushToRootDB()` persists it.
5. `demoPlayInit()` initializes preview playback when needed.

## Contracts

- `editorObject` is not a thin file helper.
- `TimeLine::WriteData()` appends JSON and saves.
- `Undo()` and `Redo()` move inside marker/history state.
- `Go()` accepts a single commit OID, not branch-name plus commit.
- `GetLogs()` serializes the current graph into `LINE` and `LOGS`.
- `DESTROY_PROJECT()` is irreversible.

## Verify

- Use [TEST_MAP.md](TEST_MAP.md): core behavior or full unit rows.
