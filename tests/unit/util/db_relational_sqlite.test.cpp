#include <doctest/doctest.h>

#include "util/db/backends/SqliteBackend.hpp"
#include "util/db/relational/Database.hpp"

#include <chrono>
#include <cstdint>
#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

namespace {

namespace fs = std::filesystem;

fs::path
make_temp_root(std::string_view label)
{
    return fs::temp_directory_path() /
           ("pdje_util_" + std::string(label) + "_" +
            std::to_string(
                std::chrono::steady_clock::now().time_since_epoch().count()));
}

struct ScopedCleanup {
    fs::path root;

    ~ScopedCleanup()
    {
        std::error_code ec;
        fs::remove_all(root, ec);
    }
};

PDJE_UTIL::db::relational::Value
make_int(std::int64_t value)
{
    return { PDJE_UTIL::db::relational::ValueStorage { value } };
}

PDJE_UTIL::db::relational::Value
make_text(std::string value)
{
    return { PDJE_UTIL::db::relational::ValueStorage { std::move(value) } };
}

PDJE_UTIL::db::relational::Value
make_bytes(PDJE_UTIL::db::Bytes value)
{
    return { PDJE_UTIL::db::relational::ValueStorage { std::move(value) } };
}

std::int64_t
read_i64(const PDJE_UTIL::db::relational::Value &value)
{
    return std::get<std::int64_t>(value.storage);
}

std::string
read_text(const PDJE_UTIL::db::relational::Value &value)
{
    return std::get<PDJE_UTIL::db::Text>(value.storage);
}

PDJE_UTIL::db::Bytes
read_bytes(const PDJE_UTIL::db::relational::Value &value)
{
    return std::get<PDJE_UTIL::db::Bytes>(value.storage);
}

} // namespace

TEST_CASE("sqlite relational backend supports sql execution and queries")
{
    using Db = PDJE_UTIL::db::relational::RelationalDatabase<
        PDJE_UTIL::db::backends::SqliteBackend>;

    const auto root = make_temp_root("sqlite_relational");
    ScopedCleanup cleanup { root };

    PDJE_UTIL::db::backends::SqliteConfig cfg {
        .path = root / "util.sqlite",
        .open_options = { .create_if_missing = true }
    };

    REQUIRE(Db::create(cfg).ok());

    auto opened = Db::open(cfg);
    REQUIRE(opened.ok());
    auto db = std::move(opened.value());

    REQUIRE(db.execute(
                   "CREATE TABLE items (id INTEGER PRIMARY KEY, name TEXT NOT NULL, payload BLOB);")
                .ok());

    const PDJE_UTIL::db::Bytes payload {
        std::byte { 0x10 },
        std::byte { 0x20 },
        std::byte { 0x30 }
    };

    auto inserted = db.execute("INSERT INTO items(id, name, payload) VALUES(?1, ?2, ?3);",
                               { make_int(1), make_text("alpha"), make_bytes(payload) });
    REQUIRE(inserted.ok());
    CHECK(inserted.value().affected_rows == 1);
    CHECK(inserted.value().last_insert_rowid.has_value());

    auto selected =
        db.query("SELECT id, name, payload FROM items WHERE id = ?1;", { make_int(1) });
    REQUIRE(selected.ok());
    REQUIRE(selected.value().rows.size() == 1);
    const auto &row = selected.value().rows.front();
    REQUIRE(row.find("name") != nullptr);
    CHECK(read_i64(row.at(0)) == 1);
    CHECK(read_text(*row.find("name")) == "alpha");
    CHECK(read_bytes(row.at(2)) == payload);

    auto updated =
        db.execute("UPDATE items SET name = ?1 WHERE id = ?2;",
                   { make_text("beta"), make_int(1) });
    REQUIRE(updated.ok());
    CHECK(updated.value().affected_rows == 1);

    auto after_update =
        db.query("SELECT name FROM items WHERE id = ?1;", { make_int(1) });
    REQUIRE(after_update.ok());
    REQUIRE(after_update.value().rows.size() == 1);
    CHECK(read_text(after_update.value().rows.front().at(0)) == "beta");

    auto invalid = db.query("SELECT FROM broken;", {});
    CHECK_FALSE(invalid.ok());
    CHECK(invalid.status().code == PDJE_UTIL::common::StatusCode::backend_error);

    REQUIRE(db.execute("DELETE FROM items WHERE id = ?1;", { make_int(1) }).ok());
    auto remaining = db.query("SELECT COUNT(*) AS total FROM items;", {});
    REQUIRE(remaining.ok());
    REQUIRE(remaining.value().rows.size() == 1);
    CHECK(read_i64(remaining.value().rows.front().at(0)) == 0);

    REQUIRE(db.close().ok());
    REQUIRE(Db::destroy(cfg).ok());
}

TEST_CASE("sqlite relational backend supports transactions and read-only mode")
{
    using Db = PDJE_UTIL::db::relational::RelationalDatabase<
        PDJE_UTIL::db::backends::SqliteBackend>;

    const auto root = make_temp_root("sqlite_transactions");
    ScopedCleanup cleanup { root };

    PDJE_UTIL::db::backends::SqliteConfig rw_cfg {
        .path = root / "util.sqlite",
        .open_options = { .create_if_missing = true }
    };

    auto opened = Db::open(rw_cfg);
    REQUIRE(opened.ok());
    auto db = std::move(opened.value());

    REQUIRE(db.execute("CREATE TABLE tx_items (id INTEGER PRIMARY KEY, name TEXT NOT NULL);").ok());

    REQUIRE(db.begin_transaction().ok());
    REQUIRE(db.execute("INSERT INTO tx_items(id, name) VALUES(?1, ?2);",
                       { make_int(1), make_text("rollback") })
                .ok());
    REQUIRE(db.rollback().ok());

    auto rolled_back = db.query("SELECT COUNT(*) FROM tx_items;", {});
    REQUIRE(rolled_back.ok());
    CHECK(read_i64(rolled_back.value().rows.front().at(0)) == 0);

    REQUIRE(db.begin_transaction().ok());
    REQUIRE(db.execute("INSERT INTO tx_items(id, name) VALUES(?1, ?2);",
                       { make_int(2), make_text("commit") })
                .ok());
    REQUIRE(db.commit().ok());

    auto committed = db.query("SELECT COUNT(*) FROM tx_items;", {});
    REQUIRE(committed.ok());
    CHECK(read_i64(committed.value().rows.front().at(0)) == 1);

    REQUIRE(db.close().ok());

    PDJE_UTIL::db::backends::SqliteConfig ro_cfg {
        .path = rw_cfg.path,
        .open_options = { .read_only = true }
    };

    auto ro_opened = Db::open(ro_cfg);
    REQUIRE(ro_opened.ok());
    auto ro_db = std::move(ro_opened.value());

    auto write_attempt =
        ro_db.execute("INSERT INTO tx_items(id, name) VALUES(?1, ?2);",
                      { make_int(3), make_text("forbidden") });
    CHECK_FALSE(write_attempt.ok());
    CHECK(write_attempt.status().code == PDJE_UTIL::common::StatusCode::backend_error);

    REQUIRE(ro_db.close().ok());
    REQUIRE(Db::destroy(rw_cfg).ok());
}
