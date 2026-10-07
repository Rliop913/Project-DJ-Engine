# PDJE Runtime Contracts

This document owns caller-visible lifecycle, ownership, state, and ABI
constraints. Re-check the listed declarations and implementations when changing
any contract.

## Borrowed Data Lines

| View | Producer | Native judge acceptance | C ABI judge attachment | Invalidation |
| --- | --- | --- | --- | --- |
| `PDJE_CORE_DATA_LINE` | `PDJE::PullOutDataLine()` | all four pointers are required | `syncD` is required; cursor and pre-render pointers are optional | player replacement, `ResetPlayer()`, or producer destruction |
| `PDJE_INPUT_DATA_LINE` | `PDJE_Input::PullOutDataLine()` | either `input_arena` or `midi_datas` is required | either input or MIDI pointer can establish attachment | `Kill()`, startup failure, or producer destruction |

The declarations live in `include/global/DataLines/`. Data lines do not own
their pointees. A null-filled line is a valid absence signal, and every consumer
must validate the members it uses.

## Core And Editor

Source anchors: `include/core/interface/PDJE_interface.hpp`,
`include/core/interface/PDJE_interface.cpp`, and
`include/core/MainObjects/editorObject/`.

| Contract | Caller impact |
| --- | --- |
| `DBROOT` is public, while search and delete operations assume it is non-null. | Preserve the root for facade operations; only `GetPCMFromMusData()` currently rejects a null root explicitly. |
| Failed `InitPlayer()` does not guarantee that an existing player was cleared. | A `false` result may leave the previous player and its view installed. |
| An unsupported player mode performs no construction, then reports whether any player is installed. | With an existing player, an unsupported mode can return `true` without replacing it. |
| Replacing or resetting a player releases the facade reference. | Treat every previously pulled core data line as invalid. |
| Failed `InitEditor()` can leave the newly allocated editor installed; `CloseEditor()` releases it. | Do not infer editor absence from the boolean result alone. |
| `editorObject::Go<T>()` consumes a commit OID, and `DESTROY_PROJECT()` is irreversible. | Validate the target and user intent before destructive project operations. |

## Input State

Source anchors: `include/input/PDJE_Input.cpp` and
`include/input/PDJE_Input_StateLogic.hpp`.

| Operation | Required state | Success / caller-visible edge |
| --- | --- | --- |
| `Init()` | `DEAD` | enters `DEVICE_CONFIG_STATE` |
| `GetDevs()` / `GetMIDIDevs()` | initialized and not torn down for discovery | return empty when dead; `GetDevs()` lazily initializes the keyboard/mouse backend |
| `Config()` | `DEVICE_CONFIG_STATE` | usable input reaches `INPUT_LOOP_READY`; empty selections return `false` and allow retry; backend setup failure tears down to `DEAD` |
| MIDI-only `Config()` | `DEVICE_CONFIG_STATE` | reaches `INPUT_LOOP_READY` with a MIDI buffer and null `input_arena`; no keyboard/mouse backend is required |
| `Run()` | `INPUT_LOOP_READY` | enters `INPUT_LOOP_RUNNING`; startup failure tears down to `DEAD` and invalidates data lines |
| `Kill()` | any state | clears owned backend/MIDI state, invalidates data lines, and ends in `DEAD`; already-dead teardown is a successful no-op |

`PullOutDataLine()` exposes configured owners: input only, MIDI only, both, or
neither. Stop the judge with `End()` before killing or replacing its input
producer; the attached pointers are borrowed.

`MIDI_shared` owns the MIDI event buffer. `MIDI` and receive callbacks retain it
through `shared_ptr`; callbacks do not reference the `MIDI` object. This protects
callback storage lifetime, not driver shutdown or borrowed consumer lifetime.
Direct native buffer access is through `MIDI::GetEventBuffer()`.

## Judge Lifecycle

Source anchors: `include/judge/PDJE_Judge.hpp`,
`include/judge/PDJE_Judge.cpp`, and `include/judge/Init/`.

