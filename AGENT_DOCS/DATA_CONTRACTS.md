# PDJE Data Contracts

Bridge types, states, modes, and timing contracts.

## Data Lines

| Type | Header | Fields | Judge acceptance |
| --- | --- | --- | --- |
| `PDJE_CORE_DATA_LINE` | `include/global/DataLines/PDJE_Core_DataLine.hpp` | `nowCursor`, `maxCursor`, `preRenderedData`, `syncD`; all non-owning and nullable | `SetCoreLine()` requires all four non-null |
| `PDJE_INPUT_DATA_LINE` | `include/global/DataLines/PDJE_Input_DataLine.hpp` | `input_arena`, `midi_datas`; both non-owning and nullable | `SetInputLine()` currently requires `input_arena != nullptr` |

## Modes And States

| Type | Value | Meaning |
| --- | --- | --- |
| `PLAY_MODE` | `FULL_PRE_RENDER` | player from track data in pre-render mode |
| `PLAY_MODE` | `HYBRID_RENDER` | player from track data in hybrid mode |
| `PLAY_MODE` | `FULL_MANUAL_RENDER` | player without track data |
| `PDJE_INPUT_STATE` | `DEVICE_CONFIG_STATE` | initialized and ready for device selection |
| `PDJE_INPUT_STATE` | `INPUT_LOOP_READY` | config succeeded; run allowed |
| `PDJE_INPUT_STATE` | `INPUT_LOOP_RUNNING` | input loop active |
| `PDJE_INPUT_STATE` | `DEAD` | inactive or torn down |

## Judge Types

| Type | Meaning |
| --- | --- |
| `EVENT_RULE` | miss/use window in microseconds |
| `RAIL_DB` | device or MIDI route to logical rail id |
| `NOTE` | judgable payload stored in microseconds |
| `OBJ` | note buffer owner for init and loop processing |

`Convert_Frame_Into_MicroSecond()` uses fixed 48 kHz conversion with floor
policy.

## Ownership

- Data lines are non-owning bridge structs.
- Wrappers mirror them through snapshots and handles, not ownership transfer.
- Callers must null-check every pointer member before use.
