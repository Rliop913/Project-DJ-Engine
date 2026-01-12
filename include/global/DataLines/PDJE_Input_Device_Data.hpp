#pragma once
#include <cstdint>
#include <future>
#include <latch>
#include <optional>
#include <string>
#include <vector>
enum class PDJE_Dev_Type { MOUSE, KEYBOARD, UNKNOWN };

struct DeviceData {
    PDJE_Dev_Type Type;
    std::string   Name;
    std::string   device_specific_id;
};
using DEV_LIST = std::vector<DeviceData>;