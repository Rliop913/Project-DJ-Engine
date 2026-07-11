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

    Db::create(cfg);
    auto db = Db::open(cfg);

    db.execute("CREATE TABLE items (id INTEGER PRIMARY KEY, name TEXT NOT NULL, payload BLOB);");

    const PDJE_UTIL::db::Bytes payload {
        std::byte { 0x10 },
        std::byte { 0x20 },
        std::byte { 0x30 }
    };

    auto inserted = db.execute("INSERT INTO items(id, name, payload) VALUES(?1, ?2, ?3);",
                               { make_int(1), make_text("alpha"), make_bytes(payload) });
    CHECK(inserted.affected_rows == 1);
    CHECK(inserted.last_insert_rowid.has_value());

    auto selected =
        db.query("SELECT id, name, payload FROM items WHERE id = ?1;", { make_int(1) });
    REQUIRE(selected.rows.size() == 1);
    const auto &row = selected.rows.front();
    REQUIRE(row.find("name") != nullptr);
    CHECK(read_i64(row.values.at(0)) == 1);
    CHECK(read_text(*row.find("name")) == "alpha");
    CHECK(read_bytes(row.values.at(2)) == payload);

    auto updated =
        db.execute("UPDATE items SET name = ?1 WHERE id = ?2;",
                   { make_text("beta"), make_int(1) });
    CHECK(updated.affected_rows == 1);

    auto after_update =
        db.query("SELECT name FROM items WHERE id = ?1;", { make_int(1) });
    REQUIRE(after_update.rows.size() == 1);
    CHECK(read_text(after_update.rows.front().values.at(0)) == "beta");

    CHECK_THROWS_AS(db.query("SELECT FROM broken;", {}), std::runtime_error);

    db.execute("DELETE FROM items WHERE id = ?1;", { make_int(1) });
    auto remaining = db.query("SELECT COUNT(*) AS total FROM items;", {});
    REQUIRE(remaining.rows.size() == 1);
    CHECK(read_i64(remaining.rows.front().values.at(0)) == 0);

    db.close();
    Db::destroy(cfg);
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

    auto db = Db::open(rw_cfg);

    db.execute("CREATE TABLE tx_items (id INTEGER PRIMARY KEY, name TEXT NOT NULL);");

    db.begin_transaction();
    db.execute("INSERT INTO tx_items(id, name) VALUES(?1, ?2);",
               { make_int(1), make_text("rollback") });
    db.rollback();

    auto rolled_back = db.query("SELECT COUNT(*) FROM tx_items;", {});
    CHECK(read_i64(rolled_back.rows.front().values.at(0)) == 0);

    db.begin_transaction();
    db.execute("INSERT INTO tx_items(id, name) VALUES(?1, ?2);",
               { make_int(2), make_text("commit") });
    db.commit();

    auto committed = db.query("SELECT COUNT(*) FROM tx_items;", {});
    CHECK(read_i64(committed.rows.front().values.at(0)) == 1);

    db.close();

    PDJE_UTIL::db::backends::SqliteConfig ro_cfg {
        .path = rw_cfg.path,
        .open_options = { .read_only = true }
    };

    auto ro_db = Db::open(ro_cfg);

    CHECK_THROWS_AS(
        ro_db.execute("INSERT INTO tx_items(id, name) VALUES(?1, ?2);",
                      { make_int(3), make_text("forbidden") }),
        std::runtime_error);

    ro_db.close();
    Db::destroy(rw_cfg);
}
