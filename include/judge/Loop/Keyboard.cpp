#include "PDJE_Judge_Init_Structs.hpp"
#include "PDJE_Judge_Loop.hpp"
#include "PDJE_Note_OBJ.hpp"
#include <cstdint>

namespace PDJE_JUDGE {

template <>
void
Judge_Loop::UseEvent<PDJE_Dev_Type::KEYBOARD>(const PDJE_Input_Log &ilog)
{
    Cached.meta.Device_ID.assign(ilog.id, ilog.id_len);
    Cached.meta.DeviceKey = ilog.event.keyboard.k;
    Cached.railid = init_datas->devparser.railData[Cached.meta].MatchRail;
    if (ilog.event.keyboard.pressed) {
        init_datas->note_objects->Get<BUFFER_MAIN>(
            Cached.use_range, Cached.railid, Cached.found_list);
        Match(ilog.microSecond, Cached.found_list, Cached.railid, true);
    } else {
        init_datas->note_objects->Get<BUFFER_SUB>(
            Cached.use_range, Cached.railid, Cached.found_list);
        Match(ilog.microSecond, Cached.found_list, Cached.railid, false);
    }
}
}; // namespace PDJE_JUDGE