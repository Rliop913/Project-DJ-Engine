# Axis input

`AXIS_MODEL` stores one bounded coordinate. `Set` consumes absolute positions,
`Move` accumulates relative deltas, and `Value` maps minimum/center/maximum to
-1/0/1. Invalid calibration throws; non-finite samples return `nullopt` without
changing state. Finite samples outside the range saturate. Use a separate model
per device/axis and synchronize shared access yourself.

## MIDI

`MidiAxis.hpp` consumes already decoded `PDJE_MIDI::MIDI_EV` events. It does not
open ports or assemble wire bytes. `ParseMidiAxis` returns a validated normalized
value and canonical rail position; `NormalizeMidiAxis` returns only the value.
The existing MIDI producer has already combined CC MSB/LSB bytes.

| Message | Accepted decoded range | Rail position |
| --- | --- | --- |
| CC 0..63, paired mode | 0..16383 | `pos % 32` |
| CC 0..119, unpaired mode | 0, 128, ..., 16256 | unchanged |
| CC 64..119, paired mode | 0, 128, ..., 16256 | unchanged |
| Pitch Bend | 0..16383, center 8192 | 0 |
| Channel Pressure | 0..127 | 0 |
| Poly Pressure | 0..127 | note number 0..127 |

Channel numbers are the existing decoded `MIDI_EV::ch` identities (0..15), not
an additional wire-channel conversion. Channel Mode CC 120..127 and non-axis
messages are ignored. Malformed axis events return `nullopt`, not a clamped
endpoint. Relative-encoder, RPN/NRPN, and switch-specific interpretations are
not implemented: register only controllers whose values you want as axes.

### Judge callback

Configure before starting the judge, preserving any other required callbacks:

```cpp
auto events = judge.inits.lambdas;
events.midi_cc_lsb_on = true; // Must match the Input/MIDI Run setting.
events.midi_axis = [](PDJE_JUDGE::LOCAL_TIME time,
                      uint64_t rail,
                      double value) {
    // Consume the [-1, 1] sample. Keep this callback short and non-throwing.
};
judge.inits.SetCustomEvents(events);
judge.inits.SetRail("port-name", 42,
    static_cast<uint8_t>(libremidi::message_type::CONTROL_CHANGE),
    0, 1); // decoded channel 0, paired controller 1
```

In paired mode CC 1 and CC 33 both reach rail 42. Register the MSB position
(0..31); there is deliberately no fallback to a separate LSB rail. In unpaired
mode those controller positions remain independent. Port and channel identities
remain separate in both modes. Resolution cannot reliably be inferred from the
value, so the mode must match the producer for every configured device.

The callback is optional and runs synchronously on the judge thread, once per
valid registered event (no deduplication). Its time is song-local microseconds
after the existing preprocessing and device offset handling. It receives values
by copy and does not consume notes or call the normal note-hit callback. Note
On/Off still use their existing judgment path. Do not throw, block, mutate judge
configuration, or end/destroy the judge from this callback.

Adding the callback/settings extends native `Custom_Events`: rebuild native C++
consumers with the new headers. Existing C ABI structures were not changed and
no C ABI MIDI-axis callback was added.

### MIDI-only Input lifecycle

Use `Init()`, `GetMIDIDevs()`, `Config(empty_input_devices, selected_midi_ports)`,
then `Run()`, checking each result. Keyboard/mouse initialization is deferred;
calling `GetDevs()` explicitly requests that backend. A successful MIDI-only
configuration enters `INPUT_LOOP_READY` and exposes `midi_datas`, with a null
`input_arena`. `Kill()` and failed startup invalidate borrowed data-line pointers;
call `Init()` again before configuring a replacement session.

The decoder emits zero-based channels (wire channel 1 becomes `ch == 0`), rejects
malformed message lengths/data bytes, and keeps paired CC state per connection
and channel. `PDJE_Input::Run()` uses paired CC mode; direct `MIDI::Run(false)`
requires matching `midi_cc_lsb_on = false` on the Judge side.

Connect MIDI-only input with
`judge.inits.SetInputLine(input.PullOutDataLine())`. The native setter accepts
an input buffer, a MIDI buffer, or both; an all-null line leaves the existing
attachment unchanged. The usual core synchronization, notes, rails, and event
rule prerequisites for `Start()` still apply. Call `judge.End()` before
`input.Kill()` or producer destruction, then reattach after reconfiguration.

For direct MIDI access, use `midi.GetEventBuffer()` instead of `midi.evlog`.
The buffer lives in `MIDI_shared`, retained by the MIDI owner and its callbacks.
Port-local decoding state stays inside each callback. Shared ownership does not
replace transport shutdown or extend the caller's borrowed data-line contract.

## Mouse

`MOUSE_AXIS_MODEL` holds separate X/Y models. Feed `PDJE_Mouse_Event` or call
`Update(x, y, axis_type)` from `custom_mouse_parse`. Choose ranges appropriate to
the source coordinate space. Relative movement accumulates; absolute and virtual
desktop positions replace state. Ignored events return `nullopt`. Reset on device
reconnection; do not share state between unrelated devices/coordinate spaces.

## Tests

- `pdje_unit_axis_model`: standard-library-only scalar model tests.
- `pdje_unit_axis_input`: adapters, real Match dispatch, and dummy-wire MIDI
  pipeline/native Judge lifecycle tests, without physical devices or unrelated
  full-engine objects. Judge runtime tests use synthetic core synchronization.
- `pdje_unit_input_midi`: real Input lifecycle with an instance-local synchronous
  dummy backend, under CTest's `input_midi` label.

The two axis targets are discovered under CTest's `judge` label. See
`tests/unit/judge/axis_model.verification.md` at the repository root for the
verified toolchain and commands.
