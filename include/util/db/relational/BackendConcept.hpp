#pragma once

#include "util/db/relational/Types.hpp"

#include <concepts>
#include <string_view>

namespace PDJE_UTIL::db::relational {

template <class Backend>
concept RelationalBackendConcept = requires(typename Backend::config_type cfg,
                                            Backend          backend,
                                            std::string_view sql,
                                            const Params    &params) {
    { Backend::create(cfg) } -> std::same_as<void>;
    { Backend::destroy(cfg) } -> std::same_as<void>;
    { backend.open(cfg) } -> std::same_as<void>;
    { backend.close() } -> std::same_as<void>;
    { backend.execute(sql, params) } -> std::same_as<ExecResult>;
    { backend.query(sql, params) } -> std::same_as<QueryResult>;
    { backend.begin_transaction() } -> std::same_as<void>;
    { backend.commit() } -> std::same_as<void>;
    { backend.rollback() } -> std::same_as<void>;
};

} // namespace PDJE_UTIL::db::relational
