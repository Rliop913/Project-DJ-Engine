# PDJE Core Runtime

This page covers the always-built core facade behind `PDJE`.

## Purpose

- open the root DB
- search music and track metadata
- initialize playback
- expose live playback state through the core data line
- initialize the editor bridge

## Primary Entry Points

- `include/core/interface/PDJE_interface.hpp`
- `include/core/interface/PDJE_interface.cpp`
- `include/core/MainObjects/audioPlayer/`
- `include/core/db/`

## Owning Paths

- `include/core/interface/`
- `include/core/MainObjects/audioPlayer/`
- `include/core/db/`
- `include/core/audioRender/`

## Lifecycle / Flow

1. Construct `PDJE(rootDir)`.
2. The constructor creates `DBROOT` and opens the root DB.
3. Use `SearchMusic()` or `SearchTrack()` to resolve authored data.
4. Call `InitPlayer()` with a `PLAY_MODE` and buffer size.
5. Read live playback pointers through `PullOutDataLine()` if another module
   needs synchronized state.
6. When playback ownership is no longer needed, call `ResetPlayer()`.

Editor bridge flow:

1. Call `InitEditor(auth_name, auth_email, projectRoot)`.
2. `PDJE` creates an `editorObject` and forwards to `editorObject::Open()`.
3. Access the handler through `GetEditorObject()`.
4. Use `CloseEditor()` to release it.

PCM helper flow:

1. `GetPCMFromMusData()` uses `musicPath` directly when available.
2. If `musicPath` is empty, it searches the DB using title/composer/bpm clues.
3. Decoder init failure, empty search results, or missing `musicPath` return an
   empty PCM vector.

## Contracts / Invariants

- `PDJE` is the root facade; playback and editor both branch from it.
- `DBROOT` must exist before search, delete, or PCM decode operations can work.
- `InitPlayer()` returns `false` on construction failure and leaves `player`
  unset.
- `FULL_PRE_RENDER` and `HYBRID_RENDER` require track data.
- `FULL_MANUAL_RENDER` uses the buffer-size-only constructor and does not need
  a resolved track.
- `PullOutDataLine()` returns a null-filled line when no player exists.
- `ResetPlayer()` and `CloseEditor()` are ownership release operations only.

## Common Change Points

- facade semantics: `include/core/interface/`
- playback mode branching: `PDJE_interface.cpp`, `audioPlayer/`
- DB search/delete behavior: `include/core/db/`
- PCM decode helper behavior: `GetPCMFromMusData()` and decoder path

## Verify

- `ctest --test-dir build -R '^unit.core::PDJE GetPCMFromMusData' --output-on-failure`
- `ctest --test-dir build -R '^unit.core::core:' --output-on-failure`
- `ctest --test-dir build -L unit --output-on-failure`

## Known Traps

- `ResetPlayer()` invalidates the core data line source immediately.
- search helpers sanitize title and composer before querying the DB.
- `GetPCMFromMusData()` is not a generic file decode helper; it still depends
  on a valid `DBROOT` and engine decoder path.
