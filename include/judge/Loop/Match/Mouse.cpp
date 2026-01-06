#include "PDJE_Judge_Loop.hpp"
#include "PDJE_Rule.hpp"
#include <cstdint>
#include <optional>

#define PDJE_MOUSE_PARSE(MASK, PDJE_MOUSE_ENUM, VAL)                           \
    if (ev & MASK) {                                                           \
        meta.DeviceKey = PDJE_MOUSE_ENUM;                                      \
        railtemp       = QueryRailid(meta);                                    \
        if (railtemp) {                                                        \
            mouse_btn_ev_queue.push_back({ railtemp.value(), VAL });           \
        }                                                                      \
    }

namespace PDJE_JUDGE {
constexpr int DOWN = 0;
constexpr int UP   = 1;
constexpr int X    = 2;
constexpr int Y    = 3;

void
Match::ParseMouse(const BITMASK ev, RAIL_META &meta)
{
    std::optional<uint64_t> railtemp;
    PDJE_MOUSE_PARSE(PDJE_MOUSE_L_BTN_DOWN, DEVICE_MOUSE_EVENT::BTN_L, DOWN)
    PDJE_MOUSE_PARSE(PDJE_MOUSE_L_BTN_UP, DEVICE_MOUSE_EVENT::BTN_L, UP)
    PDJE_MOUSE_PARSE(PDJE_MOUSE_R_BTN_DOWN, DEVICE_MOUSE_EVENT::BTN_R, DOWN)
    PDJE_MOUSE_PARSE(PDJE_MOUSE_R_BTN_UP, DEVICE_MOUSE_EVENT::BTN_R, UP)
    PDJE_MOUSE_PARSE(PDJE_MOUSE_M_BTN_DOWN, DEVICE_MOUSE_EVENT::BTN_M, DOWN)
    PDJE_MOUSE_PARSE(PDJE_MOUSE_M_BTN_UP, DEVICE_MOUSE_EVENT::BTN_M, UP)
    PDJE_MOUSE_PARSE(
        PDJE_MOUSE_SIDE_BTN_DOWN, DEVICE_MOUSE_EVENT::BTN_SIDE, DOWN)
    PDJE_MOUSE_PARSE(PDJE_MOUSE_SIDE_BTN_UP, DEVICE_MOUSE_EVENT::BTN_SIDE, UP)
    PDJE_MOUSE_PARSE(PDJE_MOUSE_EX_BTN_DOWN, DEVICE_MOUSE_EVENT::BTN_EX, DOWN)
    PDJE_MOUSE_PARSE(PDJE_MOUSE_EX_BTN_UP, DEVICE_MOUSE_EVENT::BTN_EX, UP)
    PDJE_MOUSE_PARSE(PDJE_MOUSE_XWHEEL, DEVICE_MOUSE_EVENT::WHEEL_X, X)
    PDJE_MOUSE_PARSE(PDJE_MOUSE_YWHEEL, DEVICE_MOUSE_EVENT::WHEEL_Y, Y)
}

template <>
void
Match::UseEvent<PDJE_Dev_Type::MOUSE>(const PDJE_Input_Log &ilog)
{
    RAIL_META meta;
    mouse_btn_ev_queue.clear();
    meta.Device_Name.assign(ilog.name, ilog.name_len);

    ParseMouse(ilog.event.mouse.button_type, meta);

    for (const auto &mev : mouse_btn_ev_queue) {
        switch (mev.status) {
        case DOWN:

            init->note_objects->Get<BUFFER_MAIN>(
                pre->use_range, mev.rail_id, found_list);
            Work(ilog.microSecond, found_list, mev.rail_id, true);
            break;
        case UP:
            init->note_objects->Get<BUFFER_SUB>(
                pre->use_range, mev.rail_id, found_list);
            Work(ilog.microSecond, found_list, mev.rail_id, false);
            break;
        case X:
            if (ilog.event.mouse.wheel_move > 0) {
                init->note_objects->Get<BUFFER_MAIN>(
                    pre->use_range, mev.rail_id, found_list);
                Work(ilog.microSecond, found_list, mev.rail_id, true);
            } else if (ilog.event.mouse.wheel_move < 0) {
                init->note_objects->Get<BUFFER_SUB>(
                    pre->use_range, mev.rail_id, found_list);
                Work((ilog.microSecond), found_list, mev.rail_id, false);
            }
            break;
        case Y:
            if (ilog.event.mouse.wheel_move > 0) {
                init->note_objects->Get<BUFFER_MAIN>(
                    pre->use_range, mev.rail_id, found_list);
                Work(ilog.microSecond, found_list, mev.rail_id, true);
            } else if (ilog.event.mouse.wheel_move < 0) {
                init->note_objects->Get<BUFFER_SUB>(
                    pre->use_range, mev.rail_id, found_list);
                Work((ilog.microSecond), found_list, mev.rail_id, false);
            }
            break;
        default:
            break;
        }
    }
    if (init->lambdas.custom_mouse_parse &&
        (ilog.event.mouse.x != 0 || ilog.event.mouse.y != 0)) {
        meta.DeviceKey                   = DEVICE_MOUSE_EVENT::AXIS_MOVE;
        std::optional<uint64_t> railtemp = QueryRailid(meta);
        if (railtemp) {
            init->note_objects->Get<BUFFER_SUB>(
                pre->use_range, railtemp.value(), found_list);
            init->lambdas.custom_mouse_parse(ilog.microSecond,
                                             found_list,
                                             railtemp.value(),
                                             ilog.event.mouse.x,
                                             ilog.event.mouse.y,
                                             ilog.event.mouse.axis_type);
        }
    }
}
}; // namespace PDJE_JUDGE