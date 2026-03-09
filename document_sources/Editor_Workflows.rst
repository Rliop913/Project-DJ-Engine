Editor_Workflows
================

The editor is a major subsystem of PDJE. It is reached through the `PDJE`
facade, but it owns its own workflow around project-local state, typed timeline
mutation, render and push behavior, preview playback, and edit-history
operations. This page is the manual bridge between :doc:`Core_Engine`,
:doc:`Editor_Format`, and the generated :doc:`/api/classeditorObject`
reference.

What The Editor Owns
--------------------

The editor path in the current tree is responsible for:

- project-local editing state rooted under the editor project path
- timeline mutation for mix, note, music, and key-value style content
- music metadata authoring and working-set management
- render steps that prepare root-DB-ready payloads
- push steps that persist selected authored results back into the root DB
- history and time-travel operations exposed through `editorObject`

This means editor support is a core part of PDJE, not just an optional helper
around `InitEditor()`.

How The Editor Is Reached
-------------------------

The editor uses the same top-level facade as playback, but it follows a
different lifecycle:

- Construct `PDJE` with the root database path.
- Call `InitEditor()` to prepare the editor path.
- Use `GetEditorObject()` to access the editor subsystem.
- Call `CloseEditor()` when the editor handle is no longer needed.

Playback and editor can coexist behind the same `PDJE` object, but playback and
authoring should be treated as different workflows.

.. doxygenfunction:: PDJE::InitEditor
   :project: Project_DJ_Engine

.. doxygenfunction:: PDJE::GetEditorObject
   :project: Project_DJ_Engine

.. doxygenfunction:: PDJE::CloseEditor
   :project: Project_DJ_Engine

Project Setup
-------------

The public surface exposes two main starting modes.

New project / new authored content:

- Call `InitEditor()` through `PDJE`.
- Acquire the `editorObject`.
- Use `ConfigNewMusic()` when you are creating new music metadata inside the
  editor project.

Open existing project / continue editing:

- Call `InitEditor()` through `PDJE`.
- Acquire the `editorObject`.
- Use `Open()` when you need to reopen a specific editor project path through
  the editor handle.

The header documents that if `PDJE` already initialized the editor path for
you, you do not need to call `Open()` again just to continue the same startup
flow.

The editor also exposes `DESTROY_PROJECT()` for destructive project teardown.
Treat that as a cleanup or reset tool, not part of the normal authoring loop.

.. doxygenfunction:: editorObject::Open
   :project: Project_DJ_Engine

.. doxygenfunction:: editorObject::ConfigNewMusic
   :project: Project_DJ_Engine

.. doxygenfunction:: editorObject::DESTROY_PROJECT
   :project: Project_DJ_Engine

.. code-block:: c++

   PDJE engine("root.db");
   if (!engine.InitEditor("Author Name",
                          "author@example.com",
                          "project-root")) {
       return;
   }

   auto editor = engine.GetEditorObject();
   if (!editor) {
       return;
   }

   (void)editor->ConfigNewMusic("Song",
                                "Composer",
                                "audio/song.wav",
                                "4800");

Mutation Workflow
-----------------

`editorObject` exposes typed mutation operations rather than a single
string-only edit API.

The current public argument families are:

- `EDIT_ARG_MUSIC`
  music-entry oriented edits built around `MusicArgs`
- `EDIT_ARG_MIX`
  timeline mix/event edits built around `MixArgs`
- `EDIT_ARG_NOTE`
  note/chart edits built around `NoteArgs`
- `EDIT_ARG_KEY_VALUE`
  key-value style project data

Operationally:

- `AddLine()` appends or inserts authored data into the project-local editor
  state.
- `deleteLine()` removes matching data from the project-local editor state.
- `getAll()` is the readback-oriented path when you want to enumerate the
  current typed working data.

The working representation is tied to the editor/timeline stack and JSON-backed
state. For on-disk key names and binary translation details, use
:doc:`Editor_Format`.

