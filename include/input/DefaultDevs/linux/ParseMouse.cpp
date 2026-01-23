#include "InputCore.hpp"

void
InputCore::mouseRead(const input_event &evtrig, const int FD)
{
    PDJE_Input_Log ilog;
    ilog.type   = PDJE_Dev_Type::MOUSE;
    auto idstr  = std::to_string(FD);
    ilog.id_len = (idstr.size() > 255 ? 255 : idstr.size());
    std::memcpy(ilog.id, idstr.data(), ilog.id_len);
    ilog.microSecond        = clock.ConvertToMicroSecond(evtrig.time);
    const std::string &name = id_to_name[FD];
    ilog.name_len           = (name.size() > 255 ? 255 : name.size());
    std::memcpy(ilog.name, name.data(), ilog.name_len);
    switch (evtrig.type) {
    case EV_REL:
        ilog.event.mouse.axis_type   = PDJE_Mouse_Axis_Type::REL;
        ilog.event.mouse.button_type = 0;
        ilog.event.mouse.wheel_move  = 0;
        if (evtrig.code == REL_X) {
            ilog.event.mouse.x = evtrig.value;
            ilog.event.mouse.y = 0;
        } else {
            ilog.event.mouse.x = 0;
            ilog.event.mouse.y = evtrig.value;
        }
        break;
    case EV_ABS:
        ilog.event.mouse.axis_type   = PDJE_Mouse_Axis_Type::ABS;
        ilog.event.mouse.button_type = 0;
        ilog.event.mouse.wheel_move  = 0;
        if (evtrig.code == ABS_X) {
            ilog.event.mouse.x = evtrig.value;
            ilog.event.mouse.y = 0;
        } else {
            ilog.event.mouse.x = 0;
            ilog.event.mouse.y = evtrig.value;
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
            ilog.event.mouse.button_type = 0;
            break;
        }
    } break;
    }
    out->Write(ilog);
}