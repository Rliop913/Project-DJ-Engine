#pragma once

#include "util/common/StatusCode.hpp"

#include <string>

namespace PDJE_UTIL::common {

struct Status {
    StatusCode  code    = StatusCode::ok;
    std::string message = {};

    constexpr bool
    ok() const noexcept
    {
        return code == StatusCode::ok;
    }
};

} // namespace PDJE_UTIL::common
