# Finder - Input

Role: Finder
Model / effort: `gpt-5.4-mini medium`
Actor: Herschel
Scope: Input

## Coverage

- State transitions: inspected lifecycle docs and `PDJE_Input::Config()`.
- Device sanitization: inspected Linux/Windows event producers.
- MIDI-only behavior: known Judge limitation not re-filed.
- Platform runtime paths: inspected Windows raw-input loop.
- Data-line nullability: inspected snapshot and input data-line export.
- C ABI wrapper: inspected snapshot event export.

## Candidates

### INPUT-CAND-001 - Windows HID payload is copied without initialization

- State: `accepted`
- Affected area: Windows raw-input loop and C ABI snapshot export.
- Cross-cutting tags: Input, Windows, C ABI
- Suspected failure mode: snapshots may expose random HID bytes or drop actual
  HID payloads.
- Evidence or code path:
  `include/input/runner/windows/InputLoop.cpp:47`,
  `include/input/runner/windows/InputLoop.cpp:102-119`,
  `include/input/runner/windows/InputLoop.cpp:151-154`,
  `include/input/CPDJE_Input.cpp:602-606`.
- Expected behavior: HID data should be initialized and populated only when HID
  input is present.
- Observed or inferred behavior: `PDJE_HID_Event hidEv;` is copied into every
  log without a `RIM_TYPEHID` case populating it.
- Verification notes: accepted by Euler and merged by Lead Bohr as `BUG-005`.

### INPUT-CAND-002 - C ABI snapshot reads inactive input-event union members

- State: `accepted`
- Affected area: C ABI snapshot event mapping.
- Cross-cutting tags: Input, C ABI
- Suspected failure mode: keyboard snapshots read mouse fields and mouse
  snapshots read keyboard fields from an inactive union member.
- Evidence or code path:
  `include/global/DataLines/Input_State.hpp:182-185`,
  `include/input/DefaultDevs/linux/ParseKeyboard.cpp:13-16`,
  `include/input/DefaultDevs/linux/ParseMouse.cpp:11-12`,
  `include/input/CPDJE_Input.cpp:592-601`.
- Expected behavior: export should branch on `item.type` and copy only the
  active union member.
- Observed or inferred behavior: both keyboard and mouse members are read
  unconditionally.
- Verification notes: accepted by Euler and merged by Lead Bohr as `BUG-006`.

### INPUT-CAND-003 - Config mutates MIDI state before validating state

- State: `accepted`
- Affected area: native `PDJE_Input::Config()` lifecycle.
- Cross-cutting tags: Input, lifecycle
- Suspected failure mode: invalid-state `Config()` can cache MIDI devices and
  set `FLAG_MIDI_ON` despite returning failure.
- Evidence or code path:
  `include/input/PDJE_Input.cpp:58-63`,
  `include/input/PDJE_Input.cpp:84-89`,
  `include/input/PDJE_Input_StateLogic.hpp:37-44`,
  `include/input/PDJE_Input_StateLogic.hpp:98-107`.
- Expected behavior: invalid-state config calls should leave runtime state
  unchanged.
- Observed or inferred behavior: MIDI state is changed before `CanConfig()`.
- Verification notes: accepted by Euler and merged by Lead Bohr as `BUG-007`.
