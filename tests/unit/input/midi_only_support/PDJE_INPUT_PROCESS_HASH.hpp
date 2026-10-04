#pragma once

// Only pdje_unit_input_midi includes this directory. MIDI-only lifecycle tests
// never create DefaultDevs or launch its subprocess. An impossible hash keeps
// this focused target independent of that subprocess (and GLOBAL_OBJ), without
// weakening production hashing or substituting any lifecycle implementation.
static const char EMBEDDED_INPUT_PROCESS_SHA256[] =
    "midi-only-test-no-subprocess";
