#pragma once

#include <algorithm>
#include <stdexcept>

namespace PDJE_UTIL::function {

struct ClampArgs {
    double value     = 0.0;
    double min_value = 0.0;
    double max_value = 1.0;
};

inline double
clamp(ClampArgs args)
{
    if (args.min_value > args.max_value) {
        throw std::invalid_argument("ClampArgs.min_value must be less than or "
                                    "equal to ClampArgs.max_value.");
    }

    return std::clamp(args.value, args.min_value, args.max_value);
}

} // namespace PDJE_UTIL::function
