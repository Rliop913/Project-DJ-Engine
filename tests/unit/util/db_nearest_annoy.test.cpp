#include <doctest/doctest.h>

#include "util/db/backends/AnnoyBackend.hpp"
#include "util/db/nearest/Index.hpp"

#include <array>
#include <chrono>
#include <filesystem>
#include <optional>
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

PDJE_UTIL::db::nearest::Item
make_item(std::string id,
          std::initializer_list<float> embedding,
          std::optional<std::string> text_payload = std::nullopt)
{
    PDJE_UTIL::db::nearest::Item item;
    item.id = std::move(id);
    item.embedding.assign(embedding.begin(), embedding.end());
    if (text_payload.has_value()) {
        item.text_payload = std::move(*text_payload);
    }
    return item;
}

} // namespace

TEST_CASE("annoy nearest backend supports upsert search erase and persistence")
{
    using Index = PDJE_UTIL::db::nearest::NearestNeighborIndex<
        PDJE_UTIL::db::backends::AnnoyBackend>;

    const auto root = make_temp_root("annoy_nearest");
    ScopedCleanup cleanup { root };

    PDJE_UTIL::db::backends::AnnoyConfig cfg {
        .root_path = root,
        .open_options = { .create_if_missing = true },
        .dimension = 3,
        .trees = 10
    };

    REQUIRE(Index::create(cfg).ok());

    auto opened = Index::open(cfg);
    REQUIRE(opened.ok());
    auto index = std::move(opened.value());

    REQUIRE(index.upsert_item(make_item("alpha", { 1.0F, 0.0F, 0.0F }, "alpha")).ok());
    REQUIRE(index.upsert_item(make_item("beta", { 0.0F, 1.0F, 0.0F }, "beta")).ok());
    REQUIRE(index.upsert_item(make_item("gamma", { 1.0F, 1.0F, 0.0F }, "gamma")).ok());

    auto stored = index.get_item("beta");
    REQUIRE(stored.ok());
    REQUIRE(stored.value().text_payload.has_value());
    CHECK(*stored.value().text_payload == "beta");

    const std::array<float, 3> alpha_query { 1.0F, 0.0F, 0.0F };
    auto hits = index.search(alpha_query, { .limit = 2 });
    REQUIRE(hits.ok());
    REQUIRE(hits.value().size() == 2);
    CHECK(hits.value().front().id == "alpha");
    CHECK(hits.value().front().distance == doctest::Approx(0.0F));

    auto mismatch = index.upsert_item(make_item("bad", { 1.0F, 2.0F }));
    CHECK_FALSE(mismatch.ok());
    CHECK(mismatch.status().code == PDJE_UTIL::common::StatusCode::invalid_argument);

    REQUIRE(index.erase_item("beta").ok());
    auto contains_beta = index.contains("beta");
    REQUIRE(contains_beta.ok());
    CHECK_FALSE(contains_beta.value());

    REQUIRE(index.close().ok());

    auto reopened = Index::open(cfg);
    REQUIRE(reopened.ok());
    auto reopened_index = std::move(reopened.value());

    auto reopened_item = reopened_index.get_item("alpha");
    REQUIRE(reopened_item.ok());
    CHECK(reopened_item.value().embedding.size() == 3);

    auto reopened_hits = reopened_index.search(alpha_query, { .limit = 2 });
    REQUIRE(reopened_hits.ok());
    REQUIRE_FALSE(reopened_hits.value().empty());
    CHECK(reopened_hits.value().front().id == "alpha");

    REQUIRE(reopened_index.close().ok());

    PDJE_UTIL::db::backends::AnnoyConfig ro_cfg {
        .root_path = root,
        .open_options = { .read_only = true },
        .dimension = 3,
        .trees = 10
    };

    auto ro_opened = Index::open(ro_cfg);
    REQUIRE(ro_opened.ok());
    auto ro_index = std::move(ro_opened.value());

    auto write_attempt = ro_index.upsert_item(make_item("readonly", { 0.0F, 0.0F, 1.0F }));
    CHECK_FALSE(write_attempt.ok());
    CHECK(write_attempt.status().code == PDJE_UTIL::common::StatusCode::unsupported);

    const std::array<float, 2> invalid_query_vector { 1.0F, 0.0F };
    auto invalid_query = ro_index.search(invalid_query_vector, { .limit = 1 });
    CHECK_FALSE(invalid_query.ok());
    CHECK(invalid_query.status().code == PDJE_UTIL::common::StatusCode::invalid_argument);

    REQUIRE(ro_index.close().ok());
    REQUIRE(Index::destroy(cfg).ok());
}
