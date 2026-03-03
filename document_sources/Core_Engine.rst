Core_Engine
===========

The Core Engine page documents the current public workflow exposed by `PDJE`.
The engine facade owns the root database handle, constructs `audioPlayer`
instances, exposes the editor subsystem, and forwards the core data line used
by other modules.

Older versions of this manual kept most editor operation details here. The
current docs split responsibilities differently:

- `Core_Engine` covers facade entrypoints and playback control.
- :doc:`Editor_Workflows` covers project setup, mutation, history, render/push,
  and preview playback.
- :doc:`/api/classeditorObject` remains the exact member lookup page.

Public Entry Points
-------------------

.. doxygenenum:: PLAY_MODE
   :project: Project_DJ_Engine

.. doxygenclass:: PDJE
   :project: Project_DJ_Engine

`PLAY_MODE` determines how `InitPlayer()` configures the player:

- `FULL_PRE_RENDER` builds a pre-rendered playback path.
- `HYBRID_RENDER` builds a pre-rendered path and enables manual music / FX
  control panels.
- `FULL_MANUAL_RENDER` constructs the manual player path without loading a
  track into the player constructor.

Typical Playback Flow
---------------------

1. Construct `PDJE` with the root database path.
2. Search for music or tracks through `SearchMusic()` and `SearchTrack()`.
3. Create a player with `InitPlayer()`.
4. Call `Activate()` / `Deactivate()` on the returned `audioPlayer`.
5. Pull the core data line when you need playback state from another module.

Selected facade methods:

.. doxygenfunction:: PDJE::SearchMusic
   :project: Project_DJ_Engine

.. doxygenfunction:: PDJE::SearchTrack
   :project: Project_DJ_Engine

.. doxygenfunction:: PDJE::InitPlayer
   :project: Project_DJ_Engine

.. doxygenfunction:: PDJE::GetPlayerObject
   :project: Project_DJ_Engine

.. doxygenfunction:: PDJE::PullOutDataLine
   :project: Project_DJ_Engine

.. code-block:: c++

   PDJE engine("database/path");

   auto tracks = engine.SearchTrack("example-track");
   if (tracks.empty()) {
       return;
   }

   if (!engine.InitPlayer(PLAY_MODE::HYBRID_RENDER, tracks.front(), 480)) {
       return;
   }

   auto player = engine.GetPlayerObject();
   if (!player) {
       return;
   }

   (void)player->Activate();
   auto line = engine.PullOutDataLine();
   (void)player->Deactivate();
   engine.ResetPlayer();

`PDJE::PullOutDataLine()` returns an empty `PDJE_CORE_DATA_LINE` until a player
exists. Reacquire the line after recreating or resetting the player because the
owned storage can move.

Wrapper Bindings
----------------

The older manual pages also described the wrapper-facing entry points. That
material still matters in the current tree:

- C# and Python bindings are generated only when `PDJE_SWIG_BUILD=ON`.
- The SWIG output mirrors the core/editor facade with names such as `PDJE`,
  `audioPlayer`, `editorObject`, and `PLAY_MODE`.
- The Godot wrapper uses `PDJE_Wrapper` and `PlayerWrapper` instead of the raw
  C++ class names.
- The current SWIG output keeps the legacy `Pannel` spelling for manual-control
  methods such as `GetFXControlPannel()` and `GetMusicControlPannel()`.

Quick start by binding:

.. tab-set-code::

   .. code-block:: c++

      PDJE engine("database/path");
      auto tracks = engine.SearchTrack("example-track");
      if (tracks.empty()) {
          return;
      }

      if (!engine.InitPlayer(PLAY_MODE::HYBRID_RENDER, tracks.front(), 480)) {
          return;
      }

      auto player = engine.GetPlayerObject();
      if (!player) {
          return;
      }

      (void)player->Activate();
      (void)player->Deactivate();
      engine.ResetPlayer();

   .. code-block:: c#

      PDJE engine = new PDJE("database/path");
      TRACK_VEC tracks = engine.SearchTrack("example-track");
      if (tracks.Count == 0) {
          return;
      }

      if (!engine.InitPlayer(PLAY_MODE.HYBRID_RENDER, tracks[0], 480)) {
          return;
      }

      audioPlayer player = engine.GetPlayerObject();
      if (player == null) {
          return;
      }

      player.Activate();
      player.Deactivate();
      engine.ResetPlayer();

   .. code-block:: python

      import pdje_POLYGLOT as pypdje

      engine = pypdje.PDJE("database/path")
      tracks = engine.SearchTrack("example-track")
      if len(tracks) == 0:
          raise RuntimeError("track not found")

      if not engine.InitPlayer(pypdje.HYBRID_RENDER, tracks[0], 480):
          raise RuntimeError("player init failed")

      player = engine.GetPlayerObject()
      if player is None:
          raise RuntimeError("player handle unavailable")

      player.Activate()
      player.Deactivate()
      engine.ResetPlayer()

   .. code-block:: gdscript

      var engine:PDJE_Wrapper = PDJE_Wrapper.new()
      engine.InitEngine("res://database/path")

      var tracks = engine.SearchTrack("example-track")
      if tracks.is_empty():
          return

      if not engine.InitPlayer(PDJE_Wrapper.HYBRID_RENDER, tracks[0], 480):
          return

      var player:PlayerWrapper = engine.GetPlayer()
      if player == null:
          return

      player.Activate()
      player.Deactivate()
      engine.ResetPlayer()

