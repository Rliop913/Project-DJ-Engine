#pragma once

#include "Input_State.hpp"
#include "Input_Transfer.hpp"
#include "PDJE_Buffer.hpp"
#include "PDJE_EXPORT_SETTER.hpp"
#include "PDJE_Input_Device_Data.hpp"
#include <unordered_map>
using PDJE_NAME = std::string;
using PDJE_ID   = std::string;

struct PDJE_API PDJE_INPUT_DATA_LINE {
    PDJE_Buffer_Arena<PDJE_Input_Log> *input_arena = nullptr;
};