#pragma once

#include "util/db/detail/Lifecycle.hpp"
#include "util/db/relational/BackendConcept.hpp"

#include <utility>

namespace PDJE_UTIL::db::relational {

template <RelationalBackendConcept Backend> class RelationalDatabase {
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

    static RelationalDatabase
    open(const config_type &config)
    {
        RelationalDatabase database;
        detail::open_backend(database.backend, database.is_open, config);
        return database;
    }

    RelationalDatabase() = default;
    RelationalDatabase(RelationalDatabase &&other) noexcept
    {
        detail::take_backend_state(
            backend, is_open, std::move(other.backend), other.is_open);
    }
    RelationalDatabase &
    operator=(RelationalDatabase &&other) noexcept
    {
        if (this != &other) {
            backend = std::move(other.backend);
            is_open = std::exchange(other.is_open, false);
        }
        return *this;
    }
    RelationalDatabase(const RelationalDatabase &) = delete;
    RelationalDatabase &
    operator=(const RelationalDatabase &) = delete;
    ~RelationalDatabase()                 = default;

    void
    close()
    {
        detail::close_if_open(backend, is_open);
    }
    ExecResult
    execute(std::string_view sql, const Params &params = {})
    {
        return backend.execute(sql, params);
    }
    QueryResult
    query(std::string_view sql, const Params &params = {}) const
    {
        return backend.query(sql, params);
    }
    void
    begin_transaction()
    {
        backend.begin_transaction();
    }
    void
    commit()
    {
        backend.commit();
    }
    void
    rollback()
    {
        backend.rollback();
    }
};

} // namespace PDJE_UTIL::db::relational
