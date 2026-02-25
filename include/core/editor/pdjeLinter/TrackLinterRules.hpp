#pragma once

#include "fileNameSanitizer.hpp"

#include <cstdint>
#include <string>
#include <unordered_map>

namespace PDJE_TRACK_LINTER_RULES {

using TrackLinterIdCount = std::unordered_map<int32_t, int>;

inline void
AccumulateIf(TrackLinterIdCount &counts, const int32_t id, const bool should_count)
{
    if (!counts.contains(id)) {
        counts[id] = 0;
    }
    if (should_count) {
        counts[id] += 1;
    }
}

inline bool
ValidateExactlyOne(const TrackLinterIdCount &counts,
                   const char               *label,
                   UNSANITIZED              &msg)
{
    bool ok = true;
    for (const auto &entry : counts) {
        if (entry.second != 1) {
            ok = false;
            msg += " ID " + std::to_string(entry.first) + " has ";
            if (entry.second > 1) {
                msg += std::to_string(entry.second) + " " + label +
                       " command. render failed.\n";
            } else {
                msg += "no " + std::string(label) + " command. render failed.\n";
            }
        }
    }
    return ok;
}

} // namespace PDJE_TRACK_LINTER_RULES
