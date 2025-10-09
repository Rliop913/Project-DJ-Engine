#pragma once

#include <optional>

#include "PDJE_Core_DataLine.hpp"
#include "PDJE_Input_DataLine.hpp"

class PDJE_JUDGE{
private:
    std::optional<PDJE_CORE_DATA_LINE> core;
    std::optional<PDJE_INPUT_DATA_LINE> input;
public:
    PDJE_JUDGE(const PDJE_CORE_DATA_LINE& core_line, const PDJE_INPUT_DATA_LINE& input_line);
    ~PDJE_JUDGE() = default;
};