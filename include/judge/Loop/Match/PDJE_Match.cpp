#include "PDJE_Match.hpp"
#include <limits>
namespace PDJE_JUDGE {

void
Match::Work(const LOCAL_TIME  input_time,
            const P_NOTE_VEC &note_list,
            const uint64_t    railid,
            const bool        isPressed)
{
    NOTE    *best_note    = nullptr;
    bool     best_is_late = false;
    uint64_t best_diff    = std::numeric_limits<uint64_t>::max();
    const auto use_range  = init->ev_rule->use_range_microsecond;

    for (const auto &note_local : note_list) {
        const bool isLate = note_local->microsecond < input_time;
        const auto diff   = static_cast<uint64_t>(
            isLate ? input_time - note_local->microsecond
                   : note_local->microsecond - input_time);

        if (!isLate) {
            if (diff > use_range) {
                break;
            }
            if (best_note != nullptr && diff > best_diff) {
                break;
            }
        }

        if (diff > use_range) {
            continue;
        }

        if (best_note == nullptr || diff < best_diff ||
            (diff == best_diff && isLate && !best_is_late)) {
            best_note    = note_local;
            best_is_late = isLate;
            best_diff    = diff;
        }
    }

    if (best_note == nullptr) {
        return;
    }

    best_note->used = true;
    pre->Event_Datas.use_queue.Write(
        { railid, isPressed, best_is_late, best_diff });
}

}; // namespace PDJE_JUDGE