.. code-block:: c++

   EDIT_ARG_MIX mix;
   mix.type = TypeEnum::FILTER;
   mix.details = DetailEnum::LOW;
   mix.ID = 1;
   mix.first = "1";  // cosine-style interpolation selector in older examples
   mix.second = "5000,1000,2000,3000,4000,5000,5500,6000";
   mix.third = "";
   mix.beat = 0;
   mix.subBeat = 0;
   mix.separate = 1;
   mix.Ebeat = 16;
   mix.EsubBeat = 0;
   mix.Eseparate = 1;

   (void)editor->AddLine(mix);

   std::vector<EDIT_ARG_MIX> found;
   editor->getAll<EDIT_ARG_MIX>(
       [&found](const EDIT_ARG_MIX &row) {
           if (row.beat < 32) {
               found.push_back(row);
           }
       });

   for (const auto &row : found) {
       (void)editor->deleteLine(row, false, false);
   }

Use the four typed families as the main mental model:

- `MusicArgs` changes the BPM timeline or music metadata side of the project.
- `MixArgs` changes runtime playback and effect timeline data.
- `NoteArgs` changes chart and judgment-facing event data.
- `KEY_VALUE` changes project-scoped auxiliary values.

History And Time Travel
-----------------------

The editor subsystem includes public history-oriented operations:

- `Undo()`
- `Redo()`
- `Go()`
- `GetDiff()`
- `GetLogWithJSONGraph()`
- `UpdateLog()`

Conceptually:

- `Undo()` and `Redo()` expose edit-history navigation.
- `Go()` exposes a direct history/time-travel style jump.
- `GetDiff()` exposes semantic added/removed data between two editor commits.
- `GetLogWithJSONGraph()` exposes log data in a graph-oriented JSON form.
- `UpdateLog()` refreshes the editor-side history/log state after you need a
  new view of the commit timeline.

The public API contains multiple overloads and typed specializations for these
operations, especially around the four edit families. Use
:doc:`/api/classeditorObject` when you need exact signatures or overload-level
lookup.

The log JSON described in the older docs is still useful as the mental model:

- `LINE`
  line-head entries with `OID` and `TIME_STAMP`
- `LOGS`
  commit metadata with `OID`, `BACK`, `AUTHOR`, `EMAIL`, and `TIME_STAMP`

.. code-block:: c++

   auto graph = editor->GetLogWithJSONGraph<EDIT_ARG_MIX>();
   auto json = nlohmann::json::parse(graph);
   auto commit_oid = json["LINE"].at(0)["OID"].get<std::string>();

   (void)editor->Go<EDIT_ARG_MIX>(commit_oid);
   (void)editor->Undo<EDIT_ARG_MIX>();
   (void)editor->Redo<EDIT_ARG_MIX>();
   editor->UpdateLog<EDIT_ARG_MIX>();

For diff operations, compare two commit OIDs from the same timeline.

.. code-block:: c++

   editor->UpdateLog<EDIT_ARG_MIX>();
   auto graph = nlohmann::json::parse(
       editor->GetLogWithJSONGraph<EDIT_ARG_MIX>());
   auto old_oid = graph["LOGS"].at(0)["OID"].get<std::string>();
   auto new_oid = graph["LOGS"].at(1)["OID"].get<std::string>();

   auto mix_diff = editor->GetDiff<EDIT_ARG_MIX>(old_oid, new_oid);
   auto music_diff =
       editor->GetDiff<EDIT_ARG_MUSIC>("Song", old_oid, new_oid);

   if (mix_diff.has_value() && !mix_diff->Empty()) {
       for (const auto &added : mix_diff->mixAdded) {
           // consume added MixArgs rows
       }
   }

`GetDiff()` returns `std::optional<PDJE_TIMELINE::TimeLineSemanticDiffResult>`.
Use `has_value()` to guard failure cases such as invalid OIDs or missing music
targets, and then inspect the type-specific added/removed collections:

- `kvAdded` / `kvRemoved`
- `mixAdded` / `mixRemoved`
- `noteAdded` / `noteRemoved`
- `musicBpmAdded` / `musicBpmRemoved`
- `musicMetaAdded` / `musicMetaRemoved`

`EDIT_ARG_MUSIC` uses a separate overload that requires the music name as the
first argument.

Persistence Workflow
--------------------

