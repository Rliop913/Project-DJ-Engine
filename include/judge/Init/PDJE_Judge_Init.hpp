#pragma once
#include "PDJE_ATOMIC_EVENT.hpp"
#include "PDJE_Core_DataLine.hpp"
#include "PDJE_Input_DataLine.hpp"
#include "PDJE_Note_OBJ.hpp"
#include "PDJE_Rule.hpp"
#include <cstdint>
#include <optional>
#include <unordered_map>
namespace PDJE_JUDGE {

using RAIL_ID = uint64_t;

struct Custom_Events {
    std::function<void(std::unordered_map<uint64_t, NOTE_VEC>)> missed_event;
    std::function<void(
        uint64_t railid, bool Pressed, bool IsLate, uint64_t diff)>
        used_event;
    std::function<void(uint64_t             microSecond,
                       const P_NOTE_VEC    &found_events,
                       uint64_t             railID,
                       int                  x,
                       int                  y,
                       PDJE_Mouse_Axis_Type axis_type)>
        custom_axis_parse;
};

class Judge_Init {
  private:
    void
    DefaultFill(NOTE          &obj,
                const uint64_t railid,
                const uint64_t axis1,
                const uint64_t axis2);

  public:
    Custom_Events                       lambdas;
    std::optional<PDJE_CORE_DATA_LINE>  coreline;
    std::optional<PDJE_INPUT_DATA_LINE> inputline;

    // note object
    std::optional<OBJ> note_objects;

    // rules
    std::optional<EVENT_RULE>               ev_rule;
    std::unordered_map<INPUT_RULE, RAIL_ID> dev_rules;

    void
    SetInputRule(const INPUT_CONFIG &device_config);
    void
    SetEventRule(const EVENT_RULE &event_rule);

    void
    SetCustomEvents(const Custom_Events &events);

    void
    NoteObjectCollector(const std::string        noteType,
                        const uint16_t           noteDetail,
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
};
}; // namespace PDJE_JUDGE