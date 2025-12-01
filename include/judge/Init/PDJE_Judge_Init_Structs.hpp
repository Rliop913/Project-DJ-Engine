#pragma once
#include "PDJE_Note_OBJ.hpp"
#include <cstdint>
#include <functional>
namespace PDJE_JUDGE {

constexpr long double TO_MICRO = 1000.0 / 48.0;
/** @brief Convert PCM frame count to microseconds (48 kHz). */
inline uint64_t
Convert_Frame_Into_MicroSecond(const uint64_t pcm_frame)
{
    return static_cast<uint64_t>(static_cast<long double>(pcm_frame) *
                                 TO_MICRO);
}

using RAIL_ID = uint64_t;
using MISS_CALLBACK =
    std::function<void(std::unordered_map<uint64_t, NOTE_VEC>)>;
using USE_CALLBACK                = std::function<void(
    uint64_t railid, bool Pressed, bool IsLate, uint64_t diff)>;
using MOUSE_CUSTOM_PARSE_CALLBACK = // todo - make simpler
    std::function<void(const LOCAL_TIME     microSecond,
                       const P_NOTE_VEC    &found_events,
                       uint64_t             railID,
                       int                  x,
                       int                  y,
                       PDJE_Mouse_Axis_Type axis_type)>;
/** @brief Optional callback bundle used during judgment loop. */
struct PDJE_API Custom_Events {
    MISS_CALLBACK               missed_event;
    USE_CALLBACK                used_event;
    MOUSE_CUSTOM_PARSE_CALLBACK custom_mouse_parse;
    std::chrono::milliseconds   use_event_sleep_time =
        std::chrono::milliseconds(100);
    std::chrono::milliseconds miss_event_sleep_time =
        std::chrono::milliseconds(200);
};

} // namespace PDJE_JUDGE
