#pragma once
#include "InputParser.hpp"
#include "Input_State.hpp"

#include "PDJE_Highres_Clock.hpp"
#include "PDJE_Judge_Init.hpp"
#include "PDJE_Judge_Loop_Structs.hpp"
#include "PDJE_Note_OBJ.hpp"
#include "PDJE_Rule.hpp"
#include <cstdint>
#include <optional>
#include <vector>

namespace PDJE_JUDGE {

/** @brief Internal judge loop that consumes inputs, matches notes, and emits
 * callbacks. */
class Judge_Loop {

  private:
    EV_Thread Event_Controls;

    Queues Event_Datas;

  private: // cached values
    std::pair<PDJE_Input_Log *, uint64_t> input_log;

    LoopCached Cached;

    Judge_Init               *init_datas;
    PDJE_HIGHRES_CLOCK::CLOCK clock_root;
    /** @brief Trim expired notes into miss queue. */
    inline void
    Cut(const uint64_t cut_range);
    /** @brief Prepare cached values before processing input batch. */
    PARSE_OUT *
    PreProcess();

    /** @brief Parse mouse-specific input and enqueue events. */
    std::optional<uint64_t>
    QueryRailid(const RAIL_META &meta)
    {
        auto itr = init_datas->devparser.railData.find(meta);
        if (itr != init_datas->devparser.railData.end()) {
            return itr->second.MatchRail;
        } else {
            return std::nullopt;
        }
    }
    void
    ParseMouse(const BITMASK ev);

    template <PDJE_Dev_Type D>
    /** @brief Handle a single input log entry for the given device type. */
    void
    UseEvent(const PDJE_Input_Log &ilog);
    /** @brief Find device setting bound to a rule. */
    // bool
    // FindDevSetting(const INPUT_RULE &rule, INPUT_SETTING &setting);
    /** @brief Match input timestamp against note list and mark usage. */
    void
    Match(const LOCAL_TIME  input_time,
          const P_NOTE_VEC &note_list,
          const uint64_t    railid,
          const bool        isPressed);

  public:
    /** @brief Stop use/miss worker threads. */
    void
    EndEventLoop();
    /** @brief Start use/miss worker threads. */
    void
    StartEventLoop();
    /** @brief Main loop that polls inputs and judges notes. */
    void
    loop();

    std::atomic<bool> loop_switch;
    /** @brief Construct loop with initialized data sources. */
    Judge_Loop(Judge_Init &inits);
    ~Judge_Loop() = default;
};

}; // namespace PDJE_JUDGE
