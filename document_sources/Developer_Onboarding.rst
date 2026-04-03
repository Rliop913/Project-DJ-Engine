Developer_Onboarding
====================

`Project-DJ-Engine` (`PDJE`) is a soft real-time C++ engine for DJ-style audio
playback, note-chart authoring, low-latency input capture, and rhythm-game
judgment. This page is the shortest path to a correct mental model before you
jump into generated API pages or source files.

What PDJE Is
------------

PDJE is organized around four integration surfaces:

- `PDJE`
  the main facade for playback, editor access, and the core data line.
- `PDJE_Input`
  low-latency keyboard, mouse, and MIDI ingestion.
- `PDJE_JUDGE::JUDGE`
  timing-sensitive note matching built on synchronized input and audio clocks.
- `PDJE_UTIL`
  reusable utility code for status/result transport, DB wrappers, image/WebP
  helpers, and STFT/backend-loader utilities.

Choose Your Starting Path
-------------------------

- If you need playback, player control, or track/music lookup, start with
  :doc:`Core_Engine`.
- If you need to author or edit project content, read :doc:`Core_Engine` for
  the entrypoints, then move to :doc:`Editor_Workflows`.
- If you need keyboard, mouse, or MIDI input capture, read :doc:`Input_Engine`.
- If you are building rhythm-game style timing and scoring, read
  :doc:`Judge_Engine` after :doc:`Input_Engine`.
- If you need shared live state between modules, read :doc:`Data_Lines`.
- If you need editor data formats or timeline serialization details, read
  :doc:`Editor_Format` after :doc:`Editor_Workflows`.
- If you need non-C++ bindings, read :doc:`Core_Engine` first, then use the
  wrapper notes there to distinguish SWIG bindings from Godot-facing wrappers.

How To Read This Project
------------------------

Recommended reading order:

1. Read :doc:`index` for the high-level module overview.
   It also now carries the broader use-case and project-automation context.
2. Read :doc:`Getting Started` for the guide map.
3. Read :doc:`Core_Engine` to understand the main facade and runtime flow.
4. Read :doc:`Editor_Workflows` if your application creates or mutates project
   content.
5. Read :doc:`Input_Engine` if your application touches live input.
6. Read :doc:`Judge_Engine` if your application performs note matching.
7. Read :doc:`Data_Lines` before you poll live state from another module or
   thread.
8. Read :doc:`Editor_Format` if you generate or inspect editor-side data on
   disk.
9. Use :doc:`/api/api_root` only after the workflow docs above.

The hand-written docs explain recommended workflows. The generated API explains
what symbols exist. Those are not the same thing.

Build And Configuration Reality
-------------------------------

Current build facts from the source tree:

- CMake minimum is `3.12`.
- The project requires `C++20`.
- Conan is part of the expected dependency flow through
  `conan_cmakes/conan_toolchain.cmake`.
- `PDJE_DYNAMIC`
  controls static vs shared library builds.
- `PDJE_DEVELOP_INPUT`
  controls whether the input and judge module libraries are built.
- `PDJE_SWIG_BUILD`
  enables the SWIG wrapper targets.
- `PDJE_TEST`
  enables doctest-based unit tests and CTest registration.
- `PDJE_DEV_TEST`
  enables legacy/manual developer test executables.

Practical meaning:

- `PDJE` itself is the always-built core library.
- `PDJE_MODULE_INPUT` and `PDJE_MODULE_JUDGE` exist only when
  `PDJE_DEVELOP_INPUT=ON`.
- On Windows, enabling the input path also enables the subprocess executable
  used by the Raw Input transport.
- The current tree carries production input implementations for Linux and
  Windows.
- macOS input is not implemented in-tree.

Project Mental Model
--------------------

- **Core runtime facade**
  lives under `include/core/` and exposes the main public entrypoint
  `include/core/interface/PDJE_interface.hpp`.
- **Editor and authoring subsystem**
  also lives under `include/core/`, but spans both
  `include/core/MainObjects/editorObject/` and `include/core/editor/`.
  It is a major part of the project, not just a helper around `InitEditor()`.
- **Input**
  lives under `include/input/` and exposes `include/input/PDJE_Input.hpp`.
- **Judge**
  lives under `include/judge/` and exposes `include/judge/PDJE_Judge.hpp`.
- **Utility**
  lives under `include/util/` and exposes `include/util/PDJE_Util.hpp`, with
  direct-include extensions for image/WebP and STFT helper headers.
- **Global shared infrastructure**
  lives under `include/global/` and provides data lines, clocks, crypto, RAII,
  and logging helpers used across modules.

The tree also contains older or legacy areas such as `trashbin` and
`DEPRECATE`. Do not assume those are active build paths just because they appear
in generated API pages.

Useful Domain Knowledge
-----------------------

DJ and playback terms:

- `music`
  usually means a concrete audio asset plus metadata.
- `track`
  usually means a higher-level authored object that can reference mix and note
  structures plus music metadata.
