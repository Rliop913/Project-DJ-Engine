#pragma once
#include <future>
#include <optional>
#include <string>
#include <vector>

enum class PDJE_Dev_Type { MOUSE, KEYBOARD, HID, UNKNOWN };

struct DeviceData {
    PDJE_Dev_Type Type;
    std::string   Name;
    std::string   device_specific_id;
};
using DEV_LIST             = std::vector<DeviceData>;
using ONE_SHOT_DEV_PROMISE = std::optional<std::promise<DEV_LIST>>;
using ONE_SHOT_DEV_FUTURE  = std::optional<std::future<DEV_LIST>>;
using DEV_LIST             = std::vector<DeviceData>;
using ONE_SHOT_DEV_PROMISE = std::optional<std::promise<DEV_LIST>>;
using ONE_SHOT_DEV_FUTURE  = std::optional<std::future<DEV_LIST>>;