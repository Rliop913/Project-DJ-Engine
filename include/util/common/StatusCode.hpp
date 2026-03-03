#pragma once

namespace PDJE_UTIL::common {

enum class StatusCode {
    ok,
    invalid_argument,
    not_found,
    type_mismatch,
    unsupported,
    io_error,
    closed,
    backend_error,
    out_of_range,
    internal_error
};

} // namespace PDJE_UTIL::common
