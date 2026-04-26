# PDJE Data Contracts

This page fixes the current meaning of the small bridge types and timing enums.

## Core Data Line

`PDJE_CORE_DATA_LINE` lives in
`include/global/DataLines/PDJE_Core_DataLine.hpp`.

| Field | Meaning | Contract |
| --- | --- | --- |
| `nowCursor` | current playback cursor | non-owning pointer, may be null |
| `maxCursor` | max playback cursor | non-owning pointer, may be null |
| `preRenderedData` | pre-rendered audio data pointer | non-owning pointer, may be null |
| `syncD` | atomic sync/timing payload | non-owning pointer, may be null |

`Judge_Init::SetCoreLine()` only accepts a line when all four members are
non-null.

## Input Data Line

`PDJE_INPUT_DATA_LINE` lives in
`include/global/DataLines/PDJE_Input_DataLine.hpp`.

| Field | Meaning | Contract |
| --- | --- | --- |
| `input_arena` | keyboard/mouse transport buffer | non-owning pointer, may be null |
| `midi_datas` | MIDI event double buffer | non-owning pointer, may be null |

`Judge_Init::SetInputLine()` currently accepts a line only when
`input_arena != nullptr`.

## Play Modes

`PLAY_MODE` lives in `include/core/interface/PDJE_interface.hpp`.

| Mode | Meaning |
| --- | --- |
| `FULL_PRE_RENDER` | initialize the player from track data in pre-render mode |
| `HYBRID_RENDER` | initialize the player from track data in hybrid mode |
| `FULL_MANUAL_RENDER` | initialize the player without track data in manual mode |

## Input State

`PDJE_INPUT_STATE` lives in `include/global/DataLines/Input_State.hpp`.

| State | Meaning |
| --- | --- |
| `DEVICE_CONFIG_STATE` | initialized and ready for device selection |
| `INPUT_LOOP_READY` | configuration succeeded and run is allowed |
| `INPUT_LOOP_RUNNING` | input loop active |
| `DEAD` | inactive or torn down |

## Judge Contracts

| Type | Meaning |
| --- | --- |
| `EVENT_RULE` | miss/use window in microseconds |
| `RAIL_DB` | map from device or MIDI route to logical rail id |
| `NOTE` | judgable note payload stored in microseconds |
| `OBJ` | note buffer owner used during init and loop processing |

Timing rule:

- `Convert_Frame_Into_MicroSecond()` uses a fixed 48 kHz conversion with floor
  policy.

## Ownership Rule

- data lines are non-owning bridge structs
- wrappers mirror them through snapshots and handles, not ownership transfer
- callers must null-check every pointer member before use