The current public surface exposes two persistence-oriented steps with distinct
roles:

- `render()`
  transforms project-local editor state into root-DB-ready content and performs
  the linter/render phase for a target track
- `pushToRootDB()`
  writes selected track or music payloads into the root DB after the editor-side
  content is ready

Use this distinction when explaining the workflow:

- render is the conversion and validation step
- push is the persistence and write-back step

`ConfigNewMusic()` belongs here too, because it prepares music metadata that
the render and push stages later consume.

`render()` also produces a lint message output. In the current implementation,
track rendering builds `trackdata` plus related `musdata` payloads from the
project-local JSON and binary state before `pushToRootDB()` writes them to the
root database.

.. doxygenfunction:: editorObject::render
   :project: Project_DJ_Engine

.. doxygenfunction:: editorObject::pushToRootDB(litedb &ROOTDB, const UNSANITIZED &trackTitleToPush)
   :project: Project_DJ_Engine

.. doxygenfunction:: editorObject::pushToRootDB(litedb &ROOTDB, const UNSANITIZED &musicTitle, const UNSANITIZED &musicComposer)
   :project: Project_DJ_Engine

.. code-block:: c++

   std::string lint_message;
   if (!editor->render("track-title", *engine.DBROOT, lint_message)) {
       return;
   }

   (void)editor->pushToRootDB(*engine.DBROOT, "track-title");
   (void)editor->pushToRootDB(*engine.DBROOT, "Song", "Composer");

For the exact JSON keys, binary payloads, and timeline field meanings involved
in those steps, use :doc:`Editor_Format`.

Preview And Validation
----------------------

The editor also exposes a preview bridge:

- `demoPlayInit()`
  prepares a demo-style `audioPlayer` from editor-authored content so you can
  audition the result without treating that preview step as the same thing as a
  root-DB persistence write

Validation and authoring support also span the wider editor tree, especially:

- `include/core/editor/`
- `include/core/editor/pdjeLinter/`
- `include/core/editor/TimeLine/`

.. doxygenfunction:: editorObject::demoPlayInit
   :project: Project_DJ_Engine

.. code-block:: c++

   std::shared_ptr<audioPlayer> preview;
   editor->demoPlayInit(preview, 480, "track-title");
   if (preview) {
       (void)preview->Activate();
       (void)preview->Deactivate();
   }

Wrapper Notes
-------------

The strongest non-C++ wrapper coverage in the current tree still sits on the
core/editor side. Older manuals contained large language matrices for every
editor action. The restored guidance here keeps only the supportable workflow
shape:

- C++ remains the clearest reference for typed editor APIs.
- SWIG bindings are oriented around the core/editor path, but exact wrapper
  helper names can differ from native C++.
- The Godot-facing path exposes its own wrapper objects and helper methods
  rather than mirroring template-heavy C++ exactly.

Use :doc:`Core_Engine` for the wrapper entrypoints and the generated wrapper
API when you need binding-specific method names.

How This Relates To The Older Manual
------------------------------------

The old "Editor Step-1/2/3/4" flow maps onto this page as follows:

- old Step-1, create/manage DB
  now lives in `How The Editor Is Reached` and `Project Setup`
- old Step-2, edit/history control
  now lives in `Mutation Workflow` and `History And Time Travel`
- old Step-3, configure new music, render, push
  now lives in `Persistence Workflow`
- old Step-4, playback edited project
  now lives in `Preview And Validation`

This keeps the old conceptual flow without turning `Core_Engine` back into a
single oversized editor manual.

Common Mistakes
---------------

- Do not treat the editor as only `InitEditor()` plus a couple of helper calls.
- Do not confuse editor JSON working data with root DB payloads.
- Do not assume generated API overloads define the recommended workflow order.
- Do not assume the playback flow alone explains authoring flow.
- Do not treat preview playback as the same thing as render/push persistence.
- Do not treat `Editor_Format` as the full editor manual; it explains data
  representations, not the operational workflow.

Where To Go Next
----------------

- :doc:`Core_Engine`
- :doc:`Editor_Format`
- :doc:`Developer_Onboarding`
- :doc:`/api/classeditorObject`
