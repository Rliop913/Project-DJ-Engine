# PDJE Input System

`PDJE_Input` owns device enumeration, backend configuration, live capture, and
the input data line.

## Surface

| Kind | Paths |
| --- | --- |
| facade/state | `include/input/PDJE_Input.hpp`, `.cpp`, `PDJE_Input_StateLogic.hpp` |
| devices | `include/input/DefaultDevs/` |
| transport | `include/input/IPC/`, `include/input/runner/` |
| MIDI | `include/input/midi/` |

## Flow

| State | Call / result |
| --- | --- |
| `DEAD` | construct, then `Init(platform_ctx0, platform_ctx1, use_internal_window)` |
| `DEVICE_CONFIG_STATE` | `Init()` prepared devices/MIDI; `GetDevs()` and `GetMIDIDevs()` may inspect them |
| `INPUT_LOOP_READY` | `Config(devs, midi_dev)` sanitized input and selected backend/MIDI-only outcome |
| `INPUT_LOOP_RUNNING` | `Run()` starts capture; `PullOutDataLine()` exposes buffers |
| `DEAD` | `Kill()` tears down backend state; safe as a no-op from `DEAD` |

## Contracts

- `Init()`, `Config()`, and `Run()` are valid only from the states above.
- Config sanitizes empty names, empty ids, and `UNKNOWN` device types.
- MIDI-only config is legal and returns through `Kill()` after readiness.
- Backend readiness and MIDI readiness are separate flags.
- `PullOutDataLine()` may expose only `input_arena`, only `midi_datas`, both,
  or neither.

## Platforms

- Linux: evdev, Wayland fallback, loader/runtime tests.
- Windows: subprocess/raw-input path under `runner/windows`.
- macOS: gated off by `PDJE_DEVELOP_INPUT=OFF`.

## Change Points

- discovery/runtime: `include/input/DefaultDevs/`
- buffer/IPC transport: `include/input/IPC/`
- MIDI: `include/input/midi/`
- state transitions: `PDJE_Input_StateLogic.hpp`

## Verify

- Use [TEST_MAP.md](TEST_MAP.md): input module or full unit rows.
