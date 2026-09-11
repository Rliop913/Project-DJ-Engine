# PDJE Architecture

Project-DJ-Engine is a modular C++20 engine for authored DJ/rhythm content. It
separates database-backed playback and editing, device input, judgment, and
reusable utility work while exposing native and C ABI entrypoints.

## Runtime Data Flow

~~~text
authored music + track data
            |
            v
         PDJE core -----> playback / editor
            |
            | borrowed core data line
            v
          judge <------- PDJE_Input
            ^             borrowed input data line
            |
     rules, rails, notes, callbacks
~~~

The core owns playback/editor state and publishes timing/audio views. Input
owns device and MIDI state and publishes event views. Judge consumes both
views together with authored rules and notes; it does not extend the producers'
lifetimes. `PDJE_UTIL` supports the modules but is outside this ownership chain.

## Public Entrypoints

| Surface | Declaration | Design role |
| --- | --- | --- |
| `PDJE` | `include/core/interface/PDJE_interface.hpp` | database, search, playback, editor, and core data line facade |
| `PDJE_Input` | `include/input/PDJE_Input.hpp` | device/MIDI configuration, capture, and input data line facade |
| `PDJE_JUDGE::JUDGE` | `include/judge/PDJE_Judge.hpp` | rule, rail, note, and judge-loop facade |
| `PDJE_UTIL` | `include/util/PDJE_Util.hpp` and public leaf headers under `include/util/` | reusable database, inference, signal, image, text, and scalar facilities |
| Core C ABI | `include/core/interface/CPDJE_interface.h` | opaque core, player, and editor handles and snapshots |
| Input C ABI | `include/input/CPDJE_Input.h` | input lifecycle, device views, and event snapshots |
| Judge C ABI | `include/judge/CPDJE_Judge.h` | judge configuration, lifecycle, and callbacks |

`PDJE_Util.hpp` intentionally covers only the common utility entrypoints.
STFT and image/WebP clients include their public leaf headers directly.

Use [CHANGE_MAP.md](CHANGE_MAP.md) for owning paths and build wiring, and
[RUNTIME_CONTRACTS.md](RUNTIME_CONTRACTS.md) for cross-module lifetimes.
