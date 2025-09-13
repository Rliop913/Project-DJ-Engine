#pragma once

#include <optional>

#include "PDJE_Core_DataLine.hpp"

class PDJE_JUDGE{
private:
    std::optional<PDJE_CORE_DATA_LINE> core;
public:
    PDJE_JUDGE(const PDJE_CORE_DATA_LINE& core_line);
    ~PDJE_JUDGE() = default;
};