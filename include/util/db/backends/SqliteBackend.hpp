#pragma once

#include "global/PDJE_EXPORT_SETTER.hpp"
#include "util/db/DbTypes.hpp"
#include "util/db/relational/Types.hpp"

#include <filesystem>
#include <string_view>

struct sqlite3;

namespace PDJE_UTIL::db::backends {

struct SqliteConfig {
    std::filesystem::path path;
    OpenOptions open_options {};
};

class PDJE_API SqliteBackend {
  public:
    using config_type = SqliteConfig;

    SqliteBackend() = default;
    ~SqliteBackend();
    SqliteBackend(SqliteBackend &&other) noexcept;
    SqliteBackend &operator=(SqliteBackend &&other) noexcept;
    SqliteBackend(const SqliteBackend &) = delete;
    SqliteBackend &operator=(const SqliteBackend &) = delete;

    static void create(const config_type &config);
    static void destroy(const config_type &config);
    void open(const config_type &config);
    void close();
    relational::ExecResult execute(std::string_view sql,
                                   const relational::Params &params);
    relational::QueryResult query(std::string_view sql,
                                  const relational::Params &params) const;
    void begin_transaction();
    void commit();
    void rollback();

  private:
    void require_open() const;
    void execute_command(const char *sql, std::string_view context);

    config_type config_ {};
    sqlite3 *db_ = nullptr;
};

} // namespace PDJE_UTIL::db::backends
