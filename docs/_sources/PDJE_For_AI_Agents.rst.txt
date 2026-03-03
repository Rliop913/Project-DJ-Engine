PDJE_For_AI_Agents
==================

This document is for AI agents that need to read `Project-DJ-Engine` (`PDJE`)
and explain it accurately to users. Its purpose is to reduce hallucination,
stale assumptions, and terminology drift when answering questions about the
project.

Purpose
-------

Use this page when an AI agent needs to answer:

- what PDJE is
- how its main modules fit together
- how users should build or approach it
- what wrappers exist
- how playback and editor workflows differ
- what the current source tree does and does not support

This is not:

- a generated API reference
- a repo-editing workflow guide
- a guide for using an AI feature inside PDJE

Project Facts Safe To State
---------------------------

The following claims are safe to state directly when answering users, because
they are backed by the current source tree and hand-written docs:

- PDJE is a soft real-time C++ engine for DJ-style playback, editor workflows,
  low-latency input, and rhythm-game judgment.
- The main public surfaces are `PDJE`, `PDJE_Input`,
  `PDJE_JUDGE::JUDGE`, and `PDJE_UTIL`.
- The `PDJE` surface includes a substantial editor/authoring path behind
  `InitEditor()` and `editorObject`, not just playback startup.
- The current tree includes project-local editing, mutation-history APIs, and
  root-DB persistence operations for authored content.
- The repository keeps most source code under `include/`, including many
  implementation `.cpp` files.
- The documentation pipeline uses Doxygen plus Sphinx.
- The main human workflow docs live under `document_sources/*.rst`.
- The landing page `index.rst` now carries project overview, use-case framing,
  and repository automation context in addition to the guide hub.
- The project uses CMake and Conan in the expected build flow.
- SWIG bindings exist in-tree for the core/editor-oriented surface.
- The current tree documents Linux and Windows input paths; macOS input is not
  implemented in-tree.

Claims That Must Be Qualified Or Verified
-----------------------------------------

An AI agent should qualify or verify these before stating them as facts:

- the latest release, package, or prebuilt status
- current state of external wrapper/plugin repositories
- performance claims such as latency numbers or throughput
- whether a legacy path is still part of the active build
- whether a generated API symbol is part of the recommended public workflow
- exact semantics of overload-heavy editor history APIs beyond what the current
  hand-written docs state
- platform support claims not directly backed by the current tree

When uncertain, use wording such as:

- "based on the current source tree"
- "the hand-written docs currently describe"
- "the generated API shows the symbol, but that does not necessarily mean it is
  part of the recommended integration path"

Canonical Terminology
---------------------

- `music`
  usually refers to an audio asset plus metadata.
- `track`
  usually refers to a higher-level authored object that can contain mix and note
  structures.
- `mix`
  refers to authored playback/effect timeline data, not the same thing as a
  `note`.
- `note`
  refers to rhythm-game chart/judgment data.
- `playback flow`
  means the runtime path around `SearchTrack()`, `InitPlayer()`, player control,
  and the core data line.
- `editor workflow`
  means the authoring path around `InitEditor()`, `GetEditorObject()`, mutation,
  render/push, and history operations.
- `project-local editor state`
  means the working editor data managed before content is pushed back into the
  root DB.
- `root DB`
  means the database-backed store used by the core/editor facade.
- `JSON working files`
  means the human-editable timeline-oriented editor representation.
- `binary/root payloads`
  means the serialized payloads written into `trackdata` and `musdata`.
- `rail`
  is the gameplay lane or logical input destination.
- `data line`
  means a small non-owning live-state bridge struct such as
  `PDJE_CORE_DATA_LINE` or `PDJE_INPUT_DATA_LINE`.
- `FULL_PRE_RENDER`, `HYBRID_RENDER`, `FULL_MANUAL_RENDER`
  are the current playback mode names exposed through `PLAY_MODE`.
- `SWIG bindings`
  refers to the in-tree Python and C# wrapper outputs.
- `Godot wrapper`
  refers to the older Godot-facing wrapper path and naming style such as
  `PDJE_Wrapper`, `PDJE_Input_Module`, and `PDJE_Judge_Module`.

Architecture Cheat Sheet
------------------------

- User wants to play a track:
  module `PDJE`
  doc :doc:`Core_Engine`
  entrypoint `include/core/interface/PDJE_interface.hpp`
