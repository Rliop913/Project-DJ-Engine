#pragma once

#include "PDJE_EXPORT_SETTER.hpp"
#include "PDJE_Buffer.hpp"
#include "Input_State.hpp"
#include "PDJE_Input_Device_Data.hpp"

using PDJE_NAME = std::string;
using PDJE_ID = std::string;

struct PDJE_Input_Log{
    PDJE_Dev_Type type;
    PDJE_Input_Event event;
    PDJE_HID_Event hid_event;
    std::string id;
    uint64_t microSecond;
};


struct PDJE_API PDJE_INPUT_DATA_LINE {
    PDJE_Buffer_Arena<PDJE_Input_Log>* input_arena = nullptr;
    std::unordered_map<PDJE_ID, PDJE_NAME>* id_name_conv = nullptr;
};