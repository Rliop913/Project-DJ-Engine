#include <doctest/doctest.h>

#include "PDJE_interface.hpp"

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
           ("pdje_core_" + label + "_" +
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
    constexpr std::uint16_t kChannels = 2;
    constexpr std::uint32_t kSampleRate = 48000;
    constexpr std::uint16_t kBitsPerSample = 16;
    constexpr std::uint16_t kBytesPerSample = kBitsPerSample / 8;
    constexpr std::uint16_t kBlockAlign = kChannels * kBytesPerSample;

    const std::uint32_t data_bytes = frame_count * kBlockAlign;
    const std::uint32_t riff_size = 36 + data_bytes;
    const std::uint32_t byte_rate = kSampleRate * kBlockAlign;

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

bool
insert_music_row(const fs::path &sqlite_path, const musdata &md)
{
    sqlite3 *db = nullptr;
    if (sqlite3_open(sqlite_path.string().c_str(), &db) != SQLITE_OK) {
        if (db != nullptr) {
            sqlite3_close(db);
        }
        return false;
    }

    constexpr const char *kSql =
        "INSERT INTO MUSIC (Title, Composer, MusicPath, Bpm, BpmBinary, FirstBeat) "
        "VALUES (?1, ?2, ?3, ?4, ?5, ?6);";

    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(db, kSql, -1, &stmt, nullptr) != SQLITE_OK) {
        sqlite3_close(db);
        return false;
    }

    sqlite3_bind_text(stmt, 1, md.title.c_str(), static_cast<int>(md.title.size()), SQLITE_TRANSIENT);
    sqlite3_bind_text(
        stmt, 2, md.composer.c_str(), static_cast<int>(md.composer.size()), SQLITE_TRANSIENT);
    sqlite3_bind_text(
        stmt, 3, md.musicPath.c_str(), static_cast<int>(md.musicPath.size()), SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 4, md.bpm);
    sqlite3_bind_blob(
        stmt, 5, md.bpmBinary.data(), static_cast<int>(md.bpmBinary.size()), SQLITE_TRANSIENT);
    sqlite3_bind_text(
        stmt, 6, md.firstBeat.c_str(), static_cast<int>(md.firstBeat.size()), SQLITE_TRANSIENT);

    const bool inserted = sqlite3_step(stmt) == SQLITE_DONE;

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return inserted;
}

} // namespace

TEST_CASE("PDJE GetPCMFromMusData decodes music directly from musicPath")
{
    const auto root = make_temp_root("pcm_direct");
    ScopedCleanup cleanup { root };

    const auto wave_path = root / "direct.wav";
    REQUIRE(write_silent_stereo_wav(wave_path, 3));

    PDJE engine(root.generic_string());

    musdata direct_music;
    direct_music.musicPath = wave_path.generic_string();

    const auto pcm = engine.GetPCMFromMusData(direct_music);

    REQUIRE(pcm.size() == 6);
    CHECK(doctest::Approx(pcm.front()) == 0.0f);
    CHECK(doctest::Approx(pcm.back()) == 0.0f);
}

TEST_CASE("PDJE GetPCMFromMusData falls back to database lookup when musicPath is empty")
{
    const auto root = make_temp_root("pcm_lookup");
    ScopedCleanup cleanup { root };

    const auto wave_path = root / "lookup.wav";
    REQUIRE(write_silent_stereo_wav(wave_path, 4));

    PDJE engine(root.generic_string());
    REQUIRE(engine.DBROOT != nullptr);

    musdata stored_music("pcm_song", "pcm_author", wave_path.generic_string(), 128.0);
    stored_music.bpmBinary = BIN { 0 };
    stored_music.firstBeat = "0";
    REQUIRE(insert_music_row(root / "sqlite.db", stored_music));

    musdata query_music;
    query_music.title = stored_music.title;
    query_music.composer = stored_music.composer;
    query_music.bpm = stored_music.bpm;

    const auto pcm = engine.GetPCMFromMusData(query_music);

    REQUIRE(pcm.size() == 8);
    CHECK(doctest::Approx(pcm.front()) == 0.0f);
    CHECK(doctest::Approx(pcm.back()) == 0.0f);
}
