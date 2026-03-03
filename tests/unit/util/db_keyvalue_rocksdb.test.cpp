#include <doctest/doctest.h>

#include "util/db/backends/RocksDbBackend.hpp"
#include "util/db/keyvalue/Database.hpp"

#include <chrono>
#include <filesystem>
#include <string>
#include <string_view>

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

} // namespace

TEST_CASE("rocksdb key-value backend supports text bytes overwrite and prefix scan")
{
    using Db = PDJE_UTIL::db::keyvalue::KeyValueDatabase<
        PDJE_UTIL::db::backends::RocksDbBackend>;

    const auto root = make_temp_root("rocksdb_keyvalue");
    ScopedCleanup cleanup { root };

    PDJE_UTIL::db::backends::RocksDbConfig cfg {
        .path = root,
        .open_options = { .create_if_missing = true }
    };

    REQUIRE(Db::create(cfg).ok());

    auto opened = Db::open(cfg);
    REQUIRE(opened.ok());
    auto db = std::move(opened.value());

    REQUIRE(db.put_text("alpha:1", "hello").ok());
    REQUIRE(db.put_text("alpha:2", "world").ok());
    REQUIRE(db.contains("alpha:1").ok());
    CHECK(db.contains("alpha:1").value());

    auto text = db.get_text("alpha:1");
    REQUIRE(text.ok());
    CHECK(text.value() == "hello");

    const PDJE_UTIL::db::Bytes payload {
        std::byte { 0x01 },
        std::byte { 0xAB }
    };
    REQUIRE(db.put_bytes("blob", payload).ok());

    auto bytes = db.get_bytes("blob");
    REQUIRE(bytes.ok());
    CHECK(bytes.value() == payload);

    auto type_mismatch = db.get_text("blob");
    CHECK_FALSE(type_mismatch.ok());
    CHECK(type_mismatch.status().code == PDJE_UTIL::common::StatusCode::type_mismatch);

    REQUIRE(db.put_text("alpha:1", "updated").ok());
    auto updated = db.get_text("alpha:1");
    REQUIRE(updated.ok());
    CHECK(updated.value() == "updated");

    auto prefixed = db.list_keys("alpha:");
    REQUIRE(prefixed.ok());
    REQUIRE(prefixed.value().size() == 2);
    CHECK(prefixed.value()[0] == "alpha:1");
    CHECK(prefixed.value()[1] == "alpha:2");

    REQUIRE(db.erase("alpha:2").ok());
    auto contains_removed = db.contains("alpha:2");
    REQUIRE(contains_removed.ok());
    CHECK_FALSE(contains_removed.value());

    REQUIRE(db.close().ok());

    PDJE_UTIL::db::backends::RocksDbConfig ro_cfg {
        .path = root,
        .open_options = { .read_only = true }
    };

    auto ro_opened = Db::open(ro_cfg);
    REQUIRE(ro_opened.ok());
    auto ro_db = std::move(ro_opened.value());

    auto write_attempt = ro_db.put_text("readonly", "blocked");
    CHECK_FALSE(write_attempt.ok());
    CHECK(write_attempt.status().code == PDJE_UTIL::common::StatusCode::unsupported);

    REQUIRE(ro_db.close().ok());
    REQUIRE(Db::destroy(cfg).ok());
}
