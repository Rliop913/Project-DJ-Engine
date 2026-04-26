#pragma once

#include "util/db/detail/Lifecycle.hpp"
#include "util/db/relational/BackendConcept.hpp"

#include <utility>

namespace PDJE_UTIL::db::relational {

template <RelationalBackendConcept Backend> class RelationalDatabase {
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

    static common::Result<RelationalDatabase>
    open(const config_type &cfg)
    {
        RelationalDatabase db;
        auto               opened =
            detail::open_backend(db.backend_, db.is_open_, cfg);
        if (!opened.ok()) {
            return common::Result<RelationalDatabase>::failure(opened.status());
        }
        return common::Result<RelationalDatabase>::success(std::move(db));
    }

    RelationalDatabase() = default;
    RelationalDatabase(RelationalDatabase &&other) noexcept
    {
        detail::take_backend_state(
            backend_, is_open_, std::move(other.backend_), other.is_open_);
    }

    RelationalDatabase &
    operator=(RelationalDatabase &&other) noexcept
    {
        if (this != &other) {
            (void)detail::close_if_open(backend_, is_open_);
            detail::take_backend_state(
                backend_, is_open_, std::move(other.backend_), other.is_open_);
        }
        return *this;
    }

    RelationalDatabase(const RelationalDatabase &) = delete;
    RelationalDatabase &
    operator=(const RelationalDatabase &) = delete;

    ~RelationalDatabase()
    {
        (void)detail::close_if_open(backend_, is_open_);
    }

    common::Result<void>
    close()
    {
        return detail::close_if_open(backend_, is_open_);
    }

    bool
    is_open() const noexcept
    {
        return is_open_;
    }

    common::Result<ExecResult>
    execute(std::string_view sql, const Params &params = {})
    {
        return backend_.execute(sql, params);
    }

    common::Result<QueryResult>
    query(std::string_view sql, const Params &params = {}) const
    {
        return backend_.query(sql, params);
    }

    common::Result<void>
    begin_transaction()
    {
        return backend_.begin_transaction();
    }

    common::Result<void>
    commit()
    {
        return backend_.commit();
    }

    common::Result<void>
    rollback()
    {
        return backend_.rollback();
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

} // namespace PDJE_UTIL::db::relational
