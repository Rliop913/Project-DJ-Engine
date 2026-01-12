#pragma once

#include "Input_State.hpp"
#include "Input_Transfer.hpp"
#include "PDJE_EXPORT_SETTER.hpp"
#include "PDJE_Input_Device_Data.hpp"
#include "PDJE_MIDI.hpp"
#include <unordered_map>
using PDJE_NAME = std::string;
using PDJE_ID   = std::string;

struct PDJE_API PDJE_INPUT_DATA_LINE {
    PDJE_IPC::PDJE_Input_Transfer            *input_arena = nullptr;
    Atomic_Double_Buffer<PDJE_MIDI::MIDI_EV> *midi_datas  = nullptr;
};