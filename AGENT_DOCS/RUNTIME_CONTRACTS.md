# PDJE Runtime Contracts

This document records current cross-module behavior that is easy to violate or
misread. It is not a design wish list. Re-check the listed public headers and
implementations whenever changing these contracts.

## Non-Owning Data Lines

| View | Producer | Members | Native setter acceptance | C ABI attachment acceptance |
| --- | --- | --- | --- | --- |
| `PDJE_CORE_DATA_LINE` | `PDJE::PullOutDataLine()` / `audioPlayer` | `nowCursor`, `maxCursor`, `preRenderedData`, `syncD` | all four pointers must be non-null | `syncD` must be non-null; cursor and pre-render pointers are optional |
| `PDJE_INPUT_DATA_LINE` | `PDJE_Input::PullOutDataLine()` | `input_arena`, `midi_datas` | `input_arena` must be non-null; MIDI-only is rejected | either pointer may establish the attachment |

The structs do not own pointees. Their headers are
`include/global/DataLines/PDJE_Core_DataLine.hpp` and
`include/global/DataLines/PDJE_Input_DataLine.hpp`.

- A view is valid only while its producing runtime and referenced fields remain
  alive.
- `PDJE::ResetPlayer()` or player replacement can invalidate core pointers.
- `PDJE_Input::Kill()` clears the input/MIDI owners and invalidates input
  pointers.
- A null-filled line is a normal absence signal; consumers validate the fields
  they require.

## Core And Editor

Source anchors: `include/core/interface/PDJE_interface.hpp`,
`include/core/interface/PDJE_interface.cpp`, and
`include/core/MainObjects/editorObject/`.

1. `PDJE(root)` creates `DBROOT` and calls its open operation.
2. Search resolves music/track data from that root.
3. `InitPlayer()` selects one of three construction paths:
   `FULL_PRE_RENDER` and `HYBRID_RENDER` use track data;
   `FULL_MANUAL_RENDER` uses only the frame-buffer size.
4. `PullOutDataLine()` returns the player's view or an all-null view.
5. `ResetPlayer()` releases the facade's player reference.

Current edge behavior:

- `DBROOT` is public. Search and delete methods assume it remains non-null;
  `GetPCMFromMusData()` explicitly checks it.
- `InitPlayer()` does not clear an existing player before attempting a new one.
  An invalid mode or construction failure can therefore return `false` while a
  previous player remains installed.
- `InitEditor()` replaces `editor` before calling `Open()`. An `Open()` failure
  returns `false` but does not reset that newly allocated editor.
- `CloseEditor()` releases the facade's editor reference.
- `editorObject::Go<T>()` takes a commit OID. `DESTROY_PROJECT()` is
  irreversible.

## Input State Machine

Source anchors: `include/input/PDJE_Input.cpp` and
`include/input/PDJE_Input_StateLogic.hpp`.

| Entry state | Operation/outcome | Resulting state |
| --- | --- | --- |
| `DEAD` | successful `Init()` | `DEVICE_CONFIG_STATE` |
| `DEVICE_CONFIG_STATE` | valid input devices and successful backend config | `INPUT_LOOP_READY` |
| `DEVICE_CONFIG_STATE` | input backend failure or no usable input/MIDI | unchanged; `Config()` returns `false` |
| `DEVICE_CONFIG_STATE` | MIDI devices only | decision reaches ready, then `Config()` calls `Kill()`; it normally returns `true` with final state `DEAD` |
| `INPUT_LOOP_READY` | successful `Run()` | `INPUT_LOOP_RUNNING` |
| any active/config state | `Kill()` | owned backend/MIDI state is cleared and state becomes `DEAD` |
| `DEAD` | `Kill()` | successful no-op |

Consequences:

- The current MIDI-only path does not leave a runnable input instance or a live
  MIDI data line.
- `GetDevs()` and `GetMIDIDevs()` dereference initialized optionals; call them
  only after successful `Init()` and before teardown.
- `PullOutDataLine()` populates fields only for active flags. The current public
  flow can yield input only, input plus MIDI, or neither; the MIDI-only config
  path cleans itself up and yields neither.

## Judge Lifecycle

Source anchors: `include/judge/PDJE_Judge.hpp`,
`include/judge/PDJE_Judge.cpp`, and `include/judge/Init/`.

1. Populate `judge.inits` with accepted core/input lines, event rules, rails,
   note objects, and optional callbacks.
2. Register rails before `NoteObjectCollector()`; collection returns early when
   the rail DB is empty or the requested rail is absent.
3. `Start()` rejects an already running judge and checks core line, input line,
   notes, event rule, and rail DB. It sorts notes before starting the worker.
4. `End()` stops and joins the worker, then clears core/input lines, notes,
   event rule, and rails.

Additional current behavior:

- Native `SetCoreLine()` accepts only a line with all four core pointers.
- Native `SetInputLine()` accepts only a line with
  `input_arena != nullptr`.
- The C ABI wrapper deliberately applies the broader attachment rules in the
  table above. This supports manual core playback and input/MIDI combinations
  without changing the native setters.
- Note frame positions use a fixed 48 kHz-to-microseconds floor conversion in
  `include/judge/Init/PDJE_Judge_Init_Structs.hpp`.
- `End()` does not reset `inits.lambdas`.
- `JUDGE` has a default destructor. After a successful `Start()`, callers must
  call `End()` before destruction so the owned thread is joined.

## Utility Boundaries

- `include/util/PDJE_Util.hpp` includes AI, generic DB wrappers, fuzzy search,
  clamp, and slugify. It does not define a Status/Result transport layer.
- STFT types are exposed from public leaf headers in the `PDJE_PARALLEL`
  namespace.
- Waveform/WebP functions are exposed from public leaf headers in
  `PDJE_UTIL::function::image`.
- Concrete RocksDB, SQLite, and Annoy backends live under
  `include/util/db/backends/`; the generic wrappers expose backend state
  directly and use explicit lifecycle calls.
- Headers and sources under a `detail/` directory are internal implementation
  surfaces even when the install rule currently copies headers broadly.

## C ABI

The versioned C contracts live in
`include/core/interface/CPDJE_interface.h`, `include/input/CPDJE_Input.h`, and
`include/judge/CPDJE_Judge.h`. Treat their structs, enums, callbacks, ownership
functions, and symbol names as ABI. Their `.cpp` files are implementations,
not the contract entrypoints.

- An output handle parameter is an empty ownership slot. Producing functions
  set it to null before doing work and publish a handle only after successful
  construction.
- Versioned output structs accept `struct_size == 0` as the current V1 size,
  reject undersized nonzero values, initialize every known V1 field before a
  later failure, and leave bytes beyond the V1 prefix untouched.
- String/list/event views are borrowed. Empty views are
  `{data = nullptr, size = 0}`; nonempty views remain valid only for the
  lifetime documented beside the owning handle or callback.
- Core snapshots copy scalar cursor values but borrow pre-rendered audio.
  Input snapshots own their copied events until snapshot destruction.
- Judge callbacks may run on the judge worker thread. Callback views are valid
  only during the invocation, and callbacks must not re-enter judge end or
  destruction.
- Judge attachment handles are borrowed and must outlive
  `PDJE_Judge_End()`.

See [TEST_MAP.md](TEST_MAP.md) for the shared-library doctests and pure-C
symbol smokes.
