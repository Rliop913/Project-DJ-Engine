#pragma once

#include "util/db/DbTypes.hpp"

#include <concepts>
#include <span>
#include <string_view>
#include <vector>

namespace PDJE_UTIL::db::keyvalue {

template <class Backend>
concept KeyValueBackendConcept = requires(typename Backend::config_type cfg,
                                          Backend backend,
                                          std::string_view key,
                                          std::span<const std::byte> bytes,
                                          std::string_view text) {
    { Backend::create(cfg) } -> std::same_as<void>;
    { Backend::destroy(cfg) } -> std::same_as<void>;
    { backend.open(cfg) } -> std::same_as<void>;
    { backend.close() } -> std::same_as<void>;
    { backend.contains(key) } -> std::same_as<bool>;
    { backend.get_text(key) } -> std::same_as<Text>;
    { backend.get_bytes(key) } -> std::same_as<Bytes>;
    { backend.put_text(key, text) } -> std::same_as<void>;
    { backend.put_bytes(key, bytes) } -> std::same_as<void>;
    { backend.erase(key) } -> std::same_as<void>;
    { backend.list_keys(std::string_view {}) } -> std::same_as<std::vector<Key>>;
};

} // namespace PDJE_UTIL::db::keyvalue