- User wants to author or edit track/project content:
  module `PDJE` / `editorObject`
  doc :doc:`Editor_Workflows`
  entrypoints `include/core/interface/PDJE_interface.hpp` and
  `include/core/MainObjects/editorObject/editorObject.hpp`
- User wants low-latency input:
  module `PDJE_Input`
  doc :doc:`Input_Engine`
  entrypoint `include/input/PDJE_Input.hpp`
- User wants rhythm-game judgment:
  module `PDJE_JUDGE::JUDGE`
  doc :doc:`Judge_Engine`
  entrypoint `include/judge/PDJE_Judge.hpp`
- User wants utility/helper APIs:
  module `PDJE_UTIL`
  doc :doc:`Util_Engine`
  entrypoint `include/util/PDJE_Util.hpp`
- User wants editor data format details:
  doc :doc:`Editor_Format`
- User wants live shared state details:
  doc :doc:`Data_Lines`
- User wants exact effect argument names:
  doc :doc:`FX_ARGS`

How To Answer Common User Questions
-----------------------------------

How do I build PDJE?
~~~~~~~~~~~~~~~~~~~~

- Start from the build flow described in :doc:`index` and
  :doc:`Developer_Onboarding`.
- Mention CMake, Conan, and the `BuildInitwithConan` helper scripts.
- Mention the important options only if they affect the user question:
  `PDJE_DYNAMIC`, `PDJE_DEVELOP_INPUT`, `PDJE_SWIG_BUILD`, `PDJE_TEST`,
  `PDJE_DEV_TEST`.

How do I start playback?
~~~~~~~~~~~~~~~~~~~~~~~~

- Route to :doc:`Core_Engine`.
- Refer to the `PDJE` facade, `SearchTrack()` / `SearchMusic()`,
  `InitPlayer()`, `GetPlayerObject()`, and `PullOutDataLine()`.
- Use the source anchor `include/core/interface/PDJE_interface.hpp`.

How do I author or edit a project?
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- Route first to :doc:`Editor_Workflows`.
- Explain that the editor path starts from `PDJE::InitEditor()` and
  `GetEditorObject()`, then continues into `editorObject` operations such as
  project creation/open, typed mutation, render/push, and history handling.
- Route to :doc:`Editor_Format` only for JSON and binary data shapes.
- Route to :doc:`/api/classeditorObject` when the user needs exact overloads or
  member names.

How does the editor/project storage work?
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- Route to :doc:`Editor_Workflows` and :doc:`Editor_Format`.
- Explain that the editor path sits behind `PDJE`, while storage details span
  project-local state, DB root, timeline JSON, and Cap'n Proto translators.
- Use `include/core/MainObjects/editorObject/`, `include/core/db/`, and
  `include/core/editor/` as the primary source areas.

How do input and judge relate?
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- Route to :doc:`Input_Engine`, :doc:`Judge_Engine`, and :doc:`Data_Lines`.
- Explain that judge consumes synchronized core and input data lines.
- Mention that the tested path still expects a non-null `input_arena` when the
  judge input line is set.

Which wrappers exist?
~~~~~~~~~~~~~~~~~~~~~

- State that in-tree SWIG bindings exist for Python and C# on the core/editor
  side.
- State that older wrapper-oriented docs also reference the Godot-facing
  wrapper path.
- Do not claim that input and judge are fully exposed by SWIG unless verified in
  current wrapper outputs.

Where do FX argument names live?
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- Route to :doc:`FX_ARGS`.
- If needed, mention that the current authoritative names come from
  `include/core/audioRender/ManualMix/ManualFausts/*.hpp`.

Where do editor/data format details live?
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

- Route to :doc:`Editor_Format`.
- Use it for both key names and the restored semantic explanations around beat
  tuples, `firstBeat`, and `EightPointValues`.
- Mention that current JSON/editor terminology is source-backed by
  `jsonWrapper.hpp` and related editor argument types.
- If the user is really asking about lifecycle or mutation order rather than
  file shapes, route back to :doc:`Editor_Workflows`.

Anti-Hallucination Rules
------------------------

- Do not invent SWIG wrapper coverage for `PDJE_Input` or `PDJE_JUDGE::JUDGE`.
- Do not describe the utility layer as roadmap-only; the current tree contains
  active `PDJE_UTIL` code and CMake targets.
