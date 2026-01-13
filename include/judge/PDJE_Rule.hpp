#pragma once
#include "Input_State.hpp"
#include "PDJE_EXPORT_SETTER.hpp"
#include "PDJE_Input_Device_Data.hpp"
#include <cstddef>
#include <cstdint>
#include <functional>
#include <string>
#include <unordered_map>
#include <variant>
namespace PDJE_JUDGE {

/** @brief Mouse-specific event ids mapped from input log. */
enum DEVICE_MOUSE_EVENT {
    PDJE_BTN_EX   = 0,
    PDJE_BTN_SIDE = 1,
    PDJE_BTN_M,
    PDJE_BTN_R,
    PDJE_BTN_L,
    PDJE_WHEEL_X,
    PDJE_WHEEL_Y,
    PDJE_AXIS_MOVE
};

/** @brief Global hit window configuration in microseconds. */
struct PDJE_API EVENT_RULE {
    uint64_t miss_range_microsecond = 0;
    uint64_t use_range_microsecond  = 0;
};

namespace RAIL_KEY {

struct PDJE_API KB_MOUSE {
    std::string Device_Name = "";
    BITMASK     DeviceKey   = 0;
    bool
    operator==(const KB_MOUSE &other) const noexcept
    {
        return Device_Name == other.Device_Name && DeviceKey == other.DeviceKey;
    }
};

struct PDJE_API MIDI {
    std::string port_name = "";
    uint8_t     type      = 0;
    uint8_t     ch        = 0;
    uint8_t     pos       = 0;
    bool
    operator==(const MIDI &other) const noexcept
    {
        return port_name == other.port_name && type == other.type &&
               ch == other.ch && pos == other.pos;
    }
};
struct PDJE_API META {
    std::variant<std::monostate, PDJE_Dev_Type, uint8_t> type;
    std::variant<std::monostate, KB_MOUSE, MIDI>         key;
};
}; // namespace RAIL_KEY

}; // namespace PDJE_JUDGE
