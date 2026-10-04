# Axis / MIDI verification

## Environment and result

- Preset: `windows-relwithdebinfo`; configuration: `RelWithDebInfo`.
- Generator: explicitly selected `-G Ninja`; build directory: repository-root `build/`.
- Compiler: VS 18 Community LLVM x64 `clang-cl` 20.1.8.
- Existing Conan dependencies were reused; no dependency source patches were made.
- Focused builds succeeded, including native MIDI-only Judge attachment tests.
  CMake regenerated the existing Ninja build after test-source wiring changed.
- **42/42 CTest cases passed**: 7 scalar core, 21 adapters/dispatch/pipeline,
  and 14 Input lifecycle/state/ownership cases. Final combined run: 1.43 seconds.
- The native Judge start/end/restart case also passed 20 consecutive repetitions.
- Both shared-state lifetime cases passed 20 consecutive repetitions each.
- Previous axis/Input C++ formatting checks passed; the new Judge lifecycle
  test and modified setter region were checked separately. `git diff --check`
  passed.
- No physical-device, other-platform, complete engine, or audio-thread tests ran.
- `.clangd` was not edited in this verification pass; its current working-tree
  difference was preserved, not reverted. Presets and generated Conan toolchain
  files were also left unchanged.

## What executed

- `pdje_unit_axis_model`: standard-library-only axis calibration, saturation,
  non-finite rejection, relative accumulation, reset/state isolation, extreme
  ranges, and exhaustive pitch-bend-domain monotonicity.
- `pdje_unit_axis_input`: mouse callback integration; MIDI range validation,
  exhaustive CC/pitch domains, seven-bit pressure, canonical paired-controller
  routing, port/channel identity, timestamp bounds, and Note On/Off regression.
  Three pipeline cases feed raw dummy MIDI into the real producer callback,
  double buffer, normalizer, and (for paired CC) real `Match::UseEvent` callback.
  They also cover unpaired mode and connection/channel-isolated CC state.
  Three native attachment cases additionally check all four buffer-presence
  combinations, empty-line rejection/preservation, replacement by MIDI-only,
  actual preprocessing and loop dispatch with exact song-local timestamps,
  and real `JUDGE::Start()`/`End()`/restart with dummy wire input. The threaded
  test uses a bounded future wait and stops the judge before releasing the MIDI
  producer. Core synchronization is synthetic, not an audio playback engine.
- `pdje_unit_input_midi`: real `PDJE_Input` Init/Config/Run/Kill/destructor paths,
  illegal-state calls, retryable empty configuration, filtered invalid devices,
  first/later-port startup rollback, reinitialization, instance isolation, stale
  controller-state cleanup, zero-based channels, and malformed wire data.
  The synchronous dummy backend replaces only enumeration/open/delivery, not
  lifecycle logic, decoding, or event buffering. Tests require no MIDI hardware,
  subprocess launch, sleeps, or global backend override.
  Two additional lifetime cases deliberately retain callback copies beyond
  connection teardown: one uses a latch-controlled `jthread` to invoke a callback
  after destroying its MIDI owner; the other retains callbacks after a partial
  startup failure and verifies they cannot write into a replacement engine.
  These test shared buffer ownership, not the real driver's shutdown guarantee.

The narrow targets compile actual production sources without unrelated
`GLOBAL_OBJ`/`INPUT_OBJ` dependencies. The Input target's test-only process-hash
header is scoped to that target: MIDI-only operation must never start the
keyboard/mouse subprocess. It is not usable for real default-device execution.

## Reproduction

The generated Conan toolchain sets `cl` as a normal CMake variable, overriding
cache-only compiler arguments. This untracked, build-local wrapper was used at
`build/axis-clang-cl-toolchain.cmake`:

```cmake
include("${CMAKE_CURRENT_LIST_DIR}/../conan_cmakes/conan_toolchain.cmake")
set(CMAKE_C_COMPILER "C:/Program Files/Microsoft Visual Studio/18/Community/VC/Tools/Llvm/x64/bin/clang-cl.exe")
set(CMAKE_CXX_COMPILER "${CMAKE_C_COMPILER}")
set(CMAKE_C_COMPILER "${CMAKE_C_COMPILER}" CACHE FILEPATH "Verification C compiler" FORCE)
set(CMAKE_CXX_COMPILER "${CMAKE_C_COMPILER}" CACHE FILEPATH "Verification C++ compiler" FORCE)
```

From the repository root in a Windows command prompt:

```bat
call conan_cmakes\conanvcvars.bat
cmake --preset windows-relwithdebinfo -G Ninja -DCMAKE_TOOLCHAIN_FILE=G:/Project_DJ_Engine/build/axis-clang-cl-toolchain.cmake
cmake --build --preset windows-relwithdebinfo --target pdje_unit_axis_model pdje_unit_axis_input pdje_unit_input_midi --parallel 4
ctest --test-dir build -R "unit.judge::.*(axis|midi pipeline)|unit.input_midi::" --output-on-failure --timeout 30
ctest --test-dir build -R "midi pipeline: native judge starts ends" --repeat until-fail:20 --output-on-failure
ctest --test-dir build -R "unit.input_midi::input/midi-shared:" --repeat until-fail:20 --output-on-failure --timeout 30
git diff --check
```

`--fresh` is only needed when repairing a stale compiler cache, not on each run.

## Limits and compatibility

- The earlier full `pdje_unit_judge` build encountered the existing third-party
  `build/_deps/cppcodec-src/cppcodec/detail/stream_codec.hpp:98` dependent-template
  error in `pad.operator()<CodecVariant>(...)`. That dependency was not changed;
  the full target is not claimed to pass.
- Compiler warnings remain in existing dependency/header code (including rail
  hash qualification, unused parsing helpers, and high-resolution clock fields).
- Active LSP checks remain inconclusive: the latest seven modified C++ paths
  timed out without confirming cleanliness. Real clang-cl compilation and
  executed tests are the evidence.
- Hardware transport/concurrent callback shutdown and real mixed keyboard/mouse
  operation were not exercised. The backend contract requires connection
  destruction to stop delivery and join in-flight callbacks; the dummy is
  deliberately synchronous.
- Native `SetInputLine` now accepts MIDI-only input without an API/layout change
  to the setter. Keyboard/mouse and mixed attachment tests use an opaque pointer
  identity, never a live IPC producer; their runtime transport is not covered.
  No physical audio playback or complete engine integration is claimed.
- `Custom_Events`, Input, and MIDI native layouts changed; rebuild native C++
  consumers. No C ABI layout or C ABI axis callback was added. Decoded channels
  are zero-based (wire channel 1 maps to 0); rail configuration must match.
- `MIDI` now owns `shared_ptr<MIDI_shared>` (without `optional`); the callback
  captures that pointer rather than `this`. Only the shared event buffer moved;
  decoder state remains per connection. Migrate direct `midi.evlog` access to
  `midi.GetEventBuffer()`. No stop/drain gate or production worker was added;
  continuous Judge polling and the existing buffer synchronization are unchanged.
  The manual MIDI sample was migrated but not executed.
- Use the same CC pairing mode for the producer and Judge normalization.
  `PDJE_Input::Run()` uses the paired default. Axis callbacks deliver samples
  only; they do not implement axis-note judgment, calibration discovery,
  relative-encoder interpretation, RPN/NRPN, or filtering policies.