Player Control
--------------

`audioPlayer` is the runtime playback object returned by `InitPlayer()`.

.. doxygenfunction:: audioPlayer::Activate
   :project: Project_DJ_Engine

.. doxygenfunction:: audioPlayer::Deactivate
   :project: Project_DJ_Engine

.. doxygenfunction:: audioPlayer::GetConsumedFrames
   :project: Project_DJ_Engine

.. doxygenfunction:: audioPlayer::GetFXControlPanel
   :project: Project_DJ_Engine

.. doxygenfunction:: audioPlayer::GetMusicControlPanel
   :project: Project_DJ_Engine

Manual control panels are available only when the player was created with manual
features enabled, which in the current code path means `HYBRID_RENDER` or
`FULL_MANUAL_RENDER`.

.. note::

   Native C++ uses `GetFXControlPanel()` / `GetMusicControlPanel()`. The current
   SWIG wrapper output still exposes the same panel handles with the legacy
   method names `GetFXControlPannel()` / `GetMusicControlPannel()`.

FX Control
~~~~~~~~~~

.. doxygenenum:: FXList
   :project: Project_DJ_Engine

.. doxygenfunction:: FXControlPanel::FX_ON_OFF
   :project: Project_DJ_Engine

.. doxygenfunction:: FXControlPanel::GetArgSetter
   :project: Project_DJ_Engine

.. doxygenfunction:: FXControlPanel::checkSomethingOn
   :project: Project_DJ_Engine

.. code-block:: c++

   auto fx = player->GetFXControlPanel();
   if (fx) {
       fx->FX_ON_OFF(FXList::FILTER, true);
       auto args = fx->GetArgSetter(FXList::FILTER);
       args["HLswitch"](0.0);
       args["Filterfreq"](1200.0);
   }

Music Control
~~~~~~~~~~~~~

.. doxygenclass:: MusicControlPanel
   :project: Project_DJ_Engine

.. doxygenfunction:: MusicControlPanel::LoadMusic
   :project: Project_DJ_Engine

.. doxygenfunction:: MusicControlPanel::SetMusic
   :project: Project_DJ_Engine

.. doxygenfunction:: MusicControlPanel::CueMusic
   :project: Project_DJ_Engine

.. doxygenfunction:: MusicControlPanel::UnloadMusic
   :project: Project_DJ_Engine

.. doxygenfunction:: MusicControlPanel::GetLoadedMusicList
   :project: Project_DJ_Engine

.. doxygenfunction:: MusicControlPanel::ChangeBpm
   :project: Project_DJ_Engine

.. code-block:: c++

   auto music_panel = player->GetMusicControlPanel();
   if (music_panel) {
       auto found = engine.SearchMusic("song-title", "composer-name", -1.0);
       if (!found.empty()) {
           (void)music_panel->LoadMusic(*engine.DBROOT, found.front());
           (void)music_panel->SetMusic(found.front().title, true);
       }
   }

Editor Entry Points
-------------------

The editor is a major authoring subsystem behind the same `PDJE` facade. This
page covers the entry sequence only. For the operational workflow around
project-local editing, typed mutations, render/push behavior, and history APIs,
read :doc:`Editor_Workflows`.

If you were using the older "Editor Step-1/2/3/4" manual, the material moved
there is now grouped by operation family:

- project setup and open/reopen flow
- line mutation and typed readback
- history and time-travel operations
- render and push-to-root-db flow
- preview playback through `demoPlayInit()`

.. doxygenfunction:: PDJE::InitEditor
   :project: Project_DJ_Engine

.. doxygenfunction:: PDJE::GetEditorObject
   :project: Project_DJ_Engine

.. doxygenfunction:: PDJE::CloseEditor
   :project: Project_DJ_Engine

.. doxygenfunction:: editorObject::Open
   :project: Project_DJ_Engine

.. doxygenfunction:: editorObject::ConfigNewMusic
   :project: Project_DJ_Engine

.. code-block:: c++

   if (!engine.InitEditor("Author Name", "author@example.com", "project-root")) {
       return;
   }

   auto editor = engine.GetEditorObject();
   if (!editor) {
       return;
   }

   (void)editor->ConfigNewMusic("Song", "Composer", "audio/song.wav", "0");
   engine.CloseEditor();

From this point on, use :doc:`Editor_Workflows` for the authoring flow. Use
:doc:`/api/classeditorObject` when you need exact overloads for `AddLine()`,
`Undo()`, `GetLogWithJSONGraph()`, `render()`, `pushToRootDB()`, or
`demoPlayInit()`.
