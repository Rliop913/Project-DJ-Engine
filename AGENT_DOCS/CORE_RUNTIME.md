# PDJE Core Runtime

Always-built `PDJE` facade for DB search, playback, editor entry, PCM helper,
and core data-line access.

## Surface

| Kind | Paths |
| --- | --- |
| facade | `include/core/interface/PDJE_interface.hpp`, `.cpp` |
| playback | `include/core/MainObjects/audioPlayer/`, `include/core/audioRender/` |
| DB | `include/core/db/` |

## Flows

| Flow | Order |
| --- | --- |
| Playback | `PDJE(rootDir)` opens `DBROOT`; `SearchMusic()` / `SearchTrack()` resolve data; `InitPlayer()` creates `audioPlayer`; `PullOutDataLine()` exposes non-owning state; `ResetPlayer()` releases it. |
| Editor bridge | `InitEditor(auth_name, auth_email, projectRoot)` creates `editorObject`; `GetEditorObject()` returns it; `CloseEditor()` releases it. |
| PCM helper | `GetPCMFromMusData()` uses `musicPath` when present, otherwise searches by title/composer/bpm; decode/search failure returns empty PCM. |

## Contracts

- `DBROOT` must exist before search, delete, or PCM decode work.
- `FULL_PRE_RENDER` and `HYBRID_RENDER` require track data.
- `FULL_MANUAL_RENDER` uses buffer-size-only construction.
- Failed `InitPlayer()` leaves `player` unset.
- `PullOutDataLine()` is null-filled without a player.
- `ResetPlayer()` immediately invalidates the core data-line source.
- Search helpers sanitize title/composer before DB query.

## Change Points

- facade semantics: `include/core/interface/`
- playback branching: `PDJE_interface.cpp`, `audioPlayer/`
- DB behavior: `include/core/db/`
- PCM helper: `GetPCMFromMusData()` and decoder path

## Verify

- Use [TEST_MAP.md](TEST_MAP.md): core PCM, core behavior, or full unit rows.
