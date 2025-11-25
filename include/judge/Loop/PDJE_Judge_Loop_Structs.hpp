#pragma once

#include "PDJE_Buffer.hpp"
#include "PDJE_Judge_Init_Structs.hpp"
#include "PDJE_Note_OBJ.hpp"
#include "PDJE_Rule.hpp"
#include "PDJE_SYNC_CORE.hpp"
#include <cstdint>
#include <optional>
#include <string>
#include <thread>
#include <unordered_map>

namespace PDJE_JUDGE {
/** @brief Worker thread switches and handles for miss/use callbacks. */
struct EV_Thread {
    std::optional<bool> use_event_switch;
    std::optional<bool> miss_event_switch;

    std::optional<std::thread> use_event_thread;
    std::optional<std::thread> miss_event_thread;
};

/** @brief Data passed to use-event callbacks. */
struct useDatas {
    uint64_t railid;
    bool     Pressed;
    bool     IsLate;
    uint64_t diff;
};

/** @brief Buffered queues for miss/use events. */
struct Queues {
    PDJE_Buffer_Arena<std::unordered_map<uint64_t, NOTE_VEC>> miss_queue;
    PDJE_Buffer_Arena<useDatas>                               use_queue;
    Queues() : miss_queue(100), use_queue(100)
    {
    }
};

struct mouse_btn_event {
    uint64_t rail_id = 0;
    int      status  = -1;
};
/** @brief Cached state shared across judge loop iterations. */
struct LoopCached {

    std::unordered_map<uint64_t, NOTE_VEC> missed_buffers;

    P_NOTE_VEC found_list;
    P_NOTE_VEC related_list_out;

    // time values
    LOCAL_TIME local_microsecond_position;
    uint64_t   global_local_diff;

    // LOCAL_TIME log_begin;
    // LOCAL_TIME log_end;

    LOCAL_TIME  use_range;
    std::string strCache;
    // LOCAL_TIME cut_range;

    audioSyncData synced_data;

    RAIL_META meta;
    bool      isLate;
    uint64_t  railid;
    // INPUT_SETTING setting;

    uint64_t                     diff;
    std::vector<mouse_btn_event> mouse_btn_event_queue;
};
} // namespace PDJE_JUDGE
