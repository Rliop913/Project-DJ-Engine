#include "PDJE_Judge_Init_Structs.hpp"
#include "PDJE_Match.hpp"
#include "PDJE_Note_OBJ.hpp"
#include <cstdint>

namespace PDJE_JUDGE {

template <>
void
Match::UseEvent<PDJE_Dev_Type::KEYBOARD>(const PDJE_Input_Log &ilog)
{
    RAIL_META meta;
    meta.Device_Name.assign(ilog.name, ilog.name_len);
    meta.DeviceKey = ilog.event.keyboard.k;
    auto id        = QueryRailid(meta);

    if (!id) {
        return;
    }

    if (ilog.event.keyboard.pressed) {
        init->note_objects->Get<BUFFER_MAIN>(
            pre->use_range, id.value(), found_list);
        Work(ilog.microSecond, found_list, id.value(), true);
    } else {
        init->note_objects->Get<BUFFER_SUB>(
            pre->use_range, id.value(), found_list);
        Work(ilog.microSecond, found_list, id.value(), false);
    }
}
}; // namespace PDJE_JUDGE