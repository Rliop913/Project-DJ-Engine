#include "PDJE_Judge_Init.hpp"
#include "PDJE_Input_Device_Data.hpp"
#include "PDJE_Note_OBJ.hpp"
#include "PDJE_Rule.hpp"
#include <cstdint>

template <class T, class U>
concept DecaysTo = std::same_as<std::decay_t<T>, U>;

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
Judge_Init::SetRail(const libremidi::input_port &midi_port,
                    const uint64_t               MatchRail,
                    const uint8_t                type,
                    const uint8_t                ch,
                    const uint8_t                pos,
                    const int64_t                offset_microsecond)
{
    RAIL_KEY::MIDI key;
    key.port_name = midi_port.port_name;
    if (key.port_name.size() > 255) {
        key.port_name = std::string(key.port_name, 255);
    }
    key.ch   = ch;
    key.pos  = pos;
    key.type = type;
    raildb.Add(key, MatchRail, offset_microsecond);
}
void
Judge_Init::SetRail(const DeviceData &devData,
                    const BITMASK     DeviceKey,
                    const int64_t     offset_microsecond,
                    const uint64_t    MatchRail)
{
    if (devData.Type == PDJE_Dev_Type::UNKNOWN) {
        return;
    }
    RAIL_KEY::KB_MOUSE key;
    key.Device_Name = devData.Name;
    if (key.Device_Name.size() > 255) {
        key.Device_Name = std::string(key.Device_Name, 255);
    }
    key.DeviceKey = DeviceKey;
    raildb.Add(key, devData.Type, MatchRail, offset_microsecond);
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

    if (raildb.Empty()) {
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

    auto res = raildb.GetMETA(railID);
    if (!res) {
        return; // if railid is not exists, discard note.
    }
    if (tempobj.type == "AXIS") {
        tempobj.isDown = false;
        note_objects->Fill<BUFFER_SUB>(
            tempobj,
            railID); // thid logic will be replaced after implemented AxisModel.
    } else {
        DefaultFill(tempobj, railID, micro_Y1, micro_Y2);
    }
}
}; // namespace PDJE_JUDGE