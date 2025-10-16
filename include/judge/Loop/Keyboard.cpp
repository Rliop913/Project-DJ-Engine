#include "PDJE_Judge_Loop.hpp"

namespace PDJE_JUDGE {

template <>
void
Judge_Loop::UseEvent<PDJE_Dev_Type::KEYBOARD>(const PDJE_Input_Log &ilog)
{
    INPUT_RULE rule;
    rule.Device_ID   = ilog.id;
    rule.MatchType   = ilog.type;
    rule.MatchDetail = ilog.event.keyboard.k;

    if (!FindRailID(rule, railID)) {
        return;
    }
    if (ilog.event.keyboard.pressed) {
        init_datas->note_objects->Get<BUFFER_MAIN>(
            use_range_time, railID, found_list);
        Match(ilog.microSecond, found_list, railID, true);
    } else {
        init_datas->note_objects->Get<BUFFER_SUB>(
            use_range_time, railID, found_list);
        Match(ilog.microSecond, found_list, railID, false);
    }
}
}; // namespace PDJE_JUDGE