#pragma once

#include "util/db/keyvalue/BackendConcept.hpp"

#include <span>
#include <utility>

namespace PDJE_UTIL::db::keyvalue {

template <KeyValueBackendConcept Backend> class KeyValueDatabase {
  public:
    using backend_type = Backend;
    using config_type  = typename Backend::config_type;

    static common::Result<void>
    create(const config_type &cfg)
    {
        return Backend::create(cfg);
    }

    static common::Result<void>
    destroy(const config_type &cfg)
    {
        return Backend::destroy(cfg);
    }

    static common::Result<KeyValueDatabase>
    open(const config_type &cfg)
    {
        KeyValueDatabase db;
        auto             opened = db.backend_.open(cfg);
        if (!opened.ok()) {
            return common::Result<KeyValueDatabase>::failure(opened.status());
        }
        db.is_open_ = true;
        return common::Result<KeyValueDatabase>::success(std::move(db));
    }

    KeyValueDatabase() = default;
    KeyValueDatabase(KeyValueDatabase &&other) noexcept
        : backend_(std::move(other.backend_)),
          is_open_(std::exchange(other.is_open_, false))
    {
    }

    KeyValueDatabase &
    operator=(KeyValueDatabase &&other) noexcept
    {
        if (this != &other) {
            if (is_open_) {
                (void)close();
            }
            backend_ = std::move(other.backend_);
            is_open_ = std::exchange(other.is_open_, false);
        }
        return *this;
    }

    KeyValueDatabase(const KeyValueDatabase &) = delete;
    KeyValueDatabase &
    operator=(const KeyValueDatabase &) = delete;

    ~KeyValueDatabase()
    {
        if (is_open_) {
            (void)close();
        }
    }

    common::Result<void>
    close()
    {
        if (!is_open_) {
            return common::Result<void>::success();
        }
        auto closed = backend_.close();
        if (closed.ok()) {
            is_open_ = false;
        }
        return closed;
    }

    bool
    is_open() const noexcept
    {
        return is_open_;
    }

    common::Result<bool>
    contains(std::string_view key) const
    {
        return backend_.contains(key);
    }

    common::Result<Text>
    get_text(std::string_view key) const
    {
        return backend_.get_text(key);
    }

    common::Result<Bytes>
    get_bytes(std::string_view key) const
    {
        return backend_.get_bytes(key);
    }

    common::Result<void>
    put_text(std::string_view key, std::string_view value)
    {
        return backend_.put_text(key, value);
    }

    common::Result<void>
    put_bytes(std::string_view key, std::span<const std::byte> value)
    {
        return backend_.put_bytes(key, value);
    }

    common::Result<void>
    erase(std::string_view key)
    {
        return backend_.erase(key);
    }

    common::Result<std::vector<Key>>
    list_keys(std::string_view prefix = {}) const
    {
        return backend_.list_keys(prefix);
    }

    backend_type &
    backend() noexcept
    {
        return backend_;
    }

    const backend_type &
    backend() const noexcept
    {
        return backend_;
    }

  private:
    Backend backend_;
    bool    is_open_ = false;
};

} // namespace PDJE_UTIL::db::keyvalue