- `deck`
  means a playback slot or source lane inside a DJ-style workflow.
- `cue`
  means a specific seek/play start position.
- `BPM`
  is beats per minute and appears in both playback and chart timing contexts.
- `FX`
  means runtime effect processing such as filter, EQ, echo, or roll.

Rhythm-game terms:

- `rail`
  is the gameplay lane or logical input destination a note belongs to.
- `note`
  is an authored gameplay event with timing and rail metadata.
- `judgment window`
  means the timing tolerance around the expected note time.

Audio timing terms:

- `frame`
  means one sample frame at the current sample rate.
- `sample rate`
  controls how frame positions map to time.
- `callback cadence`
  means how often the audio backend asks for more audio.
- `microsecond`
  is the fine-grained timing unit used for synchronized input/audio judgment.

Input terms:

- `backend`
  means the platform-specific input implementation that was selected.
- `MIDI port`
  means a live hardware or virtual MIDI input endpoint.
- `input_arena`
  means the keyboard/mouse transport exposed by `PDJE_INPUT_DATA_LINE`.

Storage and authoring terms:

- `root DB`
  means the database root used by the core/editor path.
- `project-local editor state`
  means the working timeline and metadata state managed by `editorObject`
  before content is pushed back into the root DB.
- `SQLite`
  is used in the relational storage layer.
- `RocksDB`
  is used in the key-value storage layer.
- `Cap'n Proto`
  is used for binary translation/serialization in the core DB pipeline.
- `editor JSON`
  means the timeline-oriented JSON representation described in
  :doc:`Editor_Format`.

First Real Workflows
--------------------

Playback-first integration flow:

1. Build the project with the desired options.
2. Construct `PDJE` with the root database path.
3. Search for music or track objects through `SearchMusic()` or `SearchTrack()`.
4. Create a player with `InitPlayer()`.
5. Pull a `PDJE_CORE_DATA_LINE` if another module needs live playback state.
6. Add `PDJE_Input` only if you need live keyboard, mouse, or MIDI capture.
7. Add `PDJE_JUDGE::JUDGE` only if you need synchronized note matching.

Editor-first authoring flow:

1. Build the project with the desired options.
2. Construct `PDJE` with the root database path.
3. Initialize the editor through `InitEditor()`.
4. Acquire the `editorObject` through `GetEditorObject()`.
5. Create or open project content, then mutate it through the editor workflow.
6. Render or push the result back toward the root DB.
7. Optionally use preview playback through the editor/player bridge.

Use these pages for the detailed workflows:

- :doc:`Core_Engine`
- :doc:`Editor_Workflows`
- :doc:`Input_Engine`
- :doc:`Judge_Engine`
- :doc:`Data_Lines`

Contributor Orientation
-----------------------

If you are modifying the repository rather than only embedding the engine, start
here:

- Main public facade:
  `include/core/interface/PDJE_interface.hpp`
- Playback implementation:
  `include/core/MainObjects/audioPlayer/`
- Editor facade object:
  `include/core/MainObjects/editorObject/`
- Editor timeline, linter, and serialization core:
  `include/core/editor/`
- Input implementation:
  `include/input/`
- Judge implementation:
  `include/judge/`
- Utility layer:
  `include/util/`
- Shared infrastructure:
  `include/global/`
- Tests:
  `include/tests/`
- Manual docs:
  `document_sources/*.rst`
- Generated API:
  `document_sources/api/`

Important project quirks:

- In this repository, `include/` is not headers-only. It also contains a large
  amount of implementation `.cpp` code.
- Editor history, timeline mutation, and render/push logic are not fully
  described by the top-level `PDJE` header. Read the editor-specific paths and
  manual docs before changing that subsystem.

Common Mistakes
---------------

- Do not assume the generated API pages define the recommended integration
  order.
- Do not assume input and judge are always built. That depends on
  `PDJE_DEVELOP_INPUT`.
- Do not assume SWIG bindings cover every native C++ subsystem. The current
  SWIG output is strongest on the core/editor path.
- Do not assume editor support is only `InitEditor()` plus one or two helper
  calls. `editorObject` is a real subsystem with mutation, history, and
  persistence workflows.
- Do not assume :doc:`Editor_Format` explains the full editor operation model.
  It explains data shapes, not the complete editing lifecycle.
- Do not assume legacy or deprecated paths are part of the active runtime.
- Do not assume macOS input support exists in the current source tree.
- Do not mix Godot wrapper names with native C++ names when reading examples.

Where To Go Next
----------------

- :doc:`Core_Engine`
- :doc:`Editor_Workflows`
- :doc:`Input_Engine`
- :doc:`Judge_Engine`
- :doc:`Data_Lines`
- :doc:`Editor_Format`
- :doc:`FX_ARGS`
- :doc:`Util_Engine`
- :doc:`PDJE_For_AI_Agents`
- :doc:`/api/api_root`
