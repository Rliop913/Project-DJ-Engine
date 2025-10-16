#pragma once
#include "Input_State.hpp"
#include "PDJE_Judge_Init.hpp"
#include "PDJE_Rule.hpp"
#include "PDJE_SYNC_CORE.hpp"
#include <cstdint>
#include <unordered_map>
#include <vector>

namespace PDJE_JUDGE {
class Judge_Loop {
  private:
    std::pmr::vector<PDJE_Input_Log>      *input_log;
    std::unordered_map<uint64_t, NOTE_VEC> missed_buffers;

    P_NOTE_VEC found_list;
    P_NOTE_VEC related_list_out;

    // time values
    uint64_t log_begin_time;
    uint64_t log_end_time;

    uint64_t use_range_time;
    uint64_t cut_range_time;

    audioSyncData synced_data;

    bool isLate;

    uint64_t railID;

    uint64_t noteMicro;

    uint64_t diff;

    struct railid_is_Down {
        uint64_t id     = 0;
        int      status = -1;
    };
    std::vector<railid_is_Down> temp_rules;

    Judge_Init *init_datas;
    bool
    PreProcess();
    void
    ParseMouse(INPUT_RULE                  &rule,
               const BITMASK                ev,
               std::vector<railid_is_Down> &res);
    template <PDJE_Dev_Type D>
    void
    UseEvent(const PDJE_Input_Log &ilog);
    bool
    FindRailID(const INPUT_RULE &rule, uint64_t &id);
    void
    Match(const uint64_t    input_time,
          const P_NOTE_VEC &note_list,
          const uint64_t    railid,
          const bool        isPressed);
    uint64_t
    FrameToMicro(uint64_t frame,
                 uint64_t origin_frame,
                 uint64_t origin_microsecond);

  public:
    void
                      loop();
    std::atomic<bool> loop_switch;
    Judge_Loop(Judge_Init &inits);
    ~Judge_Loop() = default;
};

}; // namespace PDJE_JUDGE