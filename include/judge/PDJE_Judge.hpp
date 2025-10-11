#pragma once

#include <cstdint>
#include <optional>

#include "PDJE_Core_DataLine.hpp"
#include "PDJE_Events.hpp"
#include "PDJE_Input_DataLine.hpp"
#include "PDJE_Note_OBJ.hpp"
#include "PDJE_Rule.hpp"
#include <atomic>
#include <thread>
#include <unordered_map>
#include <vector>
namespace PDJE_JUDGE {
enum JUDGE_STATUS {
    OK                   = 0,
    CORE_LINE_IS_MISSING = 1,
    INPUT_LINE_IS_MISSING,
    EVENT_RULE_IS_EMPTY,
    INPUT_RULE_IS_EMPTY,
    NOTE_OBJECT_IS_MISSING,
};

using RAIL_ID = uint64_t;
class JUDGE {
  private: // cached values
    std::pmr::vector<PDJE_Input_Log>      *input_log;
    std::unordered_map<uint64_t, NOTE_VEC> missed_in;
    std::unordered_map<uint64_t, NOTE_VEC> missed_out;
    P_NOTE_VEC                             related_list_in;
    P_NOTE_VEC                             related_list_out;

    // time values
    uint64_t log_begin_time;
    uint64_t log_end_time;

    uint64_t use_time;
    uint64_t cut_time;

    uint64_t nowFrame;
    uint64_t nowMicro;

    // flags
    bool Pressed;
    bool isLate;

    // extras
    uint64_t railID;

    uint64_t noteMicro;

    uint64_t diff;

    INPUT_RULE ir;

  private:
    std::optional<PDJE_CORE_DATA_LINE>      core;
    std::optional<PDJE_INPUT_DATA_LINE>     input;
    std::optional<OBJ>                      note_obj;
    std::optional<EVENT_RULE>               ev_rule;
    std::unordered_map<INPUT_RULE, RAIL_ID> dev_rules;
    std::optional<std::thread>              loop;
    JUDGE_STATUS                            status = JUDGE_STATUS::OK;
    std::atomic<bool>                       loop_switch;
    EVENTS                                  judge_event;
    void
    Match(const PDJE_Input_Log &input,
          const P_NOTE_VEC     &note_list,
          bool                  isPressed);
    void
    Judge_Loop();

  public:
    uint64_t
    FrameToMicro(uint64_t frame,
                 uint64_t origin_frame,
                 uint64_t origin_microsecond);

    void
    SetInputRule(const INPUT_CONFIG &device_config);
    void
    SetEventRule(const EVENT_RULE &event_rule);
    void
    NoteObjectCollector(const std::string        noteType,
                        const std::string        noteDetail,
                        const std::string        firstArg,
                        const std::string        secondArg,
                        const std::string        thirdArg,
                        const unsigned long long Y_Axis,
                        const unsigned long long Y_Axis_2,
                        const uint64_t           railID);

    void
    SetCoreLine(const PDJE_CORE_DATA_LINE &coreline);
    void
    SetInputLine(const PDJE_INPUT_DATA_LINE &inputline);

    JUDGE_STATUS
    Start();
    void
    End();
    JUDGE_STATUS
    CheckStatus();
    JUDGE()  = default;
    ~JUDGE() = default;
};
}; // namespace PDJE_JUDGE