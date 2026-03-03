Getting Started
===============

This section collects the hand-written guides that sit above the generated API
reference. Use these pages first when you want the current engine layout,
expected control flow, and high-level integration notes.

The landing page :doc:`index` now also carries the broader project overview,
use-case framing, and repository automation context that sit above these
workflow guides.

Documentation Map
-----------------

- :doc:`Developer_Onboarding` gives new developers an integration-first map of
  the project, build reality, useful domain terms, and source-tree orientation.
- :doc:`Core_Engine` covers the main `PDJE` facade, playback lifecycle, and
  editor entry points.
- :doc:`Editor_Workflows` covers project authoring, typed mutations, render and
  push behavior, preview playback, and history-oriented editor operations.
- :doc:`Input_Engine` covers keyboard, mouse, and MIDI ingestion through
  `PDJE_Input`.
- :doc:`Judge_Engine` covers `PDJE_JUDGE::JUDGE`, `Judge_Init`, and the
  note-matching pipeline.
- :doc:`Util_Engine` covers the header-first `PDJE_UTIL` layer and its common,
  database, and function helpers.
- :doc:`Data_Lines` explains the zero-copy data-line structs shared between
  modules.
- :doc:`Editor_Format` describes the editor-facing JSON and binary data shapes.
- :doc:`FX_ARGS` lists the current runtime FX argument keys exposed by manual
  control panels.
- :doc:`PDJE_For_AI_Agents` grounds AI agents that need to explain PDJE to
  users accurately.

Recommended Reading Order
-------------------------

1. Read :doc:`Developer_Onboarding` if you are new to the project.
2. Read :doc:`Core_Engine` if you are embedding the engine from an application.
3. Read :doc:`Editor_Workflows` if you create or mutate project content.
4. Read :doc:`Input_Engine` and :doc:`Judge_Engine` together if you are
   building rhythm-game style gameplay.
5. Read :doc:`Data_Lines` before polling playback or input state from another
   thread.
6. Read :doc:`Editor_Format` if you are generating or inspecting editor data on
   disk.
7. Read :doc:`PDJE_For_AI_Agents` when the consumer is an AI system that must
   explain PDJE accurately.
8. Use :doc:`/api/api_root` for symbol-by-symbol API lookup after the workflow
   docs above.

Binding Notes
-------------

- `Core_Engine` restores the older wrapper-oriented guidance for the C#,
  Python, and Godot-facing playback/editor path.
- `Input_Engine` and `Judge_Engine` keep the Godot wrapper flow documented and
  also call out that the in-tree SWIG outputs do not currently expose those
  modules as first-class bindings.
- `Data_Lines` explains which live structs are directly readable in native C++
  and which wrapper paths only expose opaque handles or higher-level helper
  objects.

.. toctree::
   :maxdepth: 1
   :caption: Guides

   Developer_Onboarding
   Core_Engine
   Editor_Workflows
   Input_Engine
   Judge_Engine
   Util_Engine
   Data_Lines
   Editor_Format
   FX_ARGS
   PDJE_For_AI_Agents