- Do not describe editor as only `InitEditor()` plus `render()`. The public
  surface also includes mutation, history, preview, and push paths.
- Do not answer authoring, history, or time-travel questions from playback docs
  alone.
- Do not merge JSON format details with persistence semantics unless the source
  or hand-written docs support that exact claim.
- Do not describe exact `Undo()`, `Redo()`, or `Go()` semantics from memory if
  you only see generated symbols and not a source-backed workflow explanation.
- Do not treat `trashbin`, `DEPRECATE`, or older wrapper names as the default
  recommended runtime path.
- Do not merge Godot wrapper names into native C++ API explanations.
- Do not assume generated API visibility means recommended workflow status.
- Do not claim macOS input support exists in-tree.
- Do not answer platform/release/package questions as timeless facts if they
  depend on external repos or current distribution state.

Known Confusion Traps
---------------------

- `Panel` vs `Pannel`
  native docs use `Panel`, while current SWIG output still exposes some legacy
  `Pannel` spellings.
- `SWIG bindings` vs `Godot wrapper`
  these are different integration paths and should not be described as one
  unified wrapper surface.
- `editor workflow` vs `editor data format`
  `Editor_Workflows` explains how the editor is used, while `Editor_Format`
  explains the shapes of working and serialized data.
- `facade entrypoint` vs `editor subsystem`
  `PDJE` is the public entry, but the deeper editor operations live on
  `editorObject`.
- `preview playback` vs `render/push`
  demo playback is not the same thing as persisting authored content back into
  the root DB.
- `include/`
  is not headers-only in this repository; it also contains many implementations.
- generated API pages
  include tests and legacy symbols because Doxygen scans broadly.
- external or older built docs
  may preserve examples that no longer match current recommended workflow.

Answer Skeletons
----------------

Build question:

- "Based on the current source tree, PDJE uses CMake plus Conan. Start with the
  build flow in the top-level docs, then choose `PDJE_DEVELOP_INPUT` and
  `PDJE_SWIG_BUILD` only if you need input/judge modules or wrapper generation."

Wrapper question:

- "PDJE currently has in-tree SWIG outputs for Python and C# on the
  core/editor-oriented surface. The older docs also reference a Godot-facing
  wrapper path, which is a separate integration style."

macOS input question:

- "In the current source tree, Linux and Windows input paths are implemented.
  macOS input is not implemented in-tree."

MIDI question:

- "Use the input module path documented in `Input_Engine`. MIDI devices are
  discovered through `PDJE_Input`, and the live MIDI stream is exposed through
  `PDJE_INPUT_DATA_LINE::midi_datas`."

How do I edit tracks or chart data?

- "Use the editor path behind `PDJE`. Start with `PDJE::InitEditor()` and
  `GetEditorObject()`, then follow `Editor_Workflows` for project creation/open,
  mutation, render/push, and history steps. Use `Editor_Format` for file/data
  shapes and the generated `editorObject` API only when you need exact method
  overloads."

Where should I start reading?

- "Start with `Developer_Onboarding`, then `Core_Engine`. If your use case
  includes authoring or project mutation, read `Editor_Workflows` before moving
  on to generated API pages."

When The Agent Should Be Cautious
---------------------------------

Prefer cautious wording when:

- a claim depends on external repositories or current releases
- a symbol appears only in generated API pages
- the path involves older wrapper names or deprecated directories
- a user asks about platform support beyond Linux/Windows input
- a user asks about performance or timing guarantees not explicitly documented
- a user asks about history or time-travel semantics
- a user asks about overload-specific editor behavior
- a user asks what stays project-local versus what is pushed to the root DB

Prefer routing users to these sources:

- :doc:`Developer_Onboarding`
- :doc:`Core_Engine`
- :doc:`Editor_Workflows`
- :doc:`Input_Engine`
- :doc:`Judge_Engine`
- :doc:`Data_Lines`
- :doc:`Editor_Format`
- :doc:`FX_ARGS`
- :doc:`/api/api_root`

When necessary, point to source entrypoints directly:

- `include/core/interface/PDJE_interface.hpp`
- `include/core/MainObjects/editorObject/editorObject.hpp`
- `include/input/PDJE_Input.hpp`
- `include/judge/PDJE_Judge.hpp`
- `include/util/PDJE_Util.hpp`
