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

    if (!FindRailID(rule, Cached.railID)) {
        return;
    }
    if (ilog.event.keyboard.pressed) {
        init_datas->note_objects->Get<BUFFER_MAIN>(
            Cached.use_range, Cached.railID, Cached.found_list, Cached.synced_data.consumed_frames, Cached.synced_data.microsecond);
        Match(ilog.microSecond, Cached.found_list, Cached.railID, true);
    } else {
        init_datas->note_objects->Get<BUFFER_SUB>(
            Cached.use_range, Cached.railID, Cached.found_list, Cached.synced_data.consumed_frames, Cached.synced_data.microsecond);
        Match(ilog.microSecond, Cached.found_list, Cached.railID, false);
    }
}
}; // namespace PDJE_JUDGE