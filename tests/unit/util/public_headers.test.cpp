#include <doctest/doctest.h>

#include "util/PDJE_Util.hpp"
#include "util/ai/beat_this/BeatThis.hpp"
#include "util/db/backends/AnnoyBackend.hpp"
#include "util/db/backends/RocksDbBackend.hpp"
#include "util/db/backends/SqliteBackend.hpp"
#include "util/function/image/WaveformWebp.hpp"
#include "util/function/image/WebpWriter.hpp"

#include <type_traits>

namespace {

struct DummyConfig {};

struct DummyKeyValueBackend {
    using config_type = DummyConfig;
    static void
    create(const config_type &)
    {
    }
    static void
    destroy(const config_type &)
    {
    }
    void
    open(const config_type &)
    {
    }
    void
    close()
    {
    }
    bool
    contains(std::string_view) const
    {
        return false;
    }
    PDJE_UTIL::db::Text
    get_text(std::string_view) const
    {
        return {};
    }
    PDJE_UTIL::db::Bytes
    get_bytes(std::string_view) const
    {
        return {};
    }
    void
    put_text(std::string_view, std::string_view)
    {
    }
    void
    put_bytes(std::string_view, std::span<const std::byte>)
    {
    }
    void
    erase(std::string_view)
    {
    }
    std::vector<PDJE_UTIL::db::Key>
    list_keys(std::string_view) const
    {
        return {};
    }
};

struct DummyRelationalBackend {
    using config_type = DummyConfig;
    static void
    create(const config_type &)
    {
    }
    static void
    destroy(const config_type &)
    {
    }
    void
    open(const config_type &)
    {
    }
    void
    close()
    {
    }
    PDJE_UTIL::db::relational::ExecResult
    execute(std::string_view, const PDJE_UTIL::db::relational::Params &)
    {
        return {};
    }
    PDJE_UTIL::db::relational::QueryResult
    query(std::string_view, const PDJE_UTIL::db::relational::Params &) const
    {
        return {};
    }
    void
    begin_transaction()
    {
    }
    void
    commit()
    {
    }
    void
    rollback()
    {
    }
};

struct DummyNearestBackend {
    using config_type = DummyConfig;
    static void
    create(const config_type &)
    {
    }
    static void
    destroy(const config_type &)
    {
    }
    void
    open(const config_type &)
    {
    }
    void
    flush()
    {
    }
    void
    close()
    {
    }
    bool
    contains(std::string_view) const
    {
        return false;
    }
    PDJE_UTIL::db::nearest::Item
    get_item(std::string_view) const
    {
        return {};
    }
    void
    upsert_item(const PDJE_UTIL::db::nearest::Item &)
    {
    }
    void
    erase_item(std::string_view)
    {
    }
    std::vector<PDJE_UTIL::db::nearest::SearchHit>
    search(std::span<const float>, PDJE_UTIL::db::nearest::SearchOptions) const
    {
        return {};
    }
    std::vector<PDJE_UTIL::db::Key>
    list_keys() const
    {
        return {};
    }
};

static_assert(
    PDJE_UTIL::db::keyvalue::KeyValueBackendConcept<DummyKeyValueBackend>);
static_assert(PDJE_UTIL::db::relational::RelationalBackendConcept<
              DummyRelationalBackend>);
static_assert(
    PDJE_UTIL::db::nearest::NearestNeighborBackendConcept<DummyNearestBackend>);
static_assert(std::is_same_v<decltype(PDJE_UTIL::function::clamp({})), double>);
static_assert(std::is_same_v<decltype(PDJE_UTIL::function::slugify(
                                 PDJE_UTIL::function::SlugifyArgs{})),
                             std::string>);
static_assert(std::is_member_object_pointer_v<
              decltype(&PDJE_PARALLEL::STFT::active_backend)>);
static_assert(std::is_member_object_pointer_v<
              decltype(&PDJE_UTIL::ai::OnnxSession::model_path)>);
static_assert(std::is_member_object_pointer_v<
              decltype(&PDJE_UTIL::ai::BeatThisDetector::frontend_config)>);
static_assert(
    std::is_const_v<std::remove_reference_t<
        decltype(std::declval<PDJE_UTIL::ai::OnnxSession>().model_path)>>);
static_assert(std::is_const_v<std::remove_reference_t<
                  decltype(std::declval<PDJE_UTIL::ai::BeatThisDetector>()
                               .frontend_config)>>);
static_assert(std::is_move_constructible_v<PDJE_UTIL::ai::OnnxSession>);
static_assert(!std::is_move_assignable_v<PDJE_UTIL::ai::OnnxSession>);
static_assert(std::is_move_constructible_v<PDJE_UTIL::ai::BeatThisDetector>);
static_assert(!std::is_move_assignable_v<PDJE_UTIL::ai::BeatThisDetector>);

} // namespace

TEST_CASE("util umbrella exposes direct-return data-oriented surface")
{
    CHECK(PDJE_UTIL::function::clamp(
              { .value = 2.0, .min_value = 0.0, .max_value = 1.0 }) == 1.0);
    CHECK(PDJE_UTIL::function::slugify({ .input = "Hello PDJE Util" }) ==
          "hello-pdje-util");

    auto keyvalue =
        PDJE_UTIL::db::keyvalue::KeyValueDatabase<DummyKeyValueBackend>::open(
            {});
    auto relational = PDJE_UTIL::db::relational::RelationalDatabase<
        DummyRelationalBackend>::open({});
    auto nearest =
        PDJE_UTIL::db::nearest::NearestNeighborIndex<DummyNearestBackend>::open(
            {});
    CHECK(keyvalue.is_open);
    CHECK(relational.is_open);
    CHECK(nearest.is_open);
}

TEST_CASE("util argument structs expose state directly")
{
    PDJE_UTIL::function::image::EncodeWaveformWebpArgs waveform;
    PDJE_UTIL::function::image::EncodeWebpArgs         webp;
    PDJE_PARALLEL::STFTRequest                         stft;
    CHECK(waveform.channel_count == 0);
    CHECK(webp.compression_level == -1);
    CHECK(stft.n_fft == 1024);
}
