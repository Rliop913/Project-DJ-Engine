#include "PDJE_Judge_Loop.hpp"
#include "PDJE_Rule.hpp"
#include <cstdint>
#include <optional>

namespace PDJE_JUDGE {
constexpr int DOWN = 0;
constexpr int UP   = 1;
constexpr int X    = 2;
constexpr int Y    = 3;

void
Judge_Loop::ParseMouse(const BITMASK ev)
{
    std::optional<uint64_t> railtemp;
    if (ev & PDJE_MOUSE_L_BTN_DOWN) {
        Cached.meta.DeviceKey = DEVICE_MOUSE_EVENT::BTN_L;
        railtemp              = QueryRailid(Cached.meta);
        if (railtemp) {
            Cached.mouse_btn_event_queue.push_back({ railtemp.value(), DOWN });
        }
    }
    if (ev & PDJE_MOUSE_L_BTN_UP) {
        Cached.meta.DeviceKey = DEVICE_MOUSE_EVENT::BTN_L;
        railtemp              = QueryRailid(Cached.meta);
        if (railtemp) {
            Cached.mouse_btn_event_queue.push_back({ railtemp.value(), UP });
        }
    }
    if (ev & PDJE_MOUSE_R_BTN_DOWN) {
        Cached.meta.DeviceKey = DEVICE_MOUSE_EVENT::BTN_R;
        railtemp              = QueryRailid(Cached.meta);
        if (railtemp) {
            Cached.mouse_btn_event_queue.push_back({ railtemp.value(), DOWN });
        }
    }
    if (ev & PDJE_MOUSE_R_BTN_UP) {
        Cached.meta.DeviceKey = DEVICE_MOUSE_EVENT::BTN_R;
        railtemp              = QueryRailid(Cached.meta);
        if (railtemp) {
            Cached.mouse_btn_event_queue.push_back({ railtemp.value(), UP });
        }
    }
    if (ev & PDJE_MOUSE_M_BTN_DOWN) {
        Cached.meta.DeviceKey = DEVICE_MOUSE_EVENT::BTN_M;
        railtemp              = QueryRailid(Cached.meta);
        if (railtemp) {
            Cached.mouse_btn_event_queue.push_back({ railtemp.value(), DOWN });
        }
    }
    if (ev & PDJE_MOUSE_M_BTN_UP) {
        Cached.meta.DeviceKey = DEVICE_MOUSE_EVENT::BTN_M;
        railtemp              = QueryRailid(Cached.meta);
        if (railtemp) {
            Cached.mouse_btn_event_queue.push_back({ railtemp.value(), UP });
        }
    }
    if (ev & PDJE_MOUSE_SIDE_BTN_DOWN) {
        Cached.meta.DeviceKey = DEVICE_MOUSE_EVENT::BTN_SIDE;
        railtemp              = QueryRailid(Cached.meta);
        if (railtemp) {
            Cached.mouse_btn_event_queue.push_back({ railtemp.value(), DOWN });
        }
    }
    if (ev & PDJE_MOUSE_SIDE_BTN_UP) {
        Cached.meta.DeviceKey = DEVICE_MOUSE_EVENT::BTN_SIDE;
        railtemp              = QueryRailid(Cached.meta);
        if (railtemp) {
            Cached.mouse_btn_event_queue.push_back({ railtemp.value(), UP });
        }
    }
    if (ev & PDJE_MOUSE_EX_BTN_DOWN) {
        Cached.meta.DeviceKey = DEVICE_MOUSE_EVENT::BTN_EX;
        railtemp              = QueryRailid(Cached.meta);
        if (railtemp) {
            Cached.mouse_btn_event_queue.push_back({ railtemp.value(), DOWN });
        }
    }
    if (ev & PDJE_MOUSE_EX_BTN_UP) {
        Cached.meta.DeviceKey = DEVICE_MOUSE_EVENT::BTN_EX;
        railtemp              = QueryRailid(Cached.meta);
        if (railtemp) {
            Cached.mouse_btn_event_queue.push_back({ railtemp.value(), UP });
        }
    }
    if (ev & PDJE_MOUSE_XWHEEL) {
        Cached.meta.DeviceKey = DEVICE_MOUSE_EVENT::WHEEL_X;
        railtemp              = QueryRailid(Cached.meta);
        // std::cout << railtemp.value_or(-77) << " xwtf???" << std::endl;
        if (railtemp) {
            Cached.mouse_btn_event_queue.push_back({ railtemp.value(), X });
        }
    }
    if (ev & PDJE_MOUSE_YWHEEL) {
        Cached.meta.DeviceKey = DEVICE_MOUSE_EVENT::WHEEL_Y;
        railtemp              = QueryRailid(Cached.meta);
        // std::cout << railtemp.value_or(-77) << " ywtf???" << std::endl;
        if (railtemp) {
            Cached.mouse_btn_event_queue.push_back({ railtemp.value(), Y });
        }
    }
}

template <>
void
Judge_Loop::UseEvent<PDJE_Dev_Type::MOUSE>(const PDJE_Input_Log &ilog)
{

    Cached.mouse_btn_event_queue.clear();
    Cached.meta.Device_Name.assign(ilog.name, ilog.name_len);

    ParseMouse(ilog.event.mouse.button_type);

    for (const auto &mev : Cached.mouse_btn_event_queue) {
        switch (mev.status) {
        case DOWN:
            init_datas->note_objects->Get<BUFFER_MAIN>(
                Cached.use_range, mev.rail_id, Cached.found_list);
            Match(ilog.microSecond, Cached.found_list, mev.rail_id, true);
            break;
        case UP:
            init_datas->note_objects->Get<BUFFER_SUB>(
                Cached.use_range, mev.rail_id, Cached.found_list);
            Match(ilog.microSecond, Cached.found_list, mev.rail_id, false);
            break;
        case X:
            if (ilog.event.mouse.wheel_move > 0) {
                init_datas->note_objects->Get<BUFFER_MAIN>(
                    Cached.use_range, mev.rail_id, Cached.found_list);
                Match(ilog.microSecond, Cached.found_list, mev.rail_id, true);
            } else if (ilog.event.mouse.wheel_move < 0) {
                init_datas->note_objects->Get<BUFFER_SUB>(
                    Cached.use_range, mev.rail_id, Cached.found_list);
                Match(
                    (ilog.microSecond), Cached.found_list, mev.rail_id, false);
            }
            break;
        case Y:
            if (ilog.event.mouse.wheel_move > 0) {
                init_datas->note_objects->Get<BUFFER_MAIN>(
                    Cached.use_range, mev.rail_id, Cached.found_list);
                Match(ilog.microSecond, Cached.found_list, mev.rail_id, true);
            } else if (ilog.event.mouse.wheel_move < 0) {
                init_datas->note_objects->Get<BUFFER_SUB>(
                    Cached.use_range, mev.rail_id, Cached.found_list);
                Match(
                    (ilog.microSecond), Cached.found_list, mev.rail_id, false);
            }
            break;
        default:
            break;
        }
    }
    if (init_datas->lambdas.custom_mouse_parse &&
        (ilog.event.mouse.x != 0 || ilog.event.mouse.y != 0)) {
        Cached.meta.DeviceKey            = DEVICE_MOUSE_EVENT::AXIS_MOVE;
        std::optional<uint64_t> railtemp = QueryRailid(Cached.meta);
        if (railtemp) {
            init_datas->note_objects->Get<BUFFER_SUB>(
                Cached.use_range, railtemp.value(), Cached.found_list);
            init_datas->lambdas.custom_mouse_parse(ilog.microSecond,
                                                   Cached.found_list,
                                                   railtemp.value(),
                                                   ilog.event.mouse.x,
                                                   ilog.event.mouse.y,
                                                   ilog.event.mouse.axis_type);
        }
    }
}
}; // namespace PDJE_JUDGE