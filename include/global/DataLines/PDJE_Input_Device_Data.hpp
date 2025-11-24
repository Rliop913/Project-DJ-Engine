#pragma once
#include <cstdint>
#include <future>
#include <latch>
#include <optional>
#include <string>
#include <vector>
enum class PDJE_Dev_Type { MOUSE, KEYBOARD, MIDI, HID, UNKNOWN };

struct DeviceData {
    PDJE_Dev_Type Type;
    std::string   Name;
    std::string   device_specific_id;
    int64_t       offset_microsecond = 0;
};
using DEV_LIST = std::vector<DeviceData>;