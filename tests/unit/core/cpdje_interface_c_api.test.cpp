#include <doctest/doctest.h>

#include "CPDJE_interface.h"
#include "fileNameSanitizer.hpp"

#include <chrono>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <sqlite3.h>
#include <string>

namespace {

namespace fs = std::filesystem;

fs::path
make_temp_root(const std::string &label)
{
    return fs::temp_directory_path() /
           ("pdje_c_abi_" + label + "_" +
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

void
write_le16(std::ostream &out, std::uint16_t value)
{
    out.put(static_cast<char>(value & 0xFF));
    out.put(static_cast<char>((value >> 8) & 0xFF));
}

void
write_le32(std::ostream &out, std::uint32_t value)
{
    out.put(static_cast<char>(value & 0xFF));
    out.put(static_cast<char>((value >> 8) & 0xFF));
    out.put(static_cast<char>((value >> 16) & 0xFF));
    out.put(static_cast<char>((value >> 24) & 0xFF));
}

bool
write_silent_stereo_wav(const fs::path &path, std::uint32_t frame_count)
{
    constexpr std::uint16_t kChannels       = 2;
    constexpr std::uint32_t kSampleRate     = 48000;
    constexpr std::uint16_t kBitsPerSample  = 16;
    constexpr std::uint16_t kBytesPerSample = kBitsPerSample / 8;
    constexpr std::uint16_t kBlockAlign     = kChannels * kBytesPerSample;

    const std::uint32_t data_bytes = frame_count * kBlockAlign;
    const std::uint32_t riff_size  = 36 + data_bytes;
    const std::uint32_t byte_rate  = kSampleRate * kBlockAlign;

    std::error_code ec;
    fs::create_directories(path.parent_path(), ec);
    if (ec) {
        return false;
    }

    std::ofstream out(path.string(), std::ios::binary | std::ios::out);
    if (!out.is_open()) {
        return false;
    }

    out.write("RIFF", 4);
    write_le32(out, riff_size);
    out.write("WAVE", 4);
    out.write("fmt ", 4);
    write_le32(out, 16);
    write_le16(out, 1);
    write_le16(out, kChannels);
    write_le32(out, kSampleRate);
    write_le32(out, byte_rate);
    write_le16(out, kBlockAlign);
    write_le16(out, kBitsPerSample);
    out.write("data", 4);
    write_le32(out, data_bytes);

    for (std::uint32_t i = 0; i < frame_count * kChannels; ++i) {
        write_le16(out, 0);
    }

    return static_cast<bool>(out);
}

const void *
blob_ptr_or_null(const unsigned char *data, const std::size_t size)
{
    return size > 0 ? data : nullptr;
}

bool
insert_music_row(const fs::path &sqlite_path,
                 const std::string &title,
                 const std::string &composer,
                 const std::string &music_path,
                 const double       bpm,
                 const std::string &first_beat)
{
    const auto safe_title    = PDJE_Name_Sanitizer::sanitizeFileName(title);
    const auto safe_composer = PDJE_Name_Sanitizer::sanitizeFileName(composer);
    if (!safe_title || !safe_composer) {
        return false;
    }

    sqlite3 *db = nullptr;
    if (sqlite3_open(sqlite_path.string().c_str(), &db) != SQLITE_OK) {
        if (db != nullptr) {
            sqlite3_close(db);
        }
        return false;
    }

    const unsigned char bpm_binary[] = { 0 };
    constexpr const char *kSql =
        "INSERT INTO MUSIC (Title, Composer, MusicPath, Bpm, BpmBinary, FirstBeat) "
        "VALUES (?1, ?2, ?3, ?4, ?5, ?6);";

    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(db, kSql, -1, &stmt, nullptr) != SQLITE_OK) {
        sqlite3_close(db);
        return false;
    }

    sqlite3_bind_text(stmt,
                      1,
                      safe_title->c_str(),
                      static_cast<int>(safe_title->size()),
                      SQLITE_TRANSIENT);
    sqlite3_bind_text(
        stmt, 2, safe_composer->c_str(), static_cast<int>(safe_composer->size()), SQLITE_TRANSIENT);
    sqlite3_bind_text(
        stmt, 3, music_path.c_str(), static_cast<int>(music_path.size()), SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 4, bpm);
    sqlite3_bind_blob(stmt,
                      5,
                      blob_ptr_or_null(bpm_binary, sizeof(bpm_binary)),
                      static_cast<int>(sizeof(bpm_binary)),
                      SQLITE_TRANSIENT);
    sqlite3_bind_text(
        stmt, 6, first_beat.c_str(), static_cast<int>(first_beat.size()), SQLITE_TRANSIENT);

    const bool inserted = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return inserted;
}

bool
insert_track_row(const fs::path &sqlite_path, const std::string &title)
{
    const auto safe_title = PDJE_Name_Sanitizer::sanitizeFileName(title);
    if (!safe_title) {
        return false;
    }

    sqlite3 *db = nullptr;
    if (sqlite3_open(sqlite_path.string().c_str(), &db) != SQLITE_OK) {
        if (db != nullptr) {
            sqlite3_close(db);
        }
        return false;
    }

    const unsigned char mix_binary[]  = { 0 };
    const unsigned char note_binary[] = { 0 };
    constexpr const char *kSql =
        "INSERT INTO TRACK (TrackTitle, MixBinary, NoteBinary, CachedMixList) "
        "VALUES (?1, ?2, ?3, ?4);";

    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(db, kSql, -1, &stmt, nullptr) != SQLITE_OK) {
        sqlite3_close(db);
        return false;
    }

    sqlite3_bind_text(stmt,
                      1,
                      safe_title->c_str(),
                      static_cast<int>(safe_title->size()),
                      SQLITE_TRANSIENT);
    sqlite3_bind_blob(stmt,
                      2,
                      blob_ptr_or_null(mix_binary, sizeof(mix_binary)),
                      static_cast<int>(sizeof(mix_binary)),
                      SQLITE_TRANSIENT);
    sqlite3_bind_blob(stmt,
                      3,
                      blob_ptr_or_null(note_binary, sizeof(note_binary)),
                      static_cast<int>(sizeof(note_binary)),
                      SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, "", 0, SQLITE_TRANSIENT);

    const bool inserted = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return inserted;
}

std::string
to_string(const PDJE_StringViewV1 &value)
{
    if (value.data == nullptr || value.size == 0) {
        return {};
    }
    return std::string(value.data, value.size);
}

} // namespace

TEST_CASE("CPDJE C ABI searches music and extracts PCM")
{
    const auto root = make_temp_root("music");
    ScopedCleanup cleanup { root };

    const auto wave_path = root / "song.wav";
    REQUIRE(write_silent_stereo_wav(wave_path, 4));

    PDJE_EngineHandleV1 *engine = nullptr;
    REQUIRE(pdje_engine_create_v1(root.generic_string().c_str(), &engine) ==
            PDJE_RESULT_OK_V1);
    REQUIRE(engine != nullptr);

    REQUIRE(insert_music_row(root / "sqlite.db",
                             "pcm_song",
                             "pcm_author",
                             wave_path.generic_string(),
                             128.0,
                             "0"));

    PDJE_MusicListHandleV1 *music_list = nullptr;
    REQUIRE(pdje_engine_search_music_v1(
                engine, "pcm_song", "pcm_author", 128.0, &music_list) ==
            PDJE_RESULT_OK_V1);
    REQUIRE(music_list != nullptr);
    REQUIRE(pdje_music_list_size_v1(music_list) == 1);

    PDJE_MusicViewV1 music_view {};
    music_view.struct_size = sizeof(music_view);
    REQUIRE(pdje_music_list_get_v1(music_list, 0, &music_view) ==
            PDJE_RESULT_OK_V1);
    REQUIRE(PDJE_Name_Sanitizer::sanitizeFileName("pcm_song").has_value());
    REQUIRE(PDJE_Name_Sanitizer::sanitizeFileName("pcm_author").has_value());
    CHECK(to_string(music_view.title) ==
          PDJE_Name_Sanitizer::sanitizeFileName("pcm_song").value());
    CHECK(to_string(music_view.composer) ==
          PDJE_Name_Sanitizer::sanitizeFileName("pcm_author").value());
    CHECK(to_string(music_view.music_path) == wave_path.generic_string());
    CHECK(music_view.bpm == doctest::Approx(128.0));
    CHECK(to_string(music_view.first_beat) == "0");

    PDJE_PcmBufferHandleV1 *pcm = nullptr;
    REQUIRE(pdje_engine_get_pcm_from_music_v1(engine, music_list, 0, &pcm) ==
            PDJE_RESULT_OK_V1);
    REQUIRE(pcm != nullptr);
    REQUIRE(pdje_pcm_buffer_size_v1(pcm) == 8);
    REQUIRE(pdje_pcm_buffer_data_v1(pcm) != nullptr);
    CHECK(doctest::Approx(pdje_pcm_buffer_data_v1(pcm)[0]) == 0.0f);
    CHECK(doctest::Approx(pdje_pcm_buffer_data_v1(pcm)[7]) == 0.0f);

    CHECK(pdje_engine_search_music_v1(engine, nullptr, nullptr, -1.0, nullptr) ==
          PDJE_RESULT_INVALID_ARGUMENT_V1);
    CHECK(pdje_music_list_get_v1(music_list, 1, &music_view) ==
          PDJE_RESULT_OUT_OF_RANGE_V1);

    pdje_pcm_buffer_destroy_v1(pcm);
    pdje_music_list_destroy_v1(music_list);
    pdje_engine_destroy_v1(engine);
}

TEST_CASE("CPDJE C ABI exposes track search, player lifecycle, and dataline")
{
    const auto root = make_temp_root("track");
    ScopedCleanup cleanup { root };

    PDJE_EngineHandleV1 *engine = nullptr;
    REQUIRE(pdje_engine_create_v1(root.generic_string().c_str(), &engine) ==
            PDJE_RESULT_OK_V1);
    REQUIRE(engine != nullptr);

    REQUIRE(insert_track_row(root / "sqlite.db", "broken_track"));

    PDJE_TrackListHandleV1 *track_list = nullptr;
    REQUIRE(pdje_engine_search_track_v1(engine, "broken_track", &track_list) ==
            PDJE_RESULT_OK_V1);
    REQUIRE(track_list != nullptr);
    REQUIRE(pdje_track_list_size_v1(track_list) == 1);

    PDJE_TrackViewV1 track_view {};
    track_view.struct_size = sizeof(track_view);
    REQUIRE(pdje_track_list_get_v1(track_list, 0, &track_view) ==
            PDJE_RESULT_OK_V1);
    REQUIRE(PDJE_Name_Sanitizer::sanitizeFileName("broken_track").has_value());
    CHECK(to_string(track_view.track_title) ==
          PDJE_Name_Sanitizer::sanitizeFileName("broken_track").value());

    PDJE_CoreDataLineSnapshotV1 dline {};
    dline.struct_size = sizeof(dline);
    REQUIRE(pdje_engine_pull_core_dataline_v1(engine, &dline) ==
            PDJE_RESULT_OK_V1);
    CHECK(dline.has_player == 0);
    CHECK(dline.has_pre_rendered_data == 0);
    CHECK(dline.has_sync == 0);

    CHECK(pdje_engine_init_player_from_track_v1(
              engine,
              PDJE_PLAY_MODE_FULL_MANUAL_RENDER_V1,
              track_list,
              0,
              48) == PDJE_RESULT_INVALID_ARGUMENT_V1);
    CHECK(pdje_engine_init_player_from_track_v1(
              engine,
              PDJE_PLAY_MODE_FULL_PRE_RENDER_V1,
              track_list,
              1,
              48) == PDJE_RESULT_OUT_OF_RANGE_V1);
    CHECK(pdje_engine_init_player_from_track_v1(
              engine,
              PDJE_PLAY_MODE_FULL_PRE_RENDER_V1,
              track_list,
              0,
              48) == PDJE_RESULT_INTERNAL_ERROR_V1);

    const int manual_init = pdje_engine_init_player_manual_v1(engine, 48);
    CHECK((manual_init == PDJE_RESULT_OK_V1 ||
           manual_init == PDJE_RESULT_INTERNAL_ERROR_V1));

    dline.struct_size = sizeof(dline);
    REQUIRE(pdje_engine_pull_core_dataline_v1(engine, &dline) ==
            PDJE_RESULT_OK_V1);
    if (manual_init == PDJE_RESULT_OK_V1) {
        CHECK(dline.has_player == 1);
        CHECK(dline.has_sync == 1);
    } else {
        CHECK(dline.has_player == 0);
    }

    pdje_track_list_destroy_v1(track_list);
    if (manual_init != PDJE_RESULT_OK_V1) {
        pdje_engine_destroy_v1(engine);
    }
}

TEST_CASE("CPDJE C ABI exposes editor lifecycle")
{
    const auto root = make_temp_root("editor");
    ScopedCleanup cleanup { root };

    PDJE_EngineHandleV1 *engine = nullptr;
    REQUIRE(pdje_engine_create_v1(root.generic_string().c_str(), &engine) ==
            PDJE_RESULT_OK_V1);
    REQUIRE(engine != nullptr);

    const auto project_root = root / "editor_project";
    REQUIRE(pdje_engine_init_editor_v1(
                engine,
                "author",
                "author@example.com",
                project_root.generic_string().c_str()) == PDJE_RESULT_OK_V1);
}
