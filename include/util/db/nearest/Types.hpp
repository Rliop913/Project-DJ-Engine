#pragma once

#include "util/db/DbTypes.hpp"

#include <optional>
#include <vector>

namespace PDJE_UTIL::db::nearest {

using Embedding = std::vector<float>;

struct Item {
    Key                 id {};
    Embedding           embedding {};
    std::optional<Text> text_payload {};
    std::optional<Bytes> bytes_payload {};
};

struct SearchOptions {
    std::size_t limit    = 10;
    int         search_k = -1;
};

struct SearchHit {
    Key                  id {};
    float                distance = 0.0F;
    std::optional<Text>  text_payload {};
    std::optional<Bytes> bytes_payload {};
};

} // namespace PDJE_UTIL::db::nearest
