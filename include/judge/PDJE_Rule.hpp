#pragma once
#include "Input_State.hpp"
#include "PDJE_Input_Device_Data.hpp"

#include <cstddef>
#include <cstdint>
#include <functional>
#include <string>
namespace PDJE_JUDGE {

struct INPUT_RULE {
    std::string   Device_ID   = "";
    PDJE_Dev_Type MatchType   = PDJE_Dev_Type::UNKNOWN;
    BITMASK       MatchDetail = 0;
    bool
    operator==(const INPUT_RULE &) const = default;
};

struct INPUT_CONFIG : INPUT_RULE {
    uint64_t MatchRail = 0;
};

struct EVENT_RULE {
    uint64_t miss_range_microsecond = 0;
    uint64_t use_range_microsecond  = 0;
};

}; // namespace PDJE_JUDGE

template <> struct std::hash<PDJE_JUDGE::INPUT_RULE> {
    std::size_t
    operator()(const PDJE_JUDGE::INPUT_RULE &rule) const noexcept
    {
        size_t h1 = std::hash<int>()(static_cast<int>(rule.MatchType));
        size_t h2 = std::hash<BITMASK>()(rule.MatchDetail);
        size_t h3 = std::hash<std::string>()(rule.Device_ID);

        size_t seed = h1;
        seed ^= h2 + 0x9e3779b97f4a7c15ULL + (seed << 6) + (seed >> 3);
        seed ^= h3 + 0x9e3779b97f4a7c15ULL + (seed << 6) + (seed >> 3);
        return seed;
    }
};