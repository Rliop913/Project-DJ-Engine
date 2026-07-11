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

    Index::create(cfg);
    auto index = Index::open(cfg);

    index.upsert_item(make_item("alpha", { 1.0F, 0.0F, 0.0F }, "alpha"));
    index.upsert_item(make_item("beta", { 0.0F, 1.0F, 0.0F }, "beta"));
    index.upsert_item(make_item("gamma", { 1.0F, 1.0F, 0.0F }, "gamma"));

    auto stored = index.get_item("beta");
    REQUIRE(stored.text_payload.has_value());
    CHECK(*stored.text_payload == "beta");

    const std::array<float, 3> alpha_query { 1.0F, 0.0F, 0.0F };
    auto hits = index.search(alpha_query, { .limit = 2 });
    REQUIRE(hits.size() == 2);
    CHECK(hits.front().id == "alpha");
    CHECK(hits.front().distance == doctest::Approx(0.0F));

    CHECK_THROWS_AS(index.upsert_item(make_item("bad", { 1.0F, 2.0F })),
                    std::invalid_argument);

    index.erase_item("beta");
    CHECK_FALSE(index.contains("beta"));
    index.close();

    auto reopened_index = Index::open(cfg);

    CHECK(reopened_index.get_item("alpha").embedding.size() == 3);

    auto reopened_hits = reopened_index.search(alpha_query, { .limit = 2 });
    REQUIRE_FALSE(reopened_hits.empty());
    CHECK(reopened_hits.front().id == "alpha");

    reopened_index.close();

    PDJE_UTIL::db::backends::AnnoyConfig ro_cfg {
        .root_path = root,
        .open_options = { .read_only = true },
        .dimension = 3,
        .trees = 10
    };

    auto ro_index = Index::open(ro_cfg);

    CHECK_THROWS_AS(
        ro_index.upsert_item(make_item("readonly", { 0.0F, 0.0F, 1.0F })),
        std::logic_error);

    const std::array<float, 2> invalid_query_vector { 1.0F, 0.0F };
    CHECK_THROWS_AS(ro_index.search(invalid_query_vector, { .limit = 1 }),
                    std::invalid_argument);
    ro_index.close();
    Index::destroy(cfg);
}
