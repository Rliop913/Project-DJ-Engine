#pragma once

#include "PDJE_EXPORT_SETTER.hpp"
#include "PDJE_Buffer.hpp"
enum class PDJE_Dev_Type{
    MOUSE,
    KEYBOARD,
    HID,
    UNKNOWN
};

using PDJE_NAME = std::string;
using PDJE_ID = std::string;

struct PDJE_Input_Log{
    PDJE_Dev_Type type;
    std::string id;
    uint64_t microSecond;
};


struct PDJE_API PDJE_INPUT_DATA_LINE {
    PDJE_Buffer_Arena<PDJE_Input_Log>* input_arena;
    std::unordered_map<PDJE_ID, PDJE_NAME>* id_name_conv;
};