#include "SqliteBackend.hpp"

#include <sqlite3.h>

#include <cctype>
#include <cstring>
#include <stdexcept>
#include <string>
#include <utility>

namespace PDJE_UTIL::db::backends {
namespace {

std::runtime_error
sqlite_error(sqlite3 *db, std::string_view context)
{
    std::string message(context);
    if (db != nullptr && sqlite3_errmsg(db) != nullptr) {
        message.append(": ").append(sqlite3_errmsg(db));
    }
    return std::runtime_error(message);
}

class Statement {
  public:
    Statement(sqlite3 *db, std::string_view sql, std::string_view context)
    {
        if (sqlite3_prepare_v2(db,
                               sql.data(),
                               static_cast<int>(sql.size()),
                               &value,
                               nullptr) != SQLITE_OK) {
            throw sqlite_error(db, context);
        }
    }
    ~Statement()
    {
        sqlite3_finalize(value);
    }
    Statement(const Statement &) = delete;
    Statement &
    operator=(const Statement &) = delete;

    sqlite3_stmt *value = nullptr;
};

void
bind_value(sqlite3_stmt *statement, int index, const relational::Value &value)
{
    int result = SQLITE_OK;
    switch (value.storage.index()) {
    case 0:
        result = sqlite3_bind_null(statement, index);
        break;
    case 1:
        result = sqlite3_bind_int64(
            statement, index, std::get<std::int64_t>(value.storage));
        break;
    case 2:
        result = sqlite3_bind_double(
            statement, index, std::get<double>(value.storage));
        break;
    case 3: {
        const auto &text = std::get<Text>(value.storage);
        result           = sqlite3_bind_text(statement,
                                   index,
                                   text.data(),
                                   static_cast<int>(text.size()),
                                   SQLITE_TRANSIENT);
        break;
    }
    default: {
        const auto &bytes = std::get<Bytes>(value.storage);
        result            = sqlite3_bind_blob(statement,
                                   index,
                                   bytes.data(),
                                   static_cast<int>(bytes.size()),
                                   SQLITE_TRANSIENT);
        break;
    }
    }
    if (result != SQLITE_OK) {
        throw std::runtime_error("Failed to bind SQLite statement parameter.");
    }
}

void
bind_params(sqlite3_stmt *statement, const relational::Params &params)
{
    const int expected = sqlite3_bind_parameter_count(statement);
    if (expected != static_cast<int>(params.size())) {
        throw std::invalid_argument(
            "SQLite parameter count does not match the SQL statement.");
    }
    for (int index = 0; index < expected; ++index) {
        bind_value(
            statement, index + 1, params[static_cast<std::size_t>(index)]);
    }
}

relational::Value
read_value(sqlite3_stmt *statement, int column)
{
    switch (sqlite3_column_type(statement, column)) {
    case SQLITE_INTEGER:
        return { std::int64_t(sqlite3_column_int64(statement, column)) };
    case SQLITE_FLOAT:
        return { sqlite3_column_double(statement, column) };
    case SQLITE_TEXT: {
        const auto *text = sqlite3_column_text(statement, column);
        const int   size = sqlite3_column_bytes(statement, column);
        return { Text(reinterpret_cast<const char *>(text), size) };
    }
    case SQLITE_BLOB: {
        const auto *data = static_cast<const std::byte *>(
            sqlite3_column_blob(statement, column));
        const int size = sqlite3_column_bytes(statement, column);
        Bytes     bytes(static_cast<std::size_t>(size));
        if (size > 0 && data != nullptr) {
            std::memcpy(bytes.data(), data, static_cast<std::size_t>(size));
        }
        return { std::move(bytes) };
    }
    default:
        return { std::monostate{} };
    }
}

bool
starts_with_keyword(std::string_view sql, std::string_view keyword)
{
    while (!sql.empty() &&
           std::isspace(static_cast<unsigned char>(sql.front())) != 0) {
        sql.remove_prefix(1);
    }
    if (sql.size() < keyword.size()) {
        return false;
    }
    for (std::size_t index = 0; index < keyword.size(); ++index) {
        if (std::toupper(static_cast<unsigned char>(sql[index])) !=
            std::toupper(static_cast<unsigned char>(keyword[index]))) {
            return false;
        }
    }
    return true;
}

void
validate_config(const SqliteConfig &config)
{
    if (config.path.empty()) {
        throw std::invalid_argument("SqliteConfig.path must not be empty.");
    }
    if (config.open_options.read_only &&
        (config.open_options.create_if_missing ||
         config.open_options.truncate_if_exists)) {
        throw std::invalid_argument(
            "SQLite read-only mode cannot create or truncate the database.");
    }
}

} // namespace

SqliteBackend::~SqliteBackend()
{
    if (db_ != nullptr) {
        sqlite3_close_v2(db_);
    }
}

SqliteBackend::SqliteBackend(SqliteBackend &&other) noexcept
    : config_(std::move(other.config_)), db_(std::exchange(other.db_, nullptr))
{
}

SqliteBackend &
SqliteBackend::operator=(SqliteBackend &&other) noexcept
{
    if (this != &other) {
        if (db_ != nullptr) {
            sqlite3_close_v2(db_);
        }
        config_ = std::move(other.config_);
        db_     = std::exchange(other.db_, nullptr);
    }
    return *this;
}

void
SqliteBackend::create(const config_type &config)
{
    validate_config(config);
    std::error_code error;
    if (const auto parent = config.path.parent_path(); !parent.empty()) {
        std::filesystem::create_directories(parent, error);
    }
    if (error) {
        throw std::runtime_error("Failed to create SQLite directory: " +
                                 error.message());
    }
    sqlite3 *database = nullptr;
    if (sqlite3_open_v2(config.path.string().c_str(),
                        &database,
                        SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE |
                            SQLITE_OPEN_NOMUTEX,
                        nullptr) != SQLITE_OK) {
        auto error_value =
            sqlite_error(database, "Failed to create SQLite database");
        sqlite3_close_v2(database);
        throw error_value;
    }
    sqlite3_close_v2(database);
}

void
SqliteBackend::destroy(const config_type &config)
{
    if (config.path.empty()) {
        throw std::invalid_argument("SqliteConfig.path must not be empty.");
    }
    std::error_code error;
    std::filesystem::remove(config.path, error);
    if (error) {
        throw std::runtime_error("Failed to remove SQLite database: " +
                                 error.message());
    }
    for (const auto suffix : { "-wal", "-shm", "-journal" }) {
        error.clear();
        std::filesystem::remove(config.path.string() + suffix, error);
    }
}

void
SqliteBackend::open(const config_type &config)
{
    if (db_ != nullptr) {
        throw std::logic_error("SQLite backend is already open.");
    }
    validate_config(config);
    if (config.open_options.truncate_if_exists) {
        destroy(config);
    }
    if (!std::filesystem::exists(config.path)) {
        if (!config.open_options.create_if_missing) {
            throw std::out_of_range("SQLite database file does not exist.");
        }
        create(config);
    }
    int flags = SQLITE_OPEN_NOMUTEX |
                (config.open_options.read_only ? SQLITE_OPEN_READONLY
                                               : SQLITE_OPEN_READWRITE);
    if (config.open_options.create_if_missing &&
        !config.open_options.read_only) {
        flags |= SQLITE_OPEN_CREATE;
    }
    if (sqlite3_open_v2(config.path.string().c_str(), &db_, flags, nullptr) !=
        SQLITE_OK) {
        auto error_value = sqlite_error(db_, "Failed to open SQLite database");
        sqlite3_close_v2(db_);
        db_ = nullptr;
        throw error_value;
    }
    config_ = config;
    sqlite3_extended_result_codes(db_, 1);
}

void
SqliteBackend::close()
{
    if (db_ == nullptr) {
        return;
    }
    if (sqlite3_close_v2(db_) != SQLITE_OK) {
        throw sqlite_error(db_, "Failed to close SQLite database");
    }
    db_ = nullptr;
}

void
SqliteBackend::require_open() const
{
    if (db_ == nullptr) {
        throw std::logic_error("SQLite backend is not open.");
    }
}

relational::ExecResult
SqliteBackend::execute(std::string_view sql, const relational::Params &params)
{
    require_open();
    Statement statement(db_, sql, "Failed to prepare SQLite statement");
    bind_params(statement.value, params);
    int result = SQLITE_ROW;
    while (result == SQLITE_ROW) {
        result = sqlite3_step(statement.value);
    }
    if (result != SQLITE_DONE) {
        throw sqlite_error(db_, "SQLite execute failed");
    }
    relational::ExecResult output{ .affected_rows = static_cast<std::uint64_t>(
                                       sqlite3_changes64(db_)) };
    if (starts_with_keyword(sql, "INSERT")) {
        output.last_insert_rowid = sqlite3_last_insert_rowid(db_);
    }
    return output;
}

relational::QueryResult
SqliteBackend::query(std::string_view          sql,
                     const relational::Params &params) const
{
    require_open();
    Statement statement(db_, sql, "Failed to prepare SQLite query");
    bind_params(statement.value, params);
    relational::QueryResult output;
    const int               columns = sqlite3_column_count(statement.value);
    int                     result  = SQLITE_OK;
    while ((result = sqlite3_step(statement.value)) == SQLITE_ROW) {
        relational::Row row;
        for (int column = 0; column < columns; ++column) {
            const char *name = sqlite3_column_name(statement.value, column);
            row.columns.emplace_back(name != nullptr ? name : "");
            row.values.push_back(read_value(statement.value, column));
        }
        output.rows.push_back(std::move(row));
    }
    if (result != SQLITE_DONE) {
        throw sqlite_error(db_, "SQLite query failed");
    }
    return output;
}

void
SqliteBackend::execute_command(const char *sql, std::string_view context)
{
    require_open();
    if (sqlite3_exec(db_, sql, nullptr, nullptr, nullptr) != SQLITE_OK) {
        throw sqlite_error(db_, context);
    }
}

void
SqliteBackend::begin_transaction()
{
    execute_command("BEGIN TRANSACTION;", "Failed to begin SQLite transaction");
}
void
SqliteBackend::commit()
{
    execute_command("COMMIT;", "Failed to commit SQLite transaction");
}
void
SqliteBackend::rollback()
{
    execute_command("ROLLBACK;", "Failed to rollback SQLite transaction");
}

} // namespace PDJE_UTIL::db::backends