| Contract | Caller obligation |
| --- | --- |
| Native input attachment accepts either buffer; an all-null line is ignored without clearing a previous attachment. Native core attachment remains stricter than C ABI core attachment. | Validate the members used by the consumer; do not use an empty input line as a detach operation. |
| Note collection requires a registered rail and returns no notes for an empty or missing rail. | Register the referenced rail before collecting its notes. |
| `Start()` requires accepted core/input lines, notes, an event rule, and rails, and rejects an already running judge. | Complete initialization and keep both producers alive before starting. |
| `End()` stops and joins owned workers; the native destructor also calls it. | Explicitly end the judge before invalidating any borrowed producer data line. |
| `End()` clears data lines, notes, rules, and rails but retains `inits.lambdas`. | Reconfigure retained callbacks explicitly when reusing a judge. |
| Note frames use a fixed 48 kHz-to-microseconds floor conversion. | Treat timing conversion changes as behavior compatibility changes. |

## Gameplay Controls

Source anchors: `include/GamePlay/PDJE_GamePlay.hpp`,
`include/judge/Loop/PDJE_Judge_Loop.hpp`, and
`include/core/MainObjects/audioPlayer/audioPlayer.hpp`.

- Orchestration belongs to `PDJE_GAMEPLAY::FACADE`; serialize controls and
  `GetState()` on a non-callback thread. Shared ownership does not synchronize
  external mutation or replacement. Native consumers must rebuild for new layouts.
- `Pause()` stops new note/miss/use/axis production before stopping the whole
  player. Input polling continues; committed use/miss callbacks may still finish.
  Do not interpret `PAUSED` as a callback-drain barrier.
- `Resume()` preserves note progress and requires fresh audio synchronization.
  Inputs older than its raw-clock cutoff are discarded before calibration offsets;
  batches crossing a production-generation boundary are discarded as a whole.
- `Restart()` joins old use/miss callbacks and discards old jobs before resetting
  note progress and Core time. It preserves setup and returns to facade `READY`,
  requiring explicit `Play()`. Existing Input/Judge polling stays running while
  judgments remain suspended. User-owned callback/axis/score state is not reset.
- Core rewind currently supports **full pre-render only**. Hybrid/manual restart
  is rejected before state mutation because deck prediction/DSP histories have
  no coordinated reset. Whole-player pause/resume do not require such a reset.
- Runtime failures are logged. Inspect `GetState()`; `FAULTED` requires `End()`
  before reuse. Restart waits for callbacks to return, so callbacks must be bounded.
  `End()` remains destructive teardown, not a configuration-preserving stop.

## C ABI Compatibility

The contract headers are `include/core/interface/CPDJE_interface.h`,
`include/input/CPDJE_Input.h`, and `include/judge/CPDJE_Judge.h`; adjacent
`.cpp` files are implementations.

| Surface | Contract |
| --- | --- |
| output handles | The caller supplies an empty ownership slot; producers publish a handle only after successful construction. |
| versioned outputs | `struct_size == 0` selects the V1 size; undersized nonzero values are rejected; known V1 fields are initialized without writing beyond the V1 prefix. |
| string, list, and event views | Views are borrowed for the lifetime documented by their owner or callback; empty views use `{nullptr, 0}`. |
| snapshots | Core snapshots copy cursor scalars but borrow pre-rendered audio; input snapshots own copied events until snapshot destruction. |
| judge callbacks | Callbacks may run on the worker thread; callback views expire on return, and the same judge must not be ended or destroyed reentrantly. |
| judge attachments | Engine and input handles are borrowed and must remain alive through `PDJE_Judge_End()`. |

Changing C structs, enums, callbacks, ownership functions, symbol names, or
these lifetimes requires ABI compatibility review.

## Utility Interfaces

`PDJE_Util.hpp` is not an exhaustive umbrella and does not define a
Status/Result transport layer. STFT and image/WebP APIs use public leaf headers;
`detail/` remains internal even when broad install rules copy its headers.
