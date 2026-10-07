#include <doctest/doctest.h>

#include "CapnpBinary.hpp"
#include "PDJE_interface.hpp"

#include <chrono>
#include <filesystem>
#include <string>

namespace {
struct TempRoot {
    std::filesystem::path path =
        std::filesystem::temp_directory_path() /
        ("pdje_note_translation_" +
         std::to_string(
             std::chrono::steady_clock::now().time_since_epoch().count()));

    ~TempRoot()
    {
        std::error_code ec;
        std::filesystem::remove_all(path, ec);
    }
};

trackdata
MakeNoteTrack(const char *mix_bpm,
              bool        empty_notes,
              const char *note_bpm = nullptr)
{
    CapWriter<MixBinaryCapnpData> mix;
    REQUIRE(mix.makeNew());
    auto bpm = mix.Wp->initDatas(1)[0];
    bpm.setType(TypeEnum::BPM_CONTROL);
    bpm.setFirst(mix_bpm);
    bpm.setSeparate(1);
    bpm.setEseparate(1);

    CapWriter<NoteBinaryCapnpData> notes;
    REQUIRE(notes.makeNew());
    auto data = notes.Wp->initDatas(empty_notes ? 0 : (note_bpm ? 2 : 1));
    if (!empty_notes) {
        auto note = data[0];
        note.setNoteType("tap");
        note.setNoteDetail(3);
        note.setFirst("one");
        note.setBeat(1);
        note.setSeparate(1);
        note.setRailID(7);
        if (note_bpm) {
            auto change = data[1];
            change.setNoteType("BPM");
            change.setFirst(note_bpm);
            change.setBeat(2);
            change.setSeparate(1);
        }
    }
    trackdata track;
    track.mixBinary  = mix.out();
    track.noteBinary = notes.out();
    return track;
}
} // namespace

TEST_CASE("core: GetNoteObjects preserves successful normal and empty charts")
{
    TempRoot root;
    PDJE     engine(root.path.generic_string());
    bool     empty = false;
    SUBCASE("normal chart")
    {
    }
    SUBCASE("empty chart")
    {
        empty = true;
    }
    auto                track   = MakeNoteTrack("120", empty);
    int                 calls   = 0;
    OBJ_SETTER_CALLBACK collect = [&](const auto &type,
                                      auto        detail,
                                      const auto &first,
                                      const auto &,
                                      const auto &,
                                      auto frame,
                                      auto end_frame,
                                      auto rail) {
        ++calls;
        CHECK(type == "tap");
        CHECK(detail == 3);
        CHECK(first == "one");
        CHECK(frame == 24000);
        CHECK(end_frame == 0);
        CHECK(rail == 7);
    };
    CHECK(engine.GetNoteObjects(track, collect));
    CHECK(calls == (empty ? 0 : 1));
}

TEST_CASE("core: GetNoteObjects propagates mix and note translation failures")
{
    TempRoot root;
    PDJE     engine(root.path.generic_string());
    auto     track = MakeNoteTrack("120", false);
    SUBCASE("mix conversion failure")
    {
        track = MakeNoteTrack("0", false);
    }
    SUBCASE("note conversion failure")
    {
        track = MakeNoteTrack("120", false, "0");
    }
    int                 calls   = 0;
    OBJ_SETTER_CALLBACK collect = [&](const auto &,
                                      auto,
                                      const auto &,
                                      const auto &,
                                      const auto &,
                                      auto,
                                      auto,
                                      auto) { ++calls; };
    CHECK_FALSE(engine.GetNoteObjects(track, collect));
    CHECK(calls == 0);
}

TEST_CASE("core: GetNoteObjects preserves callback failure behavior")
{
    TempRoot root;
    PDJE     engine(root.path.generic_string());
    auto     track = MakeNoteTrack("120", false);
    SUBCASE("empty callback is rejected")
    {
        OBJ_SETTER_CALLBACK collect;
        CHECK_FALSE(engine.GetNoteObjects(track, collect));
    }
    SUBCASE("callback exceptions propagate through RAII cleanup")
    {
        struct CallbackFailure {};
        OBJ_SETTER_CALLBACK collect = [](const auto &,
                                         auto,
                                         const auto &,
                                         const auto &,
                                         const auto &,
                                         auto,
                                         auto,
                                         auto) { throw CallbackFailure{}; };
        CHECK_THROWS_AS(engine.GetNoteObjects(track, collect), CallbackFailure);
    }
}
