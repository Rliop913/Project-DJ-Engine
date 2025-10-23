#include "PDJE_Judge_Loop.hpp"
#include "PDJE_Note_OBJ.hpp"

namespace PDJE_JUDGE {

template <>
void
Judge_Loop::UseEvent<PDJE_Dev_Type::KEYBOARD>(const PDJE_Input_Log &ilog)
{
    if (ilog.microSecond < Cached.global_local_diff) {
        std::cout << "cutted on first" << std::endl;
        return;
    }
    INPUT_RULE rule;
    
    rule.Device_ID  = (*init_datas->inputline->id_name_conv)[ilog.id];    
    rule.DeviceType = ilog.type;
    rule.DeviceKey  = ilog.event.keyboard.k;
    
    if (!FindRailID(rule, Cached.railID)) {
        std::cout << "cutted on second" << std::endl;
        return;
    }
    if (ilog.event.keyboard.pressed) {
        std::cout << "use range: " << Cached.use_range << std::endl;
        init_datas->note_objects->Get<BUFFER_MAIN>(
            Cached.use_range, Cached.railID, Cached.found_list);
        Match(ilog.microSecond - Cached.global_local_diff,
              Cached.found_list,
              Cached.railID,
              true);
    } else {
        init_datas->note_objects->Get<BUFFER_SUB>(
            Cached.use_range, Cached.railID, Cached.found_list);
        Match(ilog.microSecond - Cached.global_local_diff,
              Cached.found_list,
              Cached.railID,
              false);
    }
}
}; // namespace PDJE_JUDGE