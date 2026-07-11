#pragma once

#include "util/db/nearest/Types.hpp"

#include <concepts>
#include <span>
#include <string_view>
#include <vector>

namespace PDJE_UTIL::db::nearest {

template <class Backend>
concept NearestNeighborBackendConcept =
    requires(typename Backend::config_type cfg,
             Backend                       backend,
             std::string_view              key,
             const Item                   &item,
             std::span<const float>        query,
             SearchOptions                 options) {
        { Backend::create(cfg) } -> std::same_as<void>;
        { Backend::destroy(cfg) } -> std::same_as<void>;
        { backend.open(cfg) } -> std::same_as<void>;
        { backend.close() } -> std::same_as<void>;
        { backend.contains(key) } -> std::same_as<bool>;
        { backend.get_item(key) } -> std::same_as<Item>;
        { backend.upsert_item(item) } -> std::same_as<void>;
        { backend.erase_item(key) } -> std::same_as<void>;
        {
            backend.search(query, options)
        } -> std::same_as<std::vector<SearchHit>>;
        { backend.list_keys() } -> std::same_as<std::vector<Key>>;
    };

} // namespace PDJE_UTIL::db::nearest
