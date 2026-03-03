#pragma once

#include "util/common/Result.hpp"
#include "util/db/relational/Types.hpp"

#include <concepts>
#include <string_view>

namespace PDJE_UTIL::db::relational {

template <class Backend>
concept RelationalBackendConcept = requires(typename Backend::config_type cfg,
                                            Backend                      backend,
                                            std::string_view             sql,
                                            const Params                &params) {
    typename Backend::config_type;

    { Backend::create(cfg) } -> std::same_as<common::Result<void>>;
    { Backend::destroy(cfg) } -> std::same_as<common::Result<void>>;

    { backend.open(cfg) } -> std::same_as<common::Result<void>>;
    { backend.close() } -> std::same_as<common::Result<void>>;

    { backend.execute(sql, params) } -> std::same_as<common::Result<ExecResult>>;
    { backend.query(sql, params) } -> std::same_as<common::Result<QueryResult>>;
    { backend.begin_transaction() } -> std::same_as<common::Result<void>>;
    { backend.commit() } -> std::same_as<common::Result<void>>;
    { backend.rollback() } -> std::same_as<common::Result<void>>;
};

} // namespace PDJE_UTIL::db::relational
