#pragma once
#include "Input_State.hpp"
#include "PDJE_EXPORT_SETTER.hpp"
#include "PDJE_Input_Device_Data.hpp"
#include <cstddef>
#include <cstdint>
#include <functional>
#include <string>
#include <unordered_map>
namespace PDJE_JUDGE {

/** @brief Mouse-specific event ids mapped from input log. */
enum DEVICE_MOUSE_EVENT {
    BTN_EX   = 0,
    BTN_SIDE = 1,
    BTN_M,
    BTN_R,
    BTN_L,
    WHEEL_X,
    WHEEL_Y,
    AXIS_MOVE
};

/** @brief Device identity tuple used to bind inputs to rails. */
struct PDJE_API DEV {
    std::string Device_Name = "";
    bool
    operator==(const DEV &) const = default;
};

struct PDJE_API DEV_TYPE {
    PDJE_Dev_Type Type = PDJE_Dev_Type::UNKNOWN;
};
struct PDJE_API KEY {
    BITMASK DeviceKey = 0;
};

/** @brief Per-device judgment setting (rail mapping and offset). */
struct PDJE_API RAIL {
    uint64_t MatchRail = 0;
};
struct PDJE_API OFFSET {

    int64_t offset_microsecond = 0;
};

using RailToOffset = std::unordered_map<uint64_t, int64_t>;

/** @brief Convenience type combining rule and setting. */
struct PDJE_API INPUT_CONFIG : DEV, KEY, RAIL, OFFSET {};
struct PDJE_API RAIL_META : DEV, KEY {
    bool operator==(const RAIL_META& other) const noexcept{
        return Device_Name == other.Device_Name &&
        DeviceKey == other.DeviceKey;
    }
};
struct PDJE_API RAIL_SETTINGS : RAIL, OFFSET, DEV_TYPE {};

/** @brief Global hit window configuration in microseconds. */
struct PDJE_API EVENT_RULE {
    uint64_t miss_range_microsecond = 0;
    uint64_t use_range_microsecond  = 0;
};

}; // namespace PDJE_JUDGE

template <> struct std::hash<PDJE_JUDGE::RAIL_META> {
    std::size_t
    operator()(const PDJE_JUDGE::RAIL_META &rule) const noexcept
    {
        size_t h2 = std::hash<BITMASK>()(rule.DeviceKey);
        size_t h3 = std::hash<std::string>()(rule.Device_Name);

        size_t seed = 0;
        seed ^= h2 + 0x9e3779b97f4a7c15ULL + (seed << 6) + (seed >> 3);
        seed ^= h3 + 0x9e3779b97f4a7c15ULL + (seed << 6) + (seed >> 3);
        return seed;
    }
};
