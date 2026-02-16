#include "PDJE_Match.hpp"
namespace PDJE_JUDGE {

void
Match::Work(const LOCAL_TIME  input_time,
            const P_NOTE_VEC &note_list,
            const uint64_t    railid,
            const bool        isPressed)
{
    bool     isLate;
    uint64_t diff;
    for (const auto &note_local : note_list) {//todo - edit algorithm (make it finds the closest note. not the first one.)

        isLate = note_local->microsecond < input_time;

        diff = isLate ? input_time - note_local->microsecond
                      : note_local->microsecond - input_time;
        if (diff <= init->ev_rule->use_range_microsecond) {
            note_local->used = true;

            pre->Event_Datas.use_queue.Write(
                { railid, isPressed, isLate, diff });

            break;
        }
    }
}

}; // namespace PDJE_JUDGE