#include <doctest/doctest.h>

#include "editorObject.hpp"

#include <chrono>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <functional>
#include <nlohmann/json.hpp>
#include <optional>
#include <set>
#include <string>

namespace {

namespace fs    = std::filesystem;
using test_json = nlohmann::json;

fs::path
MakeTempRoot(const std::string &label)
{
    return fs::temp_directory_path() /
           ("pdje_editor_diff_" + label + "_" +
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
WriteLe16(std::ostream &out, std::uint16_t value)
{
    out.put(static_cast<char>(value & 0xFF));
    out.put(static_cast<char>((value >> 8) & 0xFF));
}

void
WriteLe32(std::ostream &out, std::uint32_t value)
{
    out.put(static_cast<char>(value & 0xFF));
    out.put(static_cast<char>((value >> 8) & 0xFF));
    out.put(static_cast<char>((value >> 16) & 0xFF));
    out.put(static_cast<char>((value >> 24) & 0xFF));
}

bool
WriteSilentStereoWav(const fs::path &path, std::uint32_t frame_count)
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
    WriteLe32(out, riff_size);
    out.write("WAVE", 4);
    out.write("fmt ", 4);
    WriteLe32(out, 16);
    WriteLe16(out, 1);
    WriteLe16(out, kChannels);
    WriteLe32(out, kSampleRate);
    WriteLe32(out, byte_rate);
    WriteLe16(out, kBlockAlign);
    WriteLe16(out, kBitsPerSample);
    out.write("data", 4);
    WriteLe32(out, data_bytes);

    for (std::uint32_t i = 0; i < frame_count * kChannels; ++i) {
        WriteLe16(out, 0);
    }

    return static_cast<bool>(out);
}

std::set<std::string>
CollectLogOids(const std::string &logs_json)
{
    std::set<std::string> oids;
    auto                  root = test_json::parse(logs_json);
    if (!root.contains("LOGS") || !root["LOGS"].is_array()) {
        return oids;
    }
    for (const auto &entry : root["LOGS"]) {
        if (entry.contains("OID") && entry["OID"].is_string()) {
            oids.insert(entry["OID"].get<std::string>());
        }
    }
    return oids;
}

std::optional<std::string>
FindExactlyOneNewOid(const std::set<std::string> &before,
                     const std::set<std::string> &after)
{
    std::optional<std::string> found;
    for (const auto &oid : after) {
        if (!before.contains(oid)) {
            if (found.has_value()) {
                return std::nullopt;
            }
            found = oid;
        }
    }
    return found;
}

bool
RecordNewCommitOid(const std::function<void()>        &update_logs,
                   const std::function<std::string()> &read_logs,
                   std::set<std::string>              &oid_cache,
                   std::optional<std::string>         &new_oid)
{
    update_logs();
    auto next_oids = CollectLogOids(read_logs());
    new_oid        = FindExactlyOneNewOid(oid_cache, next_oids);
    oid_cache      = std::move(next_oids);
    return new_oid.has_value();
}

EDIT_ARG_MIX
MakeMix(int id, int beat)
{
    EDIT_ARG_MIX mix;
    mix.type     = TypeEnum::LOAD;
    mix.details  = DetailEnum::HIGH;
    mix.ID       = id;
    mix.first    = "mix_song" + std::to_string(id);
    mix.second   = "mix_author" + std::to_string(id);
    mix.beat     = beat;
    mix.subBeat  = 0;
    mix.separate = 4;
    return mix;
}

EDIT_ARG_NOTE
MakeNote(int rail_id, int beat)
{
    EDIT_ARG_NOTE note;
    note.Note_Type   = "tap";
    note.Note_Detail = 1;
    note.beat        = beat;
    note.subBeat     = 0;
    note.separate    = 4;
    note.railID      = rail_id;
    return note;
}

EDIT_ARG_MUSIC
MakeMusicEdit(const std::string &music_name, const std::string &bpm, int beat)
{
    EDIT_ARG_MUSIC music;
    music.musicName    = music_name;
    music.arg.bpm      = bpm;
    music.arg.beat     = beat;
    music.arg.subBeat  = 0;
    music.arg.separate = 4;
    return music;
}

} // namespace

TEST_CASE("core: editorObject MIX diff reports append semantics")
{
    const auto    root = MakeTempRoot("mix");
    ScopedCleanup cleanup{ root };

    editorObject editor;
    REQUIRE(editor.Open(root, "diff-tester", "diff-tester@test"));

    editor.UpdateLog<EDIT_ARG_MIX>();
    auto mix_oids = CollectLogOids(editor.GetLogWithJSONGraph<EDIT_ARG_MIX>());

    const auto mix_a = MakeMix(100, 1);
    const auto mix_b = MakeMix(101, 2);

    std::optional<std::string> commit_1;
    std::optional<std::string> commit_2;
    REQUIRE(editor.AddLine<EDIT_ARG_MIX>(mix_a));
    REQUIRE(RecordNewCommitOid(
        [&editor]() { editor.UpdateLog<EDIT_ARG_MIX>(); },
        [&editor]() { return editor.GetLogWithJSONGraph<EDIT_ARG_MIX>(); },
        mix_oids,
        commit_1));

    REQUIRE(editor.AddLine<EDIT_ARG_MIX>(mix_b));
    REQUIRE(RecordNewCommitOid(
        [&editor]() { editor.UpdateLog<EDIT_ARG_MIX>(); },
        [&editor]() { return editor.GetLogWithJSONGraph<EDIT_ARG_MIX>(); },
        mix_oids,
        commit_2));

    const auto append_diff = editor.GetDiff<EDIT_ARG_MIX>(*commit_1, *commit_2);
    REQUIRE(append_diff.has_value());
    CHECK(append_diff->kind == PDJE_TIMELINE::TimeLineDiffKind::MIX);
    REQUIRE(append_diff->mixAdded.size() == 1);
    CHECK(append_diff->mixRemoved.empty());
    CHECK(append_diff->mixAdded.front().ID == mix_b.ID);
}

TEST_CASE("core: editorObject NOTE and KEY_VALUE diff report semantic changes")
{
    const auto    root = MakeTempRoot("note_kv");
    ScopedCleanup cleanup{ root };

    editorObject editor;
    REQUIRE(editor.Open(root, "diff-tester", "diff-tester@test"));

    editor.UpdateLog<EDIT_ARG_NOTE>();
    auto note_oids =
        CollectLogOids(editor.GetLogWithJSONGraph<EDIT_ARG_NOTE>());

    const auto note_a = MakeNote(1, 1);
    const auto note_b = MakeNote(2, 2);

    std::optional<std::string> note_commit_1;
    std::optional<std::string> note_commit_2;

    REQUIRE(editor.AddLine<EDIT_ARG_NOTE>(note_a));
    REQUIRE(RecordNewCommitOid(
        [&editor]() { editor.UpdateLog<EDIT_ARG_NOTE>(); },
        [&editor]() { return editor.GetLogWithJSONGraph<EDIT_ARG_NOTE>(); },
        note_oids,
        note_commit_1));

    REQUIRE(editor.AddLine<EDIT_ARG_NOTE>(note_b));
    REQUIRE(RecordNewCommitOid(
        [&editor]() { editor.UpdateLog<EDIT_ARG_NOTE>(); },
        [&editor]() { return editor.GetLogWithJSONGraph<EDIT_ARG_NOTE>(); },
        note_oids,
        note_commit_2));

    const auto note_diff =
        editor.GetDiff<EDIT_ARG_NOTE>(*note_commit_1, *note_commit_2);
    REQUIRE(note_diff.has_value());
    CHECK(note_diff->kind == PDJE_TIMELINE::TimeLineDiffKind::NOTE);
    CHECK(note_diff->noteRemoved.empty());
    REQUIRE(note_diff->noteAdded.size() == 1);
    CHECK(note_diff->noteAdded.front().railID == note_b.railID);
    CHECK(note_diff->noteAdded.front().beat == note_b.beat);

    editor.UpdateLog<EDIT_ARG_KEY_VALUE>();
    auto kv_oids =
        CollectLogOids(editor.GetLogWithJSONGraph<EDIT_ARG_KEY_VALUE>());
    std::optional<std::string> kv_commit_1;
    std::optional<std::string> kv_commit_2;

    REQUIRE(editor.AddLine<EDIT_ARG_KEY_VALUE>(KEY_VALUE{ "diff_key", "v1" }));
    REQUIRE(RecordNewCommitOid(
        [&editor]() { editor.UpdateLog<EDIT_ARG_KEY_VALUE>(); },
        [&editor]() {
            return editor.GetLogWithJSONGraph<EDIT_ARG_KEY_VALUE>();
        },
        kv_oids,
        kv_commit_1));

    REQUIRE(editor.AddLine<EDIT_ARG_KEY_VALUE>(KEY_VALUE{ "diff_key", "v2" }));
    REQUIRE(RecordNewCommitOid(
        [&editor]() { editor.UpdateLog<EDIT_ARG_KEY_VALUE>(); },
        [&editor]() {
            return editor.GetLogWithJSONGraph<EDIT_ARG_KEY_VALUE>();
        },
        kv_oids,
        kv_commit_2));

    const auto kv_diff =
        editor.GetDiff<EDIT_ARG_KEY_VALUE>(*kv_commit_1, *kv_commit_2);
    REQUIRE(kv_diff.has_value());
    CHECK(kv_diff->kind == PDJE_TIMELINE::TimeLineDiffKind::KV);
    REQUIRE(kv_diff->kvRemoved.size() == 1);
    REQUIRE(kv_diff->kvAdded.size() == 1);
    CHECK(kv_diff->kvRemoved.front().first == "diff_key");
    CHECK(kv_diff->kvAdded.front().first == "diff_key");
}

TEST_CASE("core: editorObject MUSIC diff requires an explicit target and "
          "returns semantic bpm changes")
{
    const auto    root = MakeTempRoot("music");
    ScopedCleanup cleanup{ root };

    const auto wav_path = root / "dummy.wav";
    REQUIRE(WriteSilentStereoWav(wav_path, 8));

    editorObject editor;
    REQUIRE(editor.Open(root, "diff-tester", "diff-tester@test"));
    REQUIRE(editor.ConfigNewMusic("music_target", "composer", wav_path, "0"));

    editor.UpdateLog<EDIT_ARG_MUSIC>();
    auto music_oids = CollectLogOids(
        editor.GetLogWithJSONGraph<EDIT_ARG_MUSIC>("music_target"));

    const auto music_a = MakeMusicEdit("music_target", "120", 0);
    const auto music_b = MakeMusicEdit("music_target", "140", 4);

    std::optional<std::string> commit_1;
    std::optional<std::string> commit_2;

    REQUIRE(editor.AddLine<EDIT_ARG_MUSIC>(music_a));
    REQUIRE(RecordNewCommitOid(
        [&editor]() { editor.UpdateLog<EDIT_ARG_MUSIC>(); },
        [&editor]() {
            return editor.GetLogWithJSONGraph<EDIT_ARG_MUSIC>("music_target");
        },
        music_oids,
        commit_1));

    REQUIRE(editor.AddLine<EDIT_ARG_MUSIC>(music_b));
    REQUIRE(RecordNewCommitOid(
        [&editor]() { editor.UpdateLog<EDIT_ARG_MUSIC>(); },
        [&editor]() {
            return editor.GetLogWithJSONGraph<EDIT_ARG_MUSIC>("music_target");
        },
        music_oids,
        commit_2));

    const auto music_diff =
        editor.GetDiff<EDIT_ARG_MUSIC>("music_target", *commit_1, *commit_2);
    REQUIRE(music_diff.has_value());
    CHECK(music_diff->kind == PDJE_TIMELINE::TimeLineDiffKind::MUSIC);
    CHECK(music_diff->musicBpmRemoved.empty());
    REQUIRE(music_diff->musicBpmAdded.size() == 1);
    CHECK(music_diff->musicBpmAdded.front().bpm == music_b.arg.bpm);

    REQUIRE(
        editor.ConfigNewMusic("music_target", "other_composer", wav_path, "0"));
    const auto duplicate_title_diff =
        editor.GetDiff<EDIT_ARG_MUSIC>("music_target", *commit_1, *commit_2);
    REQUIRE(duplicate_title_diff.has_value());
    CHECK(duplicate_title_diff->musicBpmAdded.size() == 1);
    CHECK(duplicate_title_diff->musicBpmAdded.front().bpm == music_b.arg.bpm);

    CHECK_FALSE(editor.GetDiff<EDIT_ARG_MUSIC>("missing", *commit_1, *commit_2)
                    .has_value());
}

TEST_CASE("core: editorObject diff invalid OIDs fail safely")
{
    const auto    root = MakeTempRoot("invalid");
    ScopedCleanup cleanup{ root };

    const auto wav_path = root / "dummy.wav";
    REQUIRE(WriteSilentStereoWav(wav_path, 8));

    editorObject editor;
    REQUIRE(editor.Open(root, "diff-tester", "diff-tester@test"));
    REQUIRE(editor.ConfigNewMusic("music_target", "composer", wav_path, "0"));

    CHECK_FALSE(editor.GetDiff<EDIT_ARG_MIX>("not-an-oid", "still-not-an-oid")
                    .has_value());
    CHECK_FALSE(editor.GetDiff<EDIT_ARG_NOTE>("not-an-oid", "still-not-an-oid")
                    .has_value());
    CHECK_FALSE(
        editor.GetDiff<EDIT_ARG_KEY_VALUE>("not-an-oid", "still-not-an-oid")
            .has_value());
    CHECK_FALSE(editor
                    .GetDiff<EDIT_ARG_MUSIC>(
                        "music_target", "not-an-oid", "still-not-an-oid")
                    .has_value());
}

TEST_CASE(
    "core: editorObject diff returns an empty semantic result for same commit")
{
    const auto    root = MakeTempRoot("same_commit");
    ScopedCleanup cleanup{ root };

    const auto wav_path = root / "dummy.wav";
    REQUIRE(WriteSilentStereoWav(wav_path, 8));

    editorObject editor;
    REQUIRE(editor.Open(root, "diff-tester", "diff-tester@test"));

    editor.UpdateLog<EDIT_ARG_MIX>();
    auto mix_oids = CollectLogOids(editor.GetLogWithJSONGraph<EDIT_ARG_MIX>());

    std::optional<std::string> mix_commit;
    REQUIRE(editor.AddLine<EDIT_ARG_MIX>(MakeMix(201, 8)));
    REQUIRE(RecordNewCommitOid(
        [&editor]() { editor.UpdateLog<EDIT_ARG_MIX>(); },
        [&editor]() { return editor.GetLogWithJSONGraph<EDIT_ARG_MIX>(); },
        mix_oids,
        mix_commit));

    const auto mix_diff =
        editor.GetDiff<EDIT_ARG_MIX>(*mix_commit, *mix_commit);
    REQUIRE(mix_diff.has_value());
    CHECK(mix_diff->kind == PDJE_TIMELINE::TimeLineDiffKind::MIX);
    CHECK(mix_diff->Empty());

    REQUIRE(
        editor.ConfigNewMusic("same_commit_song", "composer", wav_path, "0"));
    editor.UpdateLog<EDIT_ARG_MUSIC>();
    auto music_oids = CollectLogOids(
        editor.GetLogWithJSONGraph<EDIT_ARG_MUSIC>("same_commit_song"));

    std::optional<std::string> music_commit;
    REQUIRE(editor.AddLine<EDIT_ARG_MUSIC>(
        MakeMusicEdit("same_commit_song", "128", 0)));
    REQUIRE(RecordNewCommitOid(
        [&editor]() { editor.UpdateLog<EDIT_ARG_MUSIC>(); },
        [&editor]() {
            return editor.GetLogWithJSONGraph<EDIT_ARG_MUSIC>(
                "same_commit_song");
        },
        music_oids,
        music_commit));

    const auto music_diff = editor.GetDiff<EDIT_ARG_MUSIC>(
        "same_commit_song", *music_commit, *music_commit);
    REQUIRE(music_diff.has_value());
    CHECK(music_diff->kind == PDJE_TIMELINE::TimeLineDiffKind::MUSIC);
    CHECK(music_diff->Empty());
}
