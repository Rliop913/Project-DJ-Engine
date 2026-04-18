# PDJE Glossary

Compact definitions for terms that appear across code, tests, and control docs.

| Term | Canonical Meaning | Related Paths |
| --- | --- | --- |
| data line | non-owning bridge struct used to hand live state between modules | `include/global/DataLines/`, `ARCHITECTURE.md` |
| `PDJE_CORE_DATA_LINE` | core playback snapshot with cursor, pre-render, and sync pointers | `include/global/DataLines/PDJE_Core_DataLine.hpp` |
| `PDJE_INPUT_DATA_LINE` | input snapshot with input arena and optional MIDI data | `include/global/DataLines/PDJE_Input_DataLine.hpp` |
| rail | judge-side mapping from a keyboard, mouse, or MIDI route to a logical note lane | `include/judge/PDJE_RAIL.*`, `include/judge/Init/` |
| note object | judge-consumable note metadata staged per rail for matching and miss logic | `include/judge/NoteOBJ/`, `tests/unit/judge/note_obj.test.cpp` |
| `PLAY_MODE` | core playback mode enum used by the engine facade | `include/core/interface/PDJE_interface.hpp` |
| `FULL_PRE_RENDER` | playback mode that prepares the whole render path ahead of playback | `include/core/interface/PDJE_interface.*`, `include/core/MainObjects/audioPlayer/` |
| `HYBRID_RENDER` | playback mode that mixes pre-rendered and live-rendered work | `include/core/interface/PDJE_interface.*`, `include/core/MainObjects/audioPlayer/` |
| `FULL_MANUAL_RENDER` | playback mode for manual or live-started playback without track-driven pre-render setup | `include/core/interface/PDJE_interface.*`, `tests/unit/core/pdje_interface_pcm.test.cpp` |
| unit test | doctest case registered into CTest when `PDJE_TEST=ON` | `tests/unit/`, `cmakes/tests/units/`, `VERIFY.md` |
| dev/manual test | standalone developer executable built when `PDJE_DEV_TEST=ON` | `include/tests/`, `cmakes/tests/DevTests.cmake`, `TEST_MAP.md` |
| C ABI wrapper | C-facing facade layer mirroring the main C++ surfaces through `CPDJE*` libraries | `include/core/interface/CPDJE_interface.cpp`, `include/input/CPDJE_Input.cpp`, `include/judge/CPDJE_Judge.cpp` |
