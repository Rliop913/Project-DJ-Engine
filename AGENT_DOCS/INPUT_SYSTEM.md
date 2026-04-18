# PDJE Input System

This page covers `PDJE_Input`, its state machine, and its data-line contract.

## Purpose

- enumerate keyboard, mouse, and MIDI devices
- configure the selected backend
- run live input capture
- expose live input buffers to judge or external consumers

## Primary Entry Points

- `include/input/PDJE_Input.hpp`
- `include/input/PDJE_Input.cpp`
- `include/input/PDJE_Input_StateLogic.hpp`
- `include/input/DefaultDevs/`
- `include/input/midi/`

## Owning Paths

- `include/input/DefaultDevs/`
- `include/input/IPC/`
- `include/input/midi/`
- `include/input/runner/`

## Lifecycle / Flow

1. Construct `PDJE_Input`.
2. Call `Init(platform_ctx0, platform_ctx1, use_internal_window)`.
3. `Init()` creates `DefaultDevs`, prepares platform contexts, creates MIDI
   support, and moves state from `DEAD` to `DEVICE_CONFIG_STATE`.
4. Call `GetDevs()` and `GetMIDIDevs()` to inspect available devices.
5. Call `Config(devs, midi_dev)`.
6. `Config()` sanitizes device input, configures the backend, and decides
   whether the module is input-backed, MIDI-only, or invalid.
7. Call `Run()` only from `INPUT_LOOP_READY`.
8. Pull live state through `PullOutDataLine()`.
9. Call `Kill()` to terminate backend state and reset to `DEAD`.

## Contracts / Invariants

- `Init()` is valid only from `DEAD`.
- `Config()` is valid only from `DEVICE_CONFIG_STATE`.
- `Run()` is valid only from `INPUT_LOOP_READY`.
- device configuration sanitizes out empty names, empty ids, and `UNKNOWN`
  device types before backend config.
- MIDI-only configuration is legal; it transitions through `INPUT_LOOP_READY`
  and then immediately tears backend state down through `Kill()`.
- `PullOutDataLine()` returns null members when input or MIDI are not active.
- `Kill()` is safe as a no-op from `DEAD`.

## Platform Notes

- Linux and Windows are the active input targets in the current tree.
- Linux owns evdev, Wayland fallback, and loader/runtime tests.
- Windows owns the subprocess/raw-input path under `runner/windows`.
- macOS is gated off through `PDJE_DEVELOP_INPUT=OFF`.

## Common Change Points

- device discovery and runtime: `include/input/DefaultDevs/`
- input buffer / IPC transport: `include/input/IPC/`
- MIDI handling: `include/input/midi/`
- state transitions: `PDJE_Input_StateLogic.hpp`

## Verify

- `ctest --test-dir build -R '^unit.input::' --output-on-failure`
- `ctest --test-dir build -L unit --output-on-failure`

## Known Traps

- `GetDevs()` assumes the module was initialized; it is not a free-standing
  static query API.
- backend readiness and MIDI readiness are separate flags.
- `PullOutDataLine()` does not guarantee both `input_arena` and `midi_datas`
  are present at the same time.
