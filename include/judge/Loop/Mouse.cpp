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
        rule.MatchDetail = DEVICE_MOUSE_EVENT::BTN_L;
        if (FindRailID(rule, Cached.railID)) {
            Cached.mouse_btn_event_queue.push_back({ Cached.railID, DOWN });
        }
    }
    if (ev & PDJE_MOUSE_L_BTN_UP) {
        rule.MatchDetail = DEVICE_MOUSE_EVENT::BTN_L;
        if (FindRailID(rule, Cached.railID)) {
            Cached.mouse_btn_event_queue.push_back({ Cached.railID, UP });
        }
    }
    if (ev & PDJE_MOUSE_R_BTN_DOWN) {
        rule.MatchDetail = DEVICE_MOUSE_EVENT::BTN_R;
        if (FindRailID(rule, Cached.railID)) {
            Cached.mouse_btn_event_queue.push_back({ Cached.railID, DOWN });
        }
    }
    if (ev & PDJE_MOUSE_R_BTN_UP) {
        rule.MatchDetail = DEVICE_MOUSE_EVENT::BTN_R;
        if (FindRailID(rule, Cached.railID)) {
            Cached.mouse_btn_event_queue.push_back({ Cached.railID, UP });
        }
    }
    if (ev & PDJE_MOUSE_M_BTN_DOWN) {
        rule.MatchDetail = DEVICE_MOUSE_EVENT::BTN_M;
        if (FindRailID(rule, Cached.railID)) {
            Cached.mouse_btn_event_queue.push_back({ Cached.railID, DOWN });
        }
    }
    if (ev & PDJE_MOUSE_M_BTN_UP) {
        rule.MatchDetail = DEVICE_MOUSE_EVENT::BTN_M;
        if (FindRailID(rule, Cached.railID)) {
            Cached.mouse_btn_event_queue.push_back({ Cached.railID, UP });
        }
    }
    if (ev & PDJE_MOUSE_SIDE_BTN_DOWN) {
        rule.MatchDetail = DEVICE_MOUSE_EVENT::BTN_SIDE;
        if (FindRailID(rule, Cached.railID)) {
            Cached.mouse_btn_event_queue.push_back({ Cached.railID, DOWN });
        }
    }
    if (ev & PDJE_MOUSE_SIDE_BTN_UP) {
        rule.MatchDetail = DEVICE_MOUSE_EVENT::BTN_SIDE;
        if (FindRailID(rule, Cached.railID)) {
            Cached.mouse_btn_event_queue.push_back({ Cached.railID, UP });
        }
    }
    if (ev & PDJE_MOUSE_EX_BTN_DOWN) {
        rule.MatchDetail = DEVICE_MOUSE_EVENT::BTN_EX;
        if (FindRailID(rule, Cached.railID)) {
            Cached.mouse_btn_event_queue.push_back({ Cached.railID, DOWN });
        }
    }
    if (ev & PDJE_MOUSE_EX_BTN_UP) {
        rule.MatchDetail = DEVICE_MOUSE_EVENT::BTN_EX;
        if (FindRailID(rule, Cached.railID)) {
            Cached.mouse_btn_event_queue.push_back({ Cached.railID, UP });
        }
    }
    if (ev & PDJE_MOUSE_XWHEEL) {
        rule.MatchDetail = DEVICE_MOUSE_EVENT::WHEEL_X;
        if (FindRailID(rule, Cached.railID)) {
            Cached.mouse_btn_event_queue.push_back({ Cached.railID, X });
        }
    }
    if (ev & PDJE_MOUSE_YWHEEL) {
        rule.MatchDetail = DEVICE_MOUSE_EVENT::WHEEL_Y;
        if (FindRailID(rule, Cached.railID)) {
            Cached.mouse_btn_event_queue.push_back({ Cached.railID, Y });
        }
    }
}

template <>
void
Judge_Loop::UseEvent<PDJE_Dev_Type::MOUSE>(const PDJE_Input_Log &ilog)
{
    Cached.mouse_btn_event_queue.clear();

    INPUT_RULE rule;
    rule.Device_ID = ilog.id;
    rule.MatchType = ilog.type;
    ParseMouse(rule, ilog.event.mouse.button_type);

    for (const auto &mev : Cached.mouse_btn_event_queue) {
        switch (mev.status) {
        case DOWN:
            init_datas->note_objects->Get<BUFFER_MAIN>(
                Cached.use_range, mev.rail_id, Cached.found_list, Cached.synced_data.consumed_frames, Cached.synced_data.microsecond);
            Match(ilog.microSecond, Cached.found_list, mev.rail_id, true);
            break;
        case UP:
            init_datas->note_objects->Get<BUFFER_SUB>(
                Cached.use_range, mev.rail_id, Cached.found_list, Cached.synced_data.consumed_frames, Cached.synced_data.microsecond);
            Match(ilog.microSecond, Cached.found_list, mev.rail_id, false);
            break;
        case X:
            if (ilog.event.mouse.wheel_move > 0) {
                init_datas->note_objects->Get<BUFFER_MAIN>(
                    Cached.use_range, mev.rail_id, Cached.found_list, Cached.synced_data.consumed_frames, Cached.synced_data.microsecond);
                Match(ilog.microSecond, Cached.found_list, mev.rail_id, true);
            } else if (ilog.event.mouse.wheel_move < 0) {
                init_datas->note_objects->Get<BUFFER_SUB>(
                    Cached.use_range, mev.rail_id, Cached.found_list, Cached.synced_data.consumed_frames, Cached.synced_data.microsecond);
                Match(ilog.microSecond, Cached.found_list, mev.rail_id, false);
            }
            break;
        case Y:
            if (ilog.event.mouse.wheel_move > 0) {
                init_datas->note_objects->Get<BUFFER_MAIN>(
                    Cached.use_range, mev.rail_id, Cached.found_list, Cached.synced_data.consumed_frames, Cached.synced_data.microsecond);
                Match(ilog.microSecond, Cached.found_list, mev.rail_id, true);
            } else if (ilog.event.mouse.wheel_move < 0) {
                init_datas->note_objects->Get<BUFFER_SUB>(
                    Cached.use_range, mev.rail_id, Cached.found_list, Cached.synced_data.consumed_frames, Cached.synced_data.microsecond);
                Match(ilog.microSecond, Cached.found_list, mev.rail_id, false);
            }
            break;
        default:
            break;
        }
    }
    if (init_datas->lambdas.custom_axis_parse &&
        (ilog.event.mouse.x != 0 || ilog.event.mouse.y != 0)) {
        rule.MatchDetail = DEVICE_MOUSE_EVENT::AXIS_MOVE;
        if (FindRailID(rule, Cached.railID)) {
            init_datas->note_objects->Get<BUFFER_SUB>(
                Cached.use_range, Cached.railID, Cached.found_list, Cached.synced_data.consumed_frames, Cached.synced_data.microsecond);
            init_datas->lambdas.custom_axis_parse(ilog.microSecond,
                                                  Cached.found_list,
                                                  Cached.railID,
                                                  ilog.event.mouse.x,
                                                  ilog.event.mouse.y,
                                                  ilog.event.mouse.axis_type);
        }
    }
}
}; // namespace PDJE_JUDGE