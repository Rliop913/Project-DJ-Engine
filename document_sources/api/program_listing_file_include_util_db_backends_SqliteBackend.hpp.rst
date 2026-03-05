
.. _program_listing_file_include_util_db_backends_SqliteBackend.hpp:

Program Listing for File SqliteBackend.hpp
==========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_util_db_backends_SqliteBackend.hpp>` (``include\util\db\backends\SqliteBackend.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include "util/common/Result.hpp"
   #include "util/db/DbTypes.hpp"
   #include "util/db/relational/Types.hpp"
   
   #include <sqlite3.h>
   
   #include <cctype>
   #include <cstddef>
   #include <cstdint>
   #include <cstring>
   #include <filesystem>
   #include <optional>
   #include <string>
   #include <string_view>
   #include <vector>
   
   namespace PDJE_UTIL::db::backends {
   
   namespace detail {
   
   inline common::Status
   sqlite_make_status(sqlite3 *db, common::StatusCode code, std::string_view prefix)
   {
       std::string message(prefix);
       if (db != nullptr) {
           const char *sqlite_message = sqlite3_errmsg(db);
           if (sqlite_message != nullptr) {
               message.append(": ");
               message.append(sqlite_message);
           }
       }
       return { code, std::move(message) };
   }
   
   inline common::Result<void>
   sqlite_prepare(sqlite3             *db,
                  std::string_view      sql,
                  sqlite3_stmt        **stmt,
                  std::string_view      context)
   {
       if (sqlite3_prepare_v2(db, sql.data(), static_cast<int>(sql.size()), stmt, nullptr) !=
           SQLITE_OK) {
           return common::Result<void>::failure(
               sqlite_make_status(db, common::StatusCode::backend_error, context));
       }
       return common::Result<void>::success();
   }
   
   inline common::Result<void>
   sqlite_bind_value(sqlite3_stmt *stmt, int index, const relational::Value &value)
   {
       int rc = SQLITE_OK;
       switch (value.kind()) {
           case relational::ValueKind::null_value:
               rc = sqlite3_bind_null(stmt, index);
               break;
           case relational::ValueKind::integer:
               rc = sqlite3_bind_int64(
                   stmt, index, std::get<std::int64_t>(value.storage));
               break;
           case relational::ValueKind::real:
               rc = sqlite3_bind_double(stmt, index, std::get<double>(value.storage));
               break;
           case relational::ValueKind::text: {
               const auto &text = std::get<Text>(value.storage);
               rc               = sqlite3_bind_text(stmt,
                                      index,
                                      text.data(),
                                      static_cast<int>(text.size()),
                                      SQLITE_TRANSIENT);
               break;
           }
           case relational::ValueKind::bytes: {
               const auto &bytes = std::get<Bytes>(value.storage);
               rc                = sqlite3_bind_blob(stmt,
                                     index,
                                     bytes.data(),
                                     static_cast<int>(bytes.size()),
                                     SQLITE_TRANSIENT);
               break;
           }
       }
   
       if (rc != SQLITE_OK) {
           return common::Result<void>::failure(
               { common::StatusCode::backend_error, "Failed to bind SQLite statement parameter." });
       }
       return common::Result<void>::success();
   }
   
   inline common::Result<void>
   sqlite_bind_params(sqlite3_stmt *stmt, const relational::Params &params)
   {
       const int expected = sqlite3_bind_parameter_count(stmt);
       if (expected != static_cast<int>(params.size())) {
           return common::Result<void>::failure(
               { common::StatusCode::invalid_argument,
                 "SQLite parameter count does not match the SQL statement." });
       }
   
       for (int i = 0; i < expected; ++i) {
           auto bound = sqlite_bind_value(stmt, i + 1, params[static_cast<std::size_t>(i)]);
           if (!bound.ok()) {
               return bound;
           }
       }
       return common::Result<void>::success();
   }
   
   inline relational::Value
   sqlite_read_value(sqlite3_stmt *stmt, int column)
   {
       switch (sqlite3_column_type(stmt, column)) {
           case SQLITE_INTEGER:
               return { std::int64_t(sqlite3_column_int64(stmt, column)) };
           case SQLITE_FLOAT:
               return { sqlite3_column_double(stmt, column) };
           case SQLITE_TEXT: {
               const auto *text = sqlite3_column_text(stmt, column);
               const int   size = sqlite3_column_bytes(stmt, column);
               return { Text(reinterpret_cast<const char *>(text),
                             static_cast<std::size_t>(size)) };
           }
           case SQLITE_BLOB: {
               const auto *blob =
                   static_cast<const std::byte *>(sqlite3_column_blob(stmt, column));
               const int size = sqlite3_column_bytes(stmt, column);
               Bytes     bytes(static_cast<std::size_t>(size));
               if (size > 0 && blob != nullptr) {
                   std::memcpy(bytes.data(), blob, static_cast<std::size_t>(size));
               }
               return { std::move(bytes) };
           }
           case SQLITE_NULL:
           default:
               return { std::monostate {} };
       }
   }
   
   inline bool
   sqlite_starts_with_keyword(std::string_view sql, std::string_view keyword)
   {
       std::size_t offset = 0;
       while (offset < sql.size() &&
              std::isspace(static_cast<unsigned char>(sql[offset])) != 0) {
           ++offset;
       }
   
       if ((sql.size() - offset) < keyword.size()) {
           return false;
       }
   
       for (std::size_t i = 0; i < keyword.size(); ++i) {
           if (std::toupper(static_cast<unsigned char>(sql[offset + i])) !=
               std::toupper(static_cast<unsigned char>(keyword[i]))) {
               return false;
           }
       }
       return true;
   }
   
   } // namespace detail
   
   struct SqliteConfig {
       std::filesystem::path path;
       OpenOptions           open_options {};
   };
   
   class SqliteBackend {
     public:
       using config_type = SqliteConfig;
   
       static common::Result<void>
       create(const config_type &cfg)
       {
           if (cfg.path.empty()) {
               return common::Result<void>::failure(
                   { common::StatusCode::invalid_argument, "SqliteConfig.path must not be empty." });
           }
   
           auto parent = cfg.path.parent_path();
           if (!parent.empty()) {
               std::error_code ec;
               std::filesystem::create_directories(parent, ec);
               if (ec) {
                   return common::Result<void>::failure(
                       { common::StatusCode::io_error, ec.message() });
               }
           }
   
           sqlite3 *db = nullptr;
           const int rc =
               sqlite3_open_v2(cfg.path.string().c_str(),
                               &db,
                               SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_NOMUTEX,
                               nullptr);
           if (rc != SQLITE_OK) {
               auto status = detail::sqlite_make_status(
                   db, common::StatusCode::backend_error, "Failed to create SQLite database");
               if (db != nullptr) {
                   sqlite3_close_v2(db);
               }
               return common::Result<void>::failure(std::move(status));
           }
   
           sqlite3_close_v2(db);
           return common::Result<void>::success();
       }
   
       static common::Result<void>
       destroy(const config_type &cfg)
       {
           if (cfg.path.empty()) {
               return common::Result<void>::failure(
                   { common::StatusCode::invalid_argument, "SqliteConfig.path must not be empty." });
           }
   
           std::error_code ec;
           std::filesystem::remove(cfg.path, ec);
           if (ec) {
               return common::Result<void>::failure(
                   { common::StatusCode::io_error, ec.message() });
           }
   
           static constexpr std::string_view suffixes[] = { "-wal", "-shm", "-journal" };
           for (const auto suffix : suffixes) {
               ec.clear();
               std::filesystem::remove(cfg.path.string() + std::string(suffix), ec);
           }
   
           return common::Result<void>::success();
       }
   
       common::Result<void>
       open(const config_type &cfg)
       {
           if (db_ != nullptr) {
               return common::Result<void>::failure(
                   { common::StatusCode::invalid_argument, "SQLite backend is already open." });
           }
           if (cfg.path.empty()) {
               return common::Result<void>::failure(
                   { common::StatusCode::invalid_argument, "SqliteConfig.path must not be empty." });
           }
           if (cfg.open_options.read_only &&
               (cfg.open_options.create_if_missing || cfg.open_options.truncate_if_exists)) {
               return common::Result<void>::failure(
                   { common::StatusCode::invalid_argument,
                     "SQLite read-only mode cannot create or truncate the database." });
           }
   
           config_ = cfg;
   
           if (config_.open_options.truncate_if_exists) {
               auto destroyed = destroy(config_);
               if (!destroyed.ok()) {
                   return destroyed;
               }
           }
   
           const bool exists = std::filesystem::exists(config_.path);
           if (!exists) {
               if (config_.open_options.create_if_missing) {
                   auto created = create(config_);
                   if (!created.ok()) {
                       return created;
                   }
               } else {
                   return common::Result<void>::failure(
                       { common::StatusCode::not_found, "SQLite database file does not exist." });
               }
           }
   
           int flags = SQLITE_OPEN_NOMUTEX;
           flags |= config_.open_options.read_only ? SQLITE_OPEN_READONLY
                                                   : SQLITE_OPEN_READWRITE;
           if (config_.open_options.create_if_missing && !config_.open_options.read_only) {
               flags |= SQLITE_OPEN_CREATE;
           }
   
           if (sqlite3_open_v2(config_.path.string().c_str(), &db_, flags, nullptr) !=
               SQLITE_OK) {
               auto status = detail::sqlite_make_status(
                   db_, common::StatusCode::backend_error, "Failed to open SQLite database");
               if (db_ != nullptr) {
                   sqlite3_close_v2(db_);
                   db_ = nullptr;
               }
               return common::Result<void>::failure(std::move(status));
           }
   
           sqlite3_extended_result_codes(db_, 1);
           return common::Result<void>::success();
       }
   
       common::Result<void>
       close()
       {
           if (db_ == nullptr) {
               return common::Result<void>::success();
           }
           if (sqlite3_close_v2(db_) != SQLITE_OK) {
               return common::Result<void>::failure(detail::sqlite_make_status(
                   db_, common::StatusCode::backend_error, "Failed to close SQLite database"));
           }
           db_ = nullptr;
           return common::Result<void>::success();
       }
   
       common::Result<relational::ExecResult>
       execute(std::string_view sql, const relational::Params &params)
       {
           if (auto status = require_open(); !status.ok()) {
               return common::Result<relational::ExecResult>::failure(status);
           }
   
           sqlite3_stmt *stmt = nullptr;
           auto          prepared =
               detail::sqlite_prepare(db_, sql, &stmt, "Failed to prepare SQLite statement");
           if (!prepared.ok()) {
               return common::Result<relational::ExecResult>::failure(prepared.status());
           }
   
           auto bound = detail::sqlite_bind_params(stmt, params);
           if (!bound.ok()) {
               sqlite3_finalize(stmt);
               return common::Result<relational::ExecResult>::failure(bound.status());
           }
   
           int rc = SQLITE_ROW;
           while (rc == SQLITE_ROW) {
               rc = sqlite3_step(stmt);
           }
   
           if (rc != SQLITE_DONE) {
               auto status = detail::sqlite_make_status(
                   db_, common::StatusCode::backend_error, "SQLite execute failed");
               sqlite3_finalize(stmt);
               return common::Result<relational::ExecResult>::failure(std::move(status));
           }
   
           relational::ExecResult result;
           result.affected_rows = static_cast<std::uint64_t>(sqlite3_changes64(db_));
           if (detail::sqlite_starts_with_keyword(sql, "INSERT")) {
               result.last_insert_rowid = sqlite3_last_insert_rowid(db_);
           }
   
           sqlite3_finalize(stmt);
           return common::Result<relational::ExecResult>::success(std::move(result));
       }
   
       common::Result<relational::QueryResult>
       query(std::string_view sql, const relational::Params &params) const
       {
           if (auto status = require_open(); !status.ok()) {
               return common::Result<relational::QueryResult>::failure(status);
           }
   
           sqlite3_stmt *stmt = nullptr;
           auto          prepared =
               detail::sqlite_prepare(db_, sql, &stmt, "Failed to prepare SQLite query");
           if (!prepared.ok()) {
               return common::Result<relational::QueryResult>::failure(prepared.status());
           }
   
           auto bound = detail::sqlite_bind_params(stmt, params);
           if (!bound.ok()) {
               sqlite3_finalize(stmt);
               return common::Result<relational::QueryResult>::failure(bound.status());
           }
   
           relational::QueryResult result;
           const int               column_count = sqlite3_column_count(stmt);
           int                     rc           = SQLITE_ROW;
           while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
               relational::Row row;
               row.columns.reserve(static_cast<std::size_t>(column_count));
               row.values.reserve(static_cast<std::size_t>(column_count));
               for (int i = 0; i < column_count; ++i) {
                   const char *name = sqlite3_column_name(stmt, i);
                   row.columns.emplace_back(name != nullptr ? name : "");
                   row.values.push_back(detail::sqlite_read_value(stmt, i));
               }
               result.rows.push_back(std::move(row));
           }
   
           if (rc != SQLITE_DONE) {
               auto status = detail::sqlite_make_status(
                   db_, common::StatusCode::backend_error, "SQLite query failed");
               sqlite3_finalize(stmt);
               return common::Result<relational::QueryResult>::failure(std::move(status));
           }
   
           sqlite3_finalize(stmt);
           return common::Result<relational::QueryResult>::success(std::move(result));
       }
   
       common::Result<void>
       begin_transaction()
       {
           return exec_sqlite_command("BEGIN TRANSACTION;", "Failed to begin SQLite transaction");
       }
   
       common::Result<void>
       commit()
       {
           return exec_sqlite_command("COMMIT;", "Failed to commit SQLite transaction");
       }
   
       common::Result<void>
       rollback()
       {
           return exec_sqlite_command("ROLLBACK;", "Failed to rollback SQLite transaction");
       }
   
     private:
       common::Status
       require_open() const
       {
           if (db_ == nullptr) {
               return { common::StatusCode::closed, "SQLite backend is not open." };
           }
           return {};
       }
   
       common::Result<void>
       exec_sqlite_command(const char *sql, std::string_view context)
       {
           if (auto status = require_open(); !status.ok()) {
               return common::Result<void>::failure(status);
           }
           if (sqlite3_exec(db_, sql, nullptr, nullptr, nullptr) != SQLITE_OK) {
               return common::Result<void>::failure(
                   detail::sqlite_make_status(db_,
                                              common::StatusCode::backend_error,
                                              context));
           }
           return common::Result<void>::success();
       }
   
       config_type config_ {};
       sqlite3    *db_ = nullptr;
   };
   
   } // namespace PDJE_UTIL::db::backends
