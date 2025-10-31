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
    if (_inputline.input_arena != nullptr &&
        _inputline.id_name_conv != nullptr) {
        inputline = _inputline;
    }
}

void
Judge_Init::SetInputRule(const INPUT_CONFIG &device_config)
{
    if (device_config.Device_ID == "") {
        return;
    }
    INPUT_RULE rule{ .Device_ID  = device_config.Device_ID,
                     .DeviceType = device_config.DeviceType,
                     .DeviceKey  = device_config.DeviceKey };
    dev_rules[rule] = device_config.MatchRail;
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
        if (k.second == railID) {
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