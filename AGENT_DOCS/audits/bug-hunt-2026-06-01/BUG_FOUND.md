# PDJE Bug Hunt - Accepted Bugs

Date: 2026-06-01

This file records only verifier-accepted bugs. The Lead owns final insertion.

## Accepted Bugs

### BUG-001 - Long judge rail names are registered under the wrong key

- Severity: Major
- Affected modules: Judge, Input rail identity contracts
- Cross-cutting classification: data-line/name contract
- Finder candidate: `JUDGE-CAND-001`
- Verifier: McClintock
- Verification mode: static
- Approved commands: none
- Commands run: none
- Not run reason: no execution checks were approved for this run
- Fix-plan status: implemented; pending approved runtime verification

#### Summary

Judge rail registration tries to cap long device or MIDI port names but uses the
substring constructor incorrectly, storing the suffix beginning at byte 255
instead of a stable prefix.

#### Independent Evidence

The verifier checked `include/judge/Init/PDJE_Judge_Init.cpp:44-45,73-75` and
the later exact-name lookup paths in `include/judge/InputParser/InputParser.hpp`
plus keyboard/mouse match code. The stored keys can differ from the raw names
used during parsing.

#### Expected Behavior

Long rail/device names should be rejected or normalized in a way that later
parser and match lookup uses consistently.

#### Actual Or Inferred Behavior

`std::string(key.port_name, 255)` and
`std::string(key.Device_Name, 255)` create suffix strings, so long names are
registered under a different identity than the parser later uses.

#### Execution Record

Verification mode was static. No runtime command was approved or run.

#### Notes

The same issue was also found as `CORE-CAND-001`, which is recorded as a
duplicate in `final-review.md`.

### BUG-002 - Native judge double Start can terminate the process

- Severity: Critical
- Affected modules: Judge
- Cross-cutting classification: lifecycle
- Finder candidate: `JUDGE-CAND-002`
- Verifier: McClintock
- Verification mode: static
- Approved commands: none
- Commands run: none
- Not run reason: no execution checks were approved for this run
- Fix-plan status: implemented; pending approved runtime verification

#### Summary

The native `PDJE_JUDGE::JUDGE` class has no running-state guard in `Start()`.
Calling `Start()` twice without `End()` can overwrite an optional holding a
joinable `std::thread`.

#### Independent Evidence

The verifier checked `include/judge/PDJE_Judge.cpp:46-56,63-76` and
`include/judge/PDJE_Judge.hpp:27-31`. `End()` joins and resets, but `Start()`
does not check whether a loop thread already exists.

#### Expected Behavior

A second `Start()` before `End()` should be rejected cleanly, no-op safely, or
tear down the previous run before replacing thread state.

#### Actual Or Inferred Behavior

`loop.emplace(...)` can destroy the existing joinable thread object. Standard
`std::thread` destruction semantics can call `std::terminate`.

#### Execution Record

Verification mode was static. No runtime command was approved or run.

#### Notes

The C ABI wrapper has a separate `running` guard; the native C++ class does not.
`CORE-CAND-003` duplicates this accepted bug.

### BUG-003 - BeatThis dedupe can split one connected peak cluster

- Severity: Major
- Affected modules: Util
- Cross-cutting classification: AI postprocessing
- Finder candidate: `UTIL-CAND-003`
- Verifier: Popper
- Verification mode: static
- Approved commands: none
- Commands run: none
- Not run reason: no execution checks were approved for this run
- Fix-plan status: implemented; pending approved runtime verification

#### Summary

BeatThis peak deduplication compares each new peak to a running mean. That rule
is not transitive, so a connected broad peak cluster can be split into multiple
beat events.

#### Independent Evidence

The verifier checked `include/util/ai/beat_this/BeatThisPostprocessor.hpp:50`
and `include/util/ai/beat_this/BeatThisPostprocessor.cpp:79,163`. A cluster
such as `[0, 1, 2]` can split after the running mean moves.

#### Expected Behavior

A connected peak group inside the dedupe window should collapse to one
representative beat/downbeat event.

#### Actual Or Inferred Behavior

The moving-mean threshold can reject a later adjacent peak even though it is
still connected to the previous accepted peak.

#### Execution Record

Verification mode was static. No runtime command was approved or run.

#### Notes

Verifier suggested a gap-to-previous or interval-based clustering rule if
connected-peak grouping is the intended behavior.

### BUG-004 - Editor track title sanitizer failure is dereferenced

- Severity: Major
- Affected modules: Core
- Cross-cutting classification: editor validation
- Finder candidate: `CORE-CAND-002`
- Verifier: Euler
- Verification mode: static
- Approved commands: none
- Commands run: none
- Not run reason: no execution checks were approved for the reopened pass
- Fix-plan status: implemented; pending approved runtime verification

#### Summary

`editorObject::makeTrackData()` logs a failed track-title sanitization but then
unconditionally dereferences the empty optional.

#### Independent Evidence

Euler checked `include/global/DataLines/fileNameSanitizer.cpp:9`,
`include/core/MainObjects/editorObject/editorObject.cpp:26,32`,
`include/core/MainObjects/editorObject/render.cpp:13`, and
`include/core/db/trackDB.cpp:32`.

#### Expected Behavior

On sanitizer failure, the editor push/render path should bail out cleanly and
let the caller return failure.

#### Actual Or Inferred Behavior

For an overlong title, `sanitizeFileName()` returns `nullopt`, but
`makeTrackData()` still calls `safeTitle.value()`, which can throw
`std::bad_optional_access`.

#### Execution Record

Verification mode was static. No runtime command was approved or run.

#### Notes

Lead accepted the reopened verifier result as `BUG-004`.

### BUG-005 - Windows input loop drops HID payload data

- Severity: Major
- Affected modules: Input
- Cross-cutting classification: Windows runtime / C ABI snapshot
- Finder candidate: `INPUT-CAND-001`
- Verifier: Euler
- Verification mode: static
- Approved commands: none
- Commands run: none
- Not run reason: no execution checks were approved for the reopened pass
- Fix-plan status: implemented; unsupported HID payload surface removed; pending
  approved runtime verification

#### Summary

The Windows raw-input loop creates a `PDJE_HID_Event` and copies it into every
input log, but there is no visible `RIM_TYPEHID` branch that populates HID
payload data.

#### Independent Evidence

Euler checked `include/input/runner/windows/InputLoop.cpp:47,102,154`,
`include/global/DataLines/Input_State.hpp:170`, and
`include/input/CPDJE_Input.cpp:602`, and found no `RIM_TYPEHID` handling under
`include/`.

#### Expected Behavior

Windows raw input should initialize and populate HID payloads for HID events, or
explicitly zero/report unsupported HID data.

#### Actual Or Inferred Behavior

HID events are not parsed into the log. The C ABI snapshot sees an empty
`hid_report` because `hid_byte_size` remains zero.

#### Execution Record

Verification mode was static. No runtime command was approved or run.

#### Notes

The accepted visible user impact is missing HID data; verifier noted random
buffer bytes stay internal because the byte size remains zero.

### BUG-006 - Input C ABI snapshot reads inactive union arms

- Severity: Major
- Affected modules: Input
- Cross-cutting classification: C ABI snapshot
- Finder candidate: `INPUT-CAND-002`
- Verifier: Euler
- Verification mode: static
- Approved commands: none
- Commands run: none
- Not run reason: no execution checks were approved for the reopened pass
- Fix-plan status: implemented; pending approved runtime verification

#### Summary

`pdje_input_snapshot_input_get_v1()` exports both keyboard and mouse fields from
`PDJE_Input_Event`, even though that type is a union and producers write only
one active member.

#### Independent Evidence

Euler checked `include/global/DataLines/Input_State.hpp:182`,
`include/input/runner/windows/windows_keyboard_fill.hpp:7,25`, and
`include/input/CPDJE_Input.cpp:585,593,596`.

#### Expected Behavior

Snapshot export should branch on `item.type` and read only the active union arm,
leaving inactive fields zeroed in the C view.

#### Actual Or Inferred Behavior

The accessor reads `item.event.keyboard` and `item.event.mouse`
unconditionally, so inactive union data can be undefined or stale in the export.

#### Execution Record

Verification mode was static. No runtime command was approved or run.

#### Notes

The output view is reset first, but the unsafe source-side union read still
occurs.

### BUG-007 - Native Input Config mutates MIDI state before state validation

- Severity: Minor
- Affected modules: Input
- Cross-cutting classification: lifecycle
- Finder candidate: `INPUT-CAND-003`
- Verifier: Euler
- Verification mode: static
- Approved commands: none
- Commands run: none
- Not run reason: no execution checks were approved for the reopened pass
- Fix-plan status: implemented; pending approved runtime verification

#### Summary

The native `PDJE_Input::Config()` path writes MIDI configuration and flips
`FLAG_MIDI_ON` before checking whether the object is in `DEVICE_CONFIG_STATE`.

#### Independent Evidence

Euler checked `include/input/PDJE_Input.cpp:58-60,63,156,199` and
`include/input/PDJE_Input_StateLogic.hpp:37`.

#### Expected Behavior

An invalid-state `Config()` call should fail without mutating device or MIDI
runtime state.

#### Actual Or Inferred Behavior

If `midi_dev` is non-empty, the method mutates MIDI state before `CanConfig()`.
When the state check fails, those side effects remain and `PullOutDataLine()`
can expose MIDI data even though `Config()` returned false.

#### Execution Record

Verification mode was static. No runtime command was approved or run.

#### Notes

This bug is scoped to the native C++ API. The C ABI wrapper already has a
pre-call state gate. `CORE-CAND-004` is a duplicate of this bug.
