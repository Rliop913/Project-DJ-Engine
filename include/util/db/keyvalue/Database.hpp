#pragma once

#include "util/db/detail/Lifecycle.hpp"
#include "util/db/keyvalue/BackendConcept.hpp"

#include <span>
#include <utility>

namespace PDJE_UTIL::db::keyvalue {

template <KeyValueBackendConcept Backend> class KeyValueDatabase {
  public:
    using backend_type = Backend;
    using config_type  = typename Backend::config_type;

    Backend backend{};
    bool    is_open = false;

    static void
    create(const config_type &config)
    {
        Backend::create(config);
    }
    static void
    destroy(const config_type &config)
    {
        Backend::destroy(config);
    }

    static KeyValueDatabase
    open(const config_type &config)
    {
        KeyValueDatabase database;
        detail::open_backend(database.backend, database.is_open, config);
        return database;
    }

    KeyValueDatabase() = default;
    KeyValueDatabase(KeyValueDatabase &&other) noexcept
    {
        detail::take_backend_state(
            backend, is_open, std::move(other.backend), other.is_open);
    }
    KeyValueDatabase &
    operator=(KeyValueDatabase &&other) noexcept
    {
        if (this != &other) {
            backend = std::move(other.backend);
            is_open = std::exchange(other.is_open, false);
        }
        return *this;
    }
    KeyValueDatabase(const KeyValueDatabase &) = delete;
    KeyValueDatabase &
    operator=(const KeyValueDatabase &) = delete;
    ~KeyValueDatabase()                 = default;

    void
    close()
    {
        detail::close_if_open(backend, is_open);
    }
    bool
    contains(std::string_view key) const
    {
        return backend.contains(key);
    }
    Text
    get_text(std::string_view key) const
    {
        return backend.get_text(key);
    }
    Bytes
    get_bytes(std::string_view key) const
    {
        return backend.get_bytes(key);
    }
    void
    put_text(std::string_view key, std::string_view value)
    {
        backend.put_text(key, value);
    }
    void
    put_bytes(std::string_view key, std::span<const std::byte> value)
    {
        backend.put_bytes(key, value);
    }
    void
    erase(std::string_view key)
    {
        backend.erase(key);
    }
    std::vector<Key>
    list_keys(std::string_view prefix = {}) const
    {
        return backend.list_keys(prefix);
    }
};

} // namespace PDJE_UTIL::db::keyvalue
