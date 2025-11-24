#include "PDJE_Judge_Loop.hpp"

namespace PDJE_JUDGE {
constexpr int DOWN = 0;
constexpr int UP   = 1;
constexpr int X    = 2;
constexpr int Y    = 3;

void
Judge_Loop::ParseMouse(INPUT_RULE &rule, const BITMASK ev)
{
    if (ev & PDJE_MOUSE_L_BTN_DOWN) {
        rule.DeviceKey = DEVICE_MOUSE_EVENT::BTN_L;
        if (FindDevSetting(rule, Cached.setting)) {
            Cached.mouse_btn_event_queue.push_back(
                { Cached.setting.MatchRail,
                  
                  DOWN });
        }
    }
    if (ev & PDJE_MOUSE_L_BTN_UP) {
        rule.DeviceKey = DEVICE_MOUSE_EVENT::BTN_L;
        if (FindDevSetting(rule, Cached.setting)) {
            Cached.mouse_btn_event_queue.push_back(
                { Cached.setting.MatchRail,
                  
                  UP });
        }
    }
    if (ev & PDJE_MOUSE_R_BTN_DOWN) {
        rule.DeviceKey = DEVICE_MOUSE_EVENT::BTN_R;
        if (FindDevSetting(rule, Cached.setting)) {
            Cached.mouse_btn_event_queue.push_back(
                { Cached.setting.MatchRail,
                  
                  DOWN });
        }
    }
    if (ev & PDJE_MOUSE_R_BTN_UP) {
        rule.DeviceKey = DEVICE_MOUSE_EVENT::BTN_R;
        if (FindDevSetting(rule, Cached.setting)) {
            Cached.mouse_btn_event_queue.push_back(
                { Cached.setting.MatchRail,
                  
                  UP });
        }
    }
    if (ev & PDJE_MOUSE_M_BTN_DOWN) {
        rule.DeviceKey = DEVICE_MOUSE_EVENT::BTN_M;
        if (FindDevSetting(rule, Cached.setting)) {
            Cached.mouse_btn_event_queue.push_back(
                { Cached.setting.MatchRail,
                  
                  DOWN });
        }
    }
    if (ev & PDJE_MOUSE_M_BTN_UP) {
        rule.DeviceKey = DEVICE_MOUSE_EVENT::BTN_M;
        if (FindDevSetting(rule, Cached.setting)) {
            Cached.mouse_btn_event_queue.push_back(
                { Cached.setting.MatchRail,
                  
                  UP });
        }
    }
    if (ev & PDJE_MOUSE_SIDE_BTN_DOWN) {
        rule.DeviceKey = DEVICE_MOUSE_EVENT::BTN_SIDE;
        if (FindDevSetting(rule, Cached.setting)) {
            Cached.mouse_btn_event_queue.push_back(
                { Cached.setting.MatchRail,
                  
                  DOWN });
        }
    }
    if (ev & PDJE_MOUSE_SIDE_BTN_UP) {
        rule.DeviceKey = DEVICE_MOUSE_EVENT::BTN_SIDE;
        if (FindDevSetting(rule, Cached.setting)) {
            Cached.mouse_btn_event_queue.push_back(
                { Cached.setting.MatchRail,
                  
                  UP });
        }
    }
    if (ev & PDJE_MOUSE_EX_BTN_DOWN) {
        rule.DeviceKey = DEVICE_MOUSE_EVENT::BTN_EX;
        if (FindDevSetting(rule, Cached.setting)) {
            Cached.mouse_btn_event_queue.push_back(
                { Cached.setting.MatchRail,
                  
                  DOWN });
        }
    }
    if (ev & PDJE_MOUSE_EX_BTN_UP) {
        rule.DeviceKey = DEVICE_MOUSE_EVENT::BTN_EX;
        if (FindDevSetting(rule, Cached.setting)) {
            Cached.mouse_btn_event_queue.push_back(
                { Cached.setting.MatchRail,
                  
                  UP });
        }
    }
    if (ev & PDJE_MOUSE_XWHEEL) {
        rule.DeviceKey = DEVICE_MOUSE_EVENT::WHEEL_X;
        if (FindDevSetting(rule, Cached.setting)) {
            Cached.mouse_btn_event_queue.push_back(
                { Cached.setting.MatchRail,
                  
                  X });
        }
    }
    if (ev & PDJE_MOUSE_YWHEEL) {
        rule.DeviceKey = DEVICE_MOUSE_EVENT::WHEEL_Y;
        if (FindDevSetting(rule, Cached.setting)) {
            Cached.mouse_btn_event_queue.push_back(
                { Cached.setting.MatchRail,
                  
                  Y });
        }
    }
}

template <>
void
Judge_Loop::UseEvent<PDJE_Dev_Type::MOUSE>(const PDJE_Input_Log &ilog)
{
    if (ilog.microSecond < Cached.global_local_diff) {
        return;
    }
    Cached.mouse_btn_event_queue.clear();
    INPUT_RULE rule;
    rule.Device_ID  = ilog.name;
    rule.DeviceType = ilog.type;
    ParseMouse(rule, ilog.event.mouse.button_type);

    for (const auto &mev : Cached.mouse_btn_event_queue) {
        switch (mev.status) {
        case DOWN:
            init_datas->note_objects->Get<BUFFER_MAIN>(
                Cached.use_range, mev.rail_id, Cached.found_list);
            Match((ilog.microSecond) - Cached.global_local_diff,
                  Cached.found_list,
                  mev.rail_id,
                  true);
            break;
        case UP:
            init_datas->note_objects->Get<BUFFER_SUB>(
                Cached.use_range, mev.rail_id, Cached.found_list);
            Match((ilog.microSecond) - Cached.global_local_diff,
                  Cached.found_list,
                  mev.rail_id,
                  false);
            break;
        case X:
            if (ilog.event.mouse.wheel_move > 0) {
                init_datas->note_objects->Get<BUFFER_MAIN>(
                    Cached.use_range, mev.rail_id, Cached.found_list);
                Match((ilog.microSecond) -
                          Cached.global_local_diff,
                      Cached.found_list,
                      mev.rail_id,
                      true);
            } else if (ilog.event.mouse.wheel_move < 0) {
                init_datas->note_objects->Get<BUFFER_SUB>(
                    Cached.use_range, mev.rail_id, Cached.found_list);
                Match((ilog.microSecond) -
                          Cached.global_local_diff,
                      Cached.found_list,
                      mev.rail_id,
                      false);
            }
            break;
        case Y:
            if (ilog.event.mouse.wheel_move > 0) {
                init_datas->note_objects->Get<BUFFER_MAIN>(
                    Cached.use_range, mev.rail_id, Cached.found_list);
                Match((ilog.microSecond) -
                          Cached.global_local_diff,
                      Cached.found_list,
                      mev.rail_id,
                      true);
            } else if (ilog.event.mouse.wheel_move < 0) {
                init_datas->note_objects->Get<BUFFER_SUB>(
                    Cached.use_range, mev.rail_id, Cached.found_list);
                Match((ilog.microSecond) -
                          Cached.global_local_diff,
                      Cached.found_list,
                      mev.rail_id,
                      false);
            }
            break;
        default:
            break;
        }
    }
    if (init_datas->lambdas.custom_mouse_parse &&
        (ilog.event.mouse.x != 0 || ilog.event.mouse.y != 0)) {
        rule.DeviceKey = DEVICE_MOUSE_EVENT::AXIS_MOVE;
        if (FindDevSetting(rule, Cached.setting)) {
            init_datas->note_objects->Get<BUFFER_SUB>(
                Cached.use_range, Cached.setting.MatchRail, Cached.found_list);
            init_datas->lambdas.custom_mouse_parse(
                (ilog.microSecond) -
                    Cached.global_local_diff,
                Cached.found_list,
                Cached.setting.MatchRail,
                ilog.event.mouse.x,
                ilog.event.mouse.y,
                ilog.event.mouse.axis_type);
        }
    }
}
}; // namespace PDJE_JUDGE