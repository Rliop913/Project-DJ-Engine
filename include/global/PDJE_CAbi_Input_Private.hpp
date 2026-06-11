#pragma once

#ifdef __cplusplus

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

#include "CPDJE_Input.h"
#include "PDJE_Input_Device_Data.hpp"
#include "PDJE_Input_Log.hpp"
#include "PDJE_MIDI.hpp"

struct PDJE_InputHandleV1 {
    void        *input = nullptr;
    std::string  backend_name_cache;
    void        *input_arena = nullptr;
    void        *midi_datas  = nullptr;
};

struct PDJE_InputDeviceListHandleV1 {
    std::vector<DeviceData> items;
};

struct PDJE_MidiDeviceListHandleV1 {
    std::vector<libremidi::input_port> items;
};

namespace PDJE_CABI {

inline PDJE_InputStringViewV1
MakeInputStringView(const char *value, std::size_t value_size) noexcept
{
    if (value == nullptr || value_size == 0) {
        return {};
    }
    return PDJE_InputStringViewV1 { value, value_size };
}

inline PDJE_InputDeviceTypeV1
ToInputDeviceType(const PDJE_Dev_Type type) noexcept
{
    switch (type) {
    case PDJE_Dev_Type::MOUSE:
        return PDJE_INPUT_DEVICE_MOUSE_V1;
    case PDJE_Dev_Type::KEYBOARD:
        return PDJE_INPUT_DEVICE_KEYBOARD_V1;
    default:
        return PDJE_INPUT_DEVICE_UNKNOWN_V1;
    }
}

inline void
ResetInputEventViewForWrite(PDJE_InputEventViewV1 *out_event) noexcept
{
    if (out_event == nullptr) {
        return;
    }
    *out_event = {};
    out_event->struct_size = sizeof(*out_event);
}

inline void
FillInputEventViewFromLog(const PDJE_Input_Log &item,
                          PDJE_InputEventViewV1 *out_event) noexcept
{
    if (out_event == nullptr) {
        return;
    }

    ResetInputEventViewForWrite(out_event);
    out_event->type = ToInputDeviceType(item.type);
    out_event->id   = MakeInputStringView(
        item.id, std::min<std::size_t>(item.id_len, sizeof(item.id)));
    out_event->name = MakeInputStringView(
        item.name, std::min<std::size_t>(item.name_len, sizeof(item.name)));
    out_event->microsecond = item.microSecond;

    switch (item.type) {
    case PDJE_Dev_Type::KEYBOARD:
        out_event->keyboard.key_code =
            static_cast<std::uint32_t>(item.event.keyboard.k);
        out_event->keyboard.pressed =
            item.event.keyboard.pressed ? 1 : 0;
        break;
    case PDJE_Dev_Type::MOUSE:
        out_event->mouse.button_type = item.event.mouse.button_type;
        out_event->mouse.wheel_move  = item.event.mouse.wheel_move;
        out_event->mouse.axis_type =
            static_cast<std::uint32_t>(item.event.mouse.axis_type);
        out_event->mouse.x = item.event.mouse.x;
        out_event->mouse.y = item.event.mouse.y;
        break;
    default:
        break;
    }
}

inline const PDJE_InputHandleV1 *
BorrowInputDataLine(PDJE_InputHandleV1 *input) noexcept
{
    return input;
}

inline const DeviceData *
TryGetInputDevice(const PDJE_InputDeviceListHandleV1 *list, std::size_t index) noexcept
{
    if (list == nullptr || index >= list->items.size()) {
        return nullptr;
    }
    return &list->items[index];
}

inline const libremidi::input_port *
TryGetMidiDevice(const PDJE_MidiDeviceListHandleV1 *list,
                 std::size_t                        index) noexcept
{
    if (list == nullptr || index >= list->items.size()) {
        return nullptr;
    }
    return &list->items[index];
}

} // namespace PDJE_CABI

#endif
