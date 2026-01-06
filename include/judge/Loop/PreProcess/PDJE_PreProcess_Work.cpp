#include "PDJE_PreProcess.hpp"

namespace PDJE_JUDGE {
bool
PreProcess::Work()
{
    synced_data = init->coreline->syncD->load(std::memory_order_acquire);

    local_micro_pos =
        Convert_Frame_Into_MicroSecond(synced_data.consumed_frames);
    global_local_diff = synced_data.microsecond - local_micro_pos;

    if (!GetDatas()) { // no input datas
        if (local_micro_pos < init->ev_rule->miss_range_microsecond) {
            Cut(0);
        } else {
            Cut(local_micro_pos - init->ev_rule->miss_range_microsecond);
        }
        return false;
    }

    if (parsed_res.lowest < global_local_diff) {
        parsed_res.lowest = 0;
    } else {
        parsed_res.lowest -= global_local_diff;
    }
    if (parsed_res.lowest < init->ev_rule->miss_range_microsecond) {
        Cut(0);
    } else {
        Cut(parsed_res.lowest - init->ev_rule->miss_range_microsecond);
    }

    // init maximum get time
    if (parsed_res.highest < global_local_diff) {
        parsed_res.highest = 0;
    } else {
        parsed_res.highest -= global_local_diff;
    }
    use_range = parsed_res.highest + init->ev_rule->use_range_microsecond;
    for (auto &log : parsed_res.logs) {
        if (log.microSecond < global_local_diff) {
            log.microSecond = 0;
        } else {
            log.microSecond -= global_local_diff;
        }
    }
    return true;
}
}; // namespace PDJE_JUDGE