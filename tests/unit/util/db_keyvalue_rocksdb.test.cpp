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

    Db::create(cfg);
    auto db = Db::open(cfg);

    db.put_text("alpha:1", "hello");
    db.put_text("alpha:2", "world");
    CHECK(db.contains("alpha:1"));

    CHECK(db.get_text("alpha:1") == "hello");

    const PDJE_UTIL::db::Bytes payload {
        std::byte { 0x01 },
        std::byte { 0xAB }
    };
    db.put_bytes("blob", payload);
    CHECK(db.get_bytes("blob") == payload);
    CHECK_THROWS_AS(db.get_text("blob"), std::logic_error);

    db.put_text("alpha:1", "updated");
    CHECK(db.get_text("alpha:1") == "updated");

    auto prefixed = db.list_keys("alpha:");
    REQUIRE(prefixed.size() == 2);
    CHECK(prefixed[0] == "alpha:1");
    CHECK(prefixed[1] == "alpha:2");

    db.erase("alpha:2");
    CHECK_FALSE(db.contains("alpha:2"));
    db.close();

    PDJE_UTIL::db::backends::RocksDbConfig ro_cfg {
        .path = root,
        .open_options = { .read_only = true }
    };

    auto ro_db = Db::open(ro_cfg);
    CHECK_THROWS_AS(ro_db.put_text("readonly", "blocked"), std::logic_error);
    ro_db.close();
    Db::destroy(cfg);
}
