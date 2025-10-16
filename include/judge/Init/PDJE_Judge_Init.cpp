#include "PDJE_Judge.hpp"

namespace PDJE_JUDGE {

void
Judge_Init::SetCoreLine(const PDJE_CORE_DATA_LINE &_coreline)
{
    if (_coreline.maxCursor != nullptr && _coreline.nowCursor != nullptr &&
        _coreline.preRenderedData != nullptr && _coreline.syncD != nullptr) {
        coreline = coreline;
    }
}

void
Judge_Init::SetUseEvent(
    std::function<void(std::unordered_map<uint64_t, NOTE_VEC>)> _missed_event,
    std::function<void(
        uint64_t railid, bool Pressed, bool IsLate, uint64_t diff)> _used_event)
{
    missed_event = _missed_event;
    used_event   = _used_event;
}

void
Judge_Init::SetInputLine(const PDJE_INPUT_DATA_LINE &_inputline)
{
    if (_inputline.input_arena != nullptr &&
        _inputline.id_name_conv != nullptr) {
        inputline = _inputline;
    }
}

void
Judge_Init::SetInputRule(const INPUT_CONFIG &device_config)
{
    INPUT_RULE rule{ .Device_ID   = device_config.Device_ID,
                     .MatchType   = device_config.MatchType,
                     .MatchDetail = device_config.MatchDetail };
    dev_rules[rule] = device_config.MatchRail;
}

void
Judge_Init::SetEventRule(const EVENT_RULE &event_rule)
{
    ev_rule = event_rule;
}
void
Judge_Init::DefaultFill(NOTE          &obj,
                        const uint64_t railid,
                        const uint64_t axis1,
                        const uint64_t axis2)
{
    note_objects->Fill<BUFFER_MAIN>(obj, railid);
    if (axis2 != 0 && axis2 > axis1) {
        obj.isDown = false;
        obj.pos    = axis2;
        note_objects->Fill<BUFFER_SUB>(obj, railid);
    }
}
void
Judge_Init::NoteObjectCollector(const std::string        noteType,
                                const uint16_t           noteDetail,
                                const std::string        firstArg,
                                const std::string        secondArg,
                                const std::string        thirdArg,
                                const unsigned long long Y_Axis,
                                const unsigned long long Y_Axis_2,
                                const uint64_t           railID)
{
    if (dev_rules.empty()) {
        return;
    }
    if (!note_objects.has_value()) {
        note_objects.emplace();
    }
    auto tempobj = NOTE{ .type   = noteType,
                         .detail = noteDetail,
                         .first  = firstArg,
                         .second = secondArg,
                         .third  = thirdArg,
                         .pos    = 0,
                         .used   = false,
                         .isDown = true };
    tempobj.pos  = Y_Axis;
    INPUT_RULE key;
    for (const auto &k : dev_rules) {
        if (k.second == railID) {
            key = k.first;
        }
    }
    if (key.Device_ID == "") {
        return;
    }
    switch (key.MatchType) {
    case PDJE_Dev_Type::KEYBOARD:
        DefaultFill(tempobj, railID, Y_Axis, Y_Axis_2);
        break;
    case PDJE_Dev_Type::MOUSE:
        if (tempobj.type == "AXIS") { // axis type
            tempobj.isDown = false;
            note_objects->Fill<BUFFER_SUB>(tempobj, railID);
        } else {
            DefaultFill(tempobj, railID, Y_Axis, Y_Axis_2);
        }
        break;
    case PDJE_Dev_Type::MIDI:
        DefaultFill(tempobj, railID, Y_Axis, Y_Axis_2);
        break;
    case PDJE_Dev_Type::HID:
        DefaultFill(tempobj, railID, Y_Axis, Y_Axis_2);
        break;

    default:
        break;
    }
}
}; // namespace PDJE_JUDGE