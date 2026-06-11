# Verifier - Input

Role: Verifier
Model / effort: `gpt-5.4-mini xhigh`
Verification scope: Cross-check assigned Finder candidates independently.

## Candidate Disposition

### INPUT-CAND-001

- State: `accepted`
- Terminal: yes
- Independent evidence:
  Euler checked `include/input/runner/windows/InputLoop.cpp:47,102,154`,
  `include/global/DataLines/Input_State.hpp:170`, and
  `include/input/CPDJE_Input.cpp:602`; no `RIM_TYPEHID` handling was found
  under `include/`.
- Expected behavior: HID snapshot payloads should be initialized and populated
  only for HID input.
- Actual or inferred behavior: a local `PDJE_HID_Event` is copied into logs
  without a visible HID population path; visible effect is missing HID data.
- Rejection or deferral reason: n/a
- If deferred, follow-up condition: n/a
- Accepted severity: Major
- Notes for Lead: accepted as `BUG-005` after reopened verifier/Lead pass.

### INPUT-CAND-002

- State: `accepted`
- Terminal: yes
- Independent evidence:
  Euler checked `include/global/DataLines/Input_State.hpp:182`,
  `include/input/runner/windows/windows_keyboard_fill.hpp:7,25`, and
  `include/input/CPDJE_Input.cpp:585,593,596`.
- Expected behavior: C ABI export should read only the active union member by
  event type.
- Actual or inferred behavior: snapshot export reads keyboard and mouse fields
  unconditionally.
- Rejection or deferral reason: n/a
- If deferred, follow-up condition: n/a
- Accepted severity: Major
- Notes for Lead: accepted as `BUG-006` after reopened verifier/Lead pass.

### INPUT-CAND-003

- State: `accepted`
- Terminal: yes
- Independent evidence:
  Euler checked `include/input/PDJE_Input.cpp:58-60,63,156,199` and
  `include/input/PDJE_Input_StateLogic.hpp:37`.
- Expected behavior: invalid-state `Config()` should fail before mutating MIDI
  state.
- Actual or inferred behavior: MIDI devices and `FLAG_MIDI_ON` are assigned
  before `CanConfig(state)`.
- Rejection or deferral reason: n/a
- If deferred, follow-up condition: n/a
- Accepted severity: Minor
- Notes for Lead: accepted as `BUG-007`; scope is native C++ API because the C
  ABI wrapper has its own pre-call state gate.
