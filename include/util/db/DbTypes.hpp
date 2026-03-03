#pragma once

#include <cstddef>
#include <string>
#include <vector>

namespace PDJE_UTIL::db {

using Key   = std::string;
using Text  = std::string;
using Bytes = std::vector<std::byte>;

struct OpenOptions {
    bool create_if_missing  = false;
    bool truncate_if_exists = false;
    bool read_only          = false;
};

} // namespace PDJE_UTIL::db
