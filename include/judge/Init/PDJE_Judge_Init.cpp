#include "PDJE_Judge_Init.hpp"
#include "PDJE_Note_OBJ.hpp"
#include <iostream>
namespace PDJE_JUDGE {

void
Judge_Init::SetCoreLine(const PDJE_CORE_DATA_LINE &_coreline)
{
    if (_coreline.maxCursor != nullptr && _coreline.nowCursor != nullptr &&
        _coreline.preRenderedData != nullptr && _coreline.syncD != nullptr) {
        coreline = _coreline;
    }
}

void
Judge_Init::SetCustomEvents(const Custom_Events &events)
{
    lambdas = events;
}

void
Judge_Init::SetInputLine(const PDJE_INPUT_DATA_LINE &_inputline)
{
    if (_inputline.input_arena != nullptr) {
        inputline = _inputline;
    }
}

void
Judge_Init::SetDevice(const DeviceData &devData,
                      const BITMASK     DeviceKey,
                      const int64_t offset_microsecond,
                      const uint64_t    MatchRail)
{
    if (devData.Type == PDJE_Dev_Type::UNKNOWN) {
        return;
    }
    INPUT_RULE rule{ .Device_ID  = devData.device_specific_id,
                     .DeviceType = devData.Type,
                     .DeviceKey  = DeviceKey };
    dev_rules[rule] = { .MatchRail          = MatchRail,
                        .offset_microsecond = offset_microsecond };
    note_objects->SetOffsets({.MatchRail = MatchRail, .offset_microsecond = offset_microsecond});
}

void
Judge_Init::SetEventRule(const EVENT_RULE &event_rule)
{
    ev_rule = event_rule;
}
void
Judge_Init::DefaultFill(NOTE            &obj,
                        const uint64_t   railid,
                        const LOCAL_TIME axis1,
                        const LOCAL_TIME axis2)
{
    note_objects->Fill<BUFFER_MAIN>(obj, railid);
    if (axis2 != 0 && axis2 > axis1) {
        obj.isDown      = false;
        obj.microsecond = axis2;
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
    auto       tempobj  = NOTE{ .type        = noteType,
                                .detail      = noteDetail,
                                .first       = firstArg,
                                .second      = secondArg,
                                .third       = thirdArg,
                                .microsecond = 0,
                                .used        = false,
                                .isDown      = true };
    LOCAL_TIME micro_Y1 = Convert_Frame_Into_MicroSecond(Y_Axis);
    LOCAL_TIME micro_Y2 = Convert_Frame_Into_MicroSecond(Y_Axis_2);
    tempobj.microsecond = micro_Y1;
    INPUT_RULE key;
    for (const auto &k : dev_rules) {
        if (k.second.MatchRail == railID) {
            key = k.first;
        }
    }
    if (key.Device_ID == "") {
        return;
    }

    switch (key.DeviceType) {
    case PDJE_Dev_Type::KEYBOARD:
        DefaultFill(tempobj, railID, micro_Y1, micro_Y2);
        std::cout << "push keyboard, " << micro_Y1 << ", " << micro_Y2
                  << std::endl;
        break;
    case PDJE_Dev_Type::MOUSE:
        if (tempobj.type == "AXIS") { // axis type
            tempobj.isDown = false;
            note_objects->Fill<BUFFER_SUB>(tempobj, railID);
        } else {
            DefaultFill(tempobj, railID, micro_Y1, micro_Y2);
            std::cout << "push mouse, " << micro_Y1 << ", " << micro_Y2
                      << std::endl;
        }
        break;
    case PDJE_Dev_Type::MIDI:
        DefaultFill(tempobj, railID, micro_Y1, micro_Y2);
        break;
    case PDJE_Dev_Type::HID:
        DefaultFill(tempobj, railID, micro_Y1, micro_Y2);
        break;

    default:
        break;
    }
}
}; // namespace PDJE_JUDGE