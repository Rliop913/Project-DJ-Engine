#pragma once

#include "Input_State.hpp"
#include "PDJE_Buffer.hpp"
#include "PDJE_EXPORT_SETTER.hpp"
#include "PDJE_Input_Device_Data.hpp"
#include <unordered_map>
using PDJE_NAME = std::string;
using PDJE_ID   = std::string;

struct PDJE_API PDJE_Input_Log {
    PDJE_Dev_Type    type;
    PDJE_Input_Event event;
    PDJE_HID_Event   hid_event;
    char      id[256];
    char     name[256];
    uint16_t id_len;
    uint16_t name_len;
    uint64_t         microSecond;
};

struct PDJE_API PDJE_INPUT_DATA_LINE {
    PDJE_Buffer_Arena<PDJE_Input_Log>      *input_arena  = nullptr;
};