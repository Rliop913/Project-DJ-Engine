#include <doctest/doctest.h>

#include "util/PDJE_Util.hpp"
#include "util/function/image/WaveformWebp.hpp"
#include "util/function/image/WebpWriter.hpp"
#include "util/function/stft/BackendLess.hpp"
#include "util/function/stft/MelFilterBank.hpp"
#include "util/function/stft/STFT_Parallel.hpp"

#include <span>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

namespace {

struct DummyKeyValueBackend {
    struct config_type {
    };

    static PDJE_UTIL::common::Result<void>
    create(const config_type &)
    {
        return PDJE_UTIL::common::Result<void>::success();
    }

    static PDJE_UTIL::common::Result<void>
    destroy(const config_type &)
    {
        return PDJE_UTIL::common::Result<void>::success();
    }

    PDJE_UTIL::common::Result<void>
    open(const config_type &)
    {
        return PDJE_UTIL::common::Result<void>::success();
    }

    PDJE_UTIL::common::Result<void>
    close()
    {
        return PDJE_UTIL::common::Result<void>::success();
    }

    PDJE_UTIL::common::Result<bool>
    contains(std::string_view) const
    {
        return PDJE_UTIL::common::Result<bool>::success(false);
    }

    PDJE_UTIL::common::Result<PDJE_UTIL::db::Text>
    get_text(std::string_view) const
    {
        return PDJE_UTIL::common::Result<PDJE_UTIL::db::Text>::success({});
    }

    PDJE_UTIL::common::Result<PDJE_UTIL::db::Bytes>
    get_bytes(std::string_view) const
    {
        return PDJE_UTIL::common::Result<PDJE_UTIL::db::Bytes>::success({});
    }

    PDJE_UTIL::common::Result<void>
    put_text(std::string_view, std::string_view)
    {
        return PDJE_UTIL::common::Result<void>::success();
    }

    PDJE_UTIL::common::Result<void>
    put_bytes(std::string_view, std::span<const std::byte>)
    {
        return PDJE_UTIL::common::Result<void>::success();
    }

    PDJE_UTIL::common::Result<void>
    erase(std::string_view)
    {
        return PDJE_UTIL::common::Result<void>::success();
    }

    PDJE_UTIL::common::Result<std::vector<PDJE_UTIL::db::Key>>
    list_keys(std::string_view) const
    {
        return PDJE_UTIL::common::Result<std::vector<PDJE_UTIL::db::Key>>::success({});
    }
};

struct DummyRelationalBackend {
    struct config_type {
    };

    static PDJE_UTIL::common::Result<void>
    create(const config_type &)
    {
        return PDJE_UTIL::common::Result<void>::success();
    }

    static PDJE_UTIL::common::Result<void>
    destroy(const config_type &)
    {
        return PDJE_UTIL::common::Result<void>::success();
    }

    PDJE_UTIL::common::Result<void>
    open(const config_type &)
    {
        return PDJE_UTIL::common::Result<void>::success();
    }

    PDJE_UTIL::common::Result<void>
    close()
    {
        return PDJE_UTIL::common::Result<void>::success();
    }

    PDJE_UTIL::common::Result<PDJE_UTIL::db::relational::ExecResult>
    execute(std::string_view, const PDJE_UTIL::db::relational::Params &)
    {
        return PDJE_UTIL::common::Result<PDJE_UTIL::db::relational::ExecResult>::success({});
    }

    PDJE_UTIL::common::Result<PDJE_UTIL::db::relational::QueryResult>
    query(std::string_view, const PDJE_UTIL::db::relational::Params &) const
    {
        return PDJE_UTIL::common::Result<PDJE_UTIL::db::relational::QueryResult>::success({});
    }

    PDJE_UTIL::common::Result<void>
    begin_transaction()
    {
        return PDJE_UTIL::common::Result<void>::success();
    }

    PDJE_UTIL::common::Result<void>
    commit()
    {
        return PDJE_UTIL::common::Result<void>::success();
    }

    PDJE_UTIL::common::Result<void>
    rollback()
    {
        return PDJE_UTIL::common::Result<void>::success();
    }
};

struct DummyNearestBackend {
    struct config_type {
    };

    static PDJE_UTIL::common::Result<void>
    create(const config_type &)
    {
        return PDJE_UTIL::common::Result<void>::success();
    }

    static PDJE_UTIL::common::Result<void>
    destroy(const config_type &)
    {
        return PDJE_UTIL::common::Result<void>::success();
    }

    PDJE_UTIL::common::Result<void>
    open(const config_type &)
    {
        return PDJE_UTIL::common::Result<void>::success();
    }

    PDJE_UTIL::common::Result<void>
    close()
    {
        return PDJE_UTIL::common::Result<void>::success();
    }

    PDJE_UTIL::common::Result<bool>
    contains(std::string_view) const
    {
        return PDJE_UTIL::common::Result<bool>::success(false);
    }

    PDJE_UTIL::common::Result<PDJE_UTIL::db::nearest::Item>
    get_item(std::string_view) const
    {
        return PDJE_UTIL::common::Result<PDJE_UTIL::db::nearest::Item>::success({});
    }

    PDJE_UTIL::common::Result<void>
    upsert_item(const PDJE_UTIL::db::nearest::Item &)
    {
        return PDJE_UTIL::common::Result<void>::success();
    }

    PDJE_UTIL::common::Result<void>
    erase_item(std::string_view)
    {
        return PDJE_UTIL::common::Result<void>::success();
    }

    PDJE_UTIL::common::Result<std::vector<PDJE_UTIL::db::nearest::SearchHit>>
    search(std::span<const float>, PDJE_UTIL::db::nearest::SearchOptions) const
    {
        return PDJE_UTIL::common::Result<
            std::vector<PDJE_UTIL::db::nearest::SearchHit>>::success({});
    }

    PDJE_UTIL::common::Result<std::vector<PDJE_UTIL::db::Key>>
    list_keys() const
    {
        return PDJE_UTIL::common::Result<std::vector<PDJE_UTIL::db::Key>>::success({});
    }
};

static_assert(PDJE_UTIL::db::BackendConcept<DummyKeyValueBackend>);
static_assert(PDJE_UTIL::db::keyvalue::KeyValueBackendConcept<DummyKeyValueBackend>);
static_assert(PDJE_UTIL::db::relational::RelationalBackendConcept<DummyRelationalBackend>);
static_assert(PDJE_UTIL::db::nearest::NearestNeighborBackendConcept<DummyNearestBackend>);
static_assert(!std::is_member_function_pointer_v<decltype(&PDJE_UTIL::function::clamp)>);
static_assert(!std::is_member_function_pointer_v<decltype(&PDJE_UTIL::function::slugify)>);
static_assert(std::is_same_v<decltype(PDJE_PARALLEL::STFT::detect_available_backend()),
                             PDJE_PARALLEL::BACKEND_T>);

} // namespace

TEST_CASE("util umbrella header exposes public surface")
{
    auto clamped = PDJE_UTIL::function::clamp(
        { .value = 2.5, .min_value = 0.0, .max_value = 1.0 });
    REQUIRE(clamped.ok());
    CHECK(clamped.value() == doctest::Approx(1.0));

    auto slug = PDJE_UTIL::function::slugify(
        { .input = "Hello, PDJE Util!", .lowercase = true, .separator = '-' });
    REQUIRE(slug.ok());
    CHECK(slug.value() == "hello-pdje-util");
}

TEST_CASE("util database wrappers can be instantiated with compatible backends")
{
    using DummyLegacyDb = PDJE_UTIL::db::Database<DummyKeyValueBackend>;
    using DummyKeyValueDb =
        PDJE_UTIL::db::keyvalue::KeyValueDatabase<DummyKeyValueBackend>;
    using DummyRelationalDb =
        PDJE_UTIL::db::relational::RelationalDatabase<DummyRelationalBackend>;
    using DummyNearestIndex =
        PDJE_UTIL::db::nearest::NearestNeighborIndex<DummyNearestBackend>;

    auto legacy_opened = DummyLegacyDb::open({});
    REQUIRE(legacy_opened.ok());
    CHECK(legacy_opened.value().is_open());

    auto keyvalue_opened = DummyKeyValueDb::open({});
    REQUIRE(keyvalue_opened.ok());
    CHECK(keyvalue_opened.value().is_open());

    auto relational_opened = DummyRelationalDb::open({});
    REQUIRE(relational_opened.ok());
    CHECK(relational_opened.value().is_open());

    auto nearest_opened = DummyNearestIndex::open({});
    REQUIRE(nearest_opened.ok());
    CHECK(nearest_opened.value().is_open());
}

TEST_CASE("util stable leaf headers remain self-contained")
{
    PDJE_PARALLEL::STFT stft;
    const bool hasSupportedBackend =
        stft.active_backend() == PDJE_PARALLEL::BACKEND_T::SERIAL ||
        stft.active_backend() == PDJE_PARALLEL::BACKEND_T::OPENCL;

    CHECK(hasSupportedBackend);

    PDJE_UTIL::function::image::EncodeWaveformWebpArgs waveformArgs;
    CHECK(waveformArgs.channel_count == 0u);

    PDJE_UTIL::function::image::EncodeWebpArgs webpArgs;
    CHECK(webpArgs.compression_level == -1);
}
