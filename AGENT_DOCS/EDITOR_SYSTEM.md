# PDJE Editor System

This page covers the authoring path behind `PDJE::InitEditor()` and
`editorObject`.

## Purpose

- manage project-local editing state
- mutate note, mix, key-value, and music timelines
- expose undo, redo, and commit-travel operations
- render and push authored content back toward the root DB

## Primary Entry Points

- `include/core/MainObjects/editorObject/editorObject.hpp`
- `include/core/MainObjects/editorObject/`
- `include/core/editor/`
- `include/core/editor/TimeLine/TimeLine.hpp`

## Owning Paths

- `include/core/MainObjects/editorObject/`
- `include/core/editor/`
- `include/core/db/Capnp/`

## Lifecycle / Flow

1. `PDJE::InitEditor()` constructs `editorObject`.
2. `editorObject::Open(projectRoot, auth_name, auth_email)` initializes the
   project-local store and editor core.
3. Use typed `AddLine`, `deleteLine`, `Undo`, `Redo`, `Go`, and `getAll`
   operations on note, mix, key-value, and music data.
4. Use `render()` to produce track-facing output, optionally with lint output.
5. Use `pushToRootDB()` to persist track or music changes into the root DB.
6. Use `demoPlayInit()` when the editor path needs preview playback.

Timeline behavior:

- `TimeLine::WriteData()` appends JSON and then saves.
- `Undo()` and `Redo()` move inside the timeline marker/history model.
- `Go()` accepts a single commit OID.
- `GetLogs()` serializes the current graph into `LINE` and `LOGS` arrays.

## Contracts / Invariants

- `editorObject` is the high-level authoring API, not a thin file helper.
- `Go()` is commit-targeted. It does not accept a branch-name plus commit pair.
- history and diff logic live under the editor/timeline path, not the player.
- `render()` and `pushToRootDB()` are different steps; render prepares authored
  output, push persists it.
- `DESTROY_PROJECT()` is the irreversible project teardown entry.

## Common Change Points

- editor facade methods: `include/core/MainObjects/editorObject/`
- timeline, diff, log JSON, linter: `include/core/editor/TimeLine/`,
  `include/core/editor/pdjeLinter/`
- DB translation and serialization: `include/core/db/Capnp/`

## Verify

- `ctest --test-dir ./build -R '^unit.core::core:' --output-on-failure`
- `ctest --test-dir ./build -L unit --output-on-failure`

## Known Traps

- editor workflow and editor data format are different concerns.
- `GetLogWithJSONGraph()` should be interpreted through the current `LINE` and
  `LOGS` schema, not older branch/commit wording.
- playback docs do not fully describe mutation, history, or render/push rules.
