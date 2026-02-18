#include "InputCore.hpp"
#include <algorithm>
#include <cstring>

void
InputCore::mouseRead(const input_event &evtrig, const int FD)
{
    constexpr std::size_t kStringCap = 256;

    PDJE_Input_Log ilog{};
    ilog.type   = PDJE_Dev_Type::MOUSE;
    auto idstr  = std::to_string(FD);
    ilog.id_len =
        static_cast<uint16_t>(std::min(idstr.size(), kStringCap));
    std::memcpy(ilog.id, idstr.data(), ilog.id_len);
    ilog.microSecond        = clock.ConvertToMicroSecond(evtrig.time);
    auto name_it = id_to_name.find(FD);
    if (name_it == id_to_name.end()) {
        return;
    }
    const std::string &name = name_it->second;
    ilog.name_len =
        static_cast<uint16_t>(std::min(name.size(), kStringCap));
    std::memcpy(ilog.name, name.data(), ilog.name_len);
    switch (evtrig.type) {
    case EV_REL:
        ilog.event.mouse.axis_type   = PDJE_Mouse_Axis_Type::REL;
        ilog.event.mouse.button_type = 0;
        ilog.event.mouse.wheel_move  = 0;
        ilog.event.mouse.x           = 0;
        ilog.event.mouse.y           = 0;
        if (evtrig.code == REL_X) {
            ilog.event.mouse.x = evtrig.value;
        } else if (evtrig.code == REL_Y) {
            ilog.event.mouse.y = evtrig.value;
        } else if (evtrig.code == REL_WHEEL ||
                   evtrig.code == REL_HWHEEL) {
            ilog.event.mouse.axis_type = PDJE_Mouse_Axis_Type::PDJE_AXIS_IGNORE;
            ilog.event.mouse.wheel_move = evtrig.value;
        } else {
            return;
        }
        break;
    case EV_ABS:
        ilog.event.mouse.axis_type   = PDJE_Mouse_Axis_Type::ABS;
        ilog.event.mouse.button_type = 0;
        ilog.event.mouse.wheel_move  = 0;
        ilog.event.mouse.x           = 0;
        ilog.event.mouse.y           = 0;
        if (evtrig.code == ABS_X) {
            ilog.event.mouse.x = evtrig.value;
        } else if (evtrig.code == ABS_Y) {
            ilog.event.mouse.y = evtrig.value;
        } else {
            return;
        }
        break;
    case EV_KEY: {
        ilog.event.mouse.axis_type = PDJE_Mouse_Axis_Type::PDJE_AXIS_IGNORE;

        ilog.event.mouse.wheel_move = 0;
        ilog.event.mouse.x          = 0;
        ilog.event.mouse.y          = 0;
        const bool down             = (evtrig.value != 0);
        switch (evtrig.code) {
        case BTN_LEFT:
            ilog.event.mouse.button_type =
                down ? PDJE_MOUSE_L_BTN_DOWN : PDJE_MOUSE_L_BTN_UP;
            break;
        case BTN_RIGHT:
            ilog.event.mouse.button_type =
                down ? PDJE_MOUSE_R_BTN_DOWN : PDJE_MOUSE_R_BTN_UP;
            break;
        case BTN_MIDDLE:
            ilog.event.mouse.button_type =
                down ? PDJE_MOUSE_M_BTN_DOWN : PDJE_MOUSE_M_BTN_UP;
            break;
        case BTN_SIDE:
            ilog.event.mouse.button_type =
                down ? PDJE_MOUSE_SIDE_BTN_DOWN
                     : PDJE_MOUSE_SIDE_BTN_UP; // XBUTTON1
            break;
        case BTN_EXTRA:
            ilog.event.mouse.button_type =
                down ? PDJE_MOUSE_EX_BTN_DOWN
                     : PDJE_MOUSE_EX_BTN_UP; // XBUTTON2
            break;
        default:
            return;
        }
    } break;
    default:
        return;
    }
    out->Write(ilog);
}
