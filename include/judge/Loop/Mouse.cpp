#include "PDJE_Judge_Loop.hpp"

namespace PDJE_JUDGE {
constexpr int DOWN = 0;
constexpr int UP   = 1;
constexpr int X    = 2;
constexpr int Y    = 3;

void
Judge_Loop::ParseMouse(INPUT_RULE                  &rule,
                       const BITMASK                ev,
                       std::vector<railid_is_Down> &res)
{
    if (ev & PDJE_MOUSE_L_BTN_DOWN) {
        rule.MatchDetail = DEVICE_MOUSE_EVENT::BTN_L;
        if (FindRailID(rule, railID)) {
            res.push_back({ railID, DOWN });
        }
    }
    if (ev & PDJE_MOUSE_L_BTN_UP) {
        rule.MatchDetail = DEVICE_MOUSE_EVENT::BTN_L;
        if (FindRailID(rule, railID)) {
            res.push_back({ railID, UP });
        }
    }
    if (ev & PDJE_MOUSE_R_BTN_DOWN) {
        rule.MatchDetail = DEVICE_MOUSE_EVENT::BTN_R;
        if (FindRailID(rule, railID)) {
            res.push_back({ railID, DOWN });
        }
    }
    if (ev & PDJE_MOUSE_R_BTN_UP) {
        rule.MatchDetail = DEVICE_MOUSE_EVENT::BTN_R;
        if (FindRailID(rule, railID)) {
            res.push_back({ railID, UP });
        }
    }
    if (ev & PDJE_MOUSE_M_BTN_DOWN) {
        rule.MatchDetail = DEVICE_MOUSE_EVENT::BTN_M;
        if (FindRailID(rule, railID)) {
            res.push_back({ railID, DOWN });
        }
    }
    if (ev & PDJE_MOUSE_M_BTN_UP) {
        rule.MatchDetail = DEVICE_MOUSE_EVENT::BTN_M;
        if (FindRailID(rule, railID)) {
            res.push_back({ railID, UP });
        }
    }
    if (ev & PDJE_MOUSE_SIDE_BTN_DOWN) {
        rule.MatchDetail = DEVICE_MOUSE_EVENT::BTN_SIDE;
        if (FindRailID(rule, railID)) {
            res.push_back({ railID, DOWN });
        }
    }
    if (ev & PDJE_MOUSE_SIDE_BTN_UP) {
        rule.MatchDetail = DEVICE_MOUSE_EVENT::BTN_SIDE;
        if (FindRailID(rule, railID)) {
            res.push_back({ railID, UP });
        }
    }
    if (ev & PDJE_MOUSE_EX_BTN_DOWN) {
        rule.MatchDetail = DEVICE_MOUSE_EVENT::BTN_EX;
        if (FindRailID(rule, railID)) {
            res.push_back({ railID, DOWN });
        }
    }
    if (ev & PDJE_MOUSE_EX_BTN_UP) {
        rule.MatchDetail = DEVICE_MOUSE_EVENT::BTN_EX;
        if (FindRailID(rule, railID)) {
            res.push_back({ railID, UP });
        }
    }
    if (ev & PDJE_MOUSE_XWHEEL) {
        rule.MatchDetail = DEVICE_MOUSE_EVENT::WHEEL_X;
        if (FindRailID(rule, railID)) {
            res.push_back({ railID, X });
        }
    }
    if (ev & PDJE_MOUSE_YWHEEL) {
        rule.MatchDetail = DEVICE_MOUSE_EVENT::WHEEL_Y;
        if (FindRailID(rule, railID)) {
            res.push_back({ railID, Y });
        }
    }
}

template <>
void
Judge_Loop::UseEvent<PDJE_Dev_Type::MOUSE>(const PDJE_Input_Log &ilog)
{
    temp_rules.clear();
    INPUT_RULE rule;
    rule.Device_ID = ilog.id;
    rule.MatchType = ilog.type;
    ParseMouse(rule, ilog.event.mouse.button_type, temp_rules);

    for (const auto &mev : temp_rules) {
        switch (mev.status) {
        case DOWN:
            init_datas->note_objects->Get<BUFFER_MAIN>(
                use_range_time, mev.id, found_list);
            Match(ilog.microSecond, found_list, mev.id, true);
            break;
        case UP:
            init_datas->note_objects->Get<BUFFER_SUB>(
                use_range_time, mev.id, found_list);
            Match(ilog.microSecond, found_list, mev.id, false);
            break;
        case X:
            if (ilog.event.mouse.wheel_move > 0) {
                init_datas->note_objects->Get<BUFFER_MAIN>(
                    use_range_time, mev.id, found_list);
                Match(ilog.microSecond, found_list, mev.id, true);
            } else if (ilog.event.mouse.wheel_move < 0) {
                init_datas->note_objects->Get<BUFFER_SUB>(
                    use_range_time, mev.id, found_list);
                Match(ilog.microSecond, found_list, mev.id, false);
            }
            break;
        case Y:
            if (ilog.event.mouse.wheel_move > 0) {
                init_datas->note_objects->Get<BUFFER_MAIN>(
                    use_range_time, mev.id, found_list);
                Match(ilog.microSecond, found_list, mev.id, true);
            } else if (ilog.event.mouse.wheel_move < 0) {
                init_datas->note_objects->Get<BUFFER_SUB>(
                    use_range_time, mev.id, found_list);
                Match(ilog.microSecond, found_list, mev.id, false);
            }
            break;
        default:
            break;
        }
    }
    if (init_datas->custom_axis_parse &&
        (ilog.event.mouse.x != 0 || ilog.event.mouse.y != 0)) {
        rule.MatchDetail = DEVICE_MOUSE_EVENT::AXIS_MOVE;
        if (FindRailID(rule, railID)) {
            init_datas->note_objects->Get<BUFFER_SUB>(
                use_range_time, railID, found_list);
            init_datas->custom_axis_parse(ilog.microSecond,
                                          found_list,
                                          railID,
                                          ilog.event.mouse.x,
                                          ilog.event.mouse.y,
                                          ilog.event.mouse.axis_type);
        }
    }
}
}; // namespace PDJE_JUDGE