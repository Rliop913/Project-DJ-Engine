#include "FrameCalcCore.hpp"
#include <doctest/doctest.h>

namespace {

BpmStruct
MakeSampleBpmStructUnsorted()
{
    BpmStruct s;
    s.fragments.push_back(BpmFragment{ .beat = 8, .subBeat = 0, .separate = 4, .bpm = 180.0 });
    s.fragments.push_back(BpmFragment{ .beat = 0, .subBeat = 0, .separate = 4, .bpm = 120.0 });
    s.fragments.push_back(BpmFragment{ .beat = 4, .subBeat = 0, .separate = 4, .bpm = 60.0 });
    return s;
}

} // namespace

TEST_CASE("core: bpm struct sorts fragments by beat/subBeat position")
{
    auto s = MakeSampleBpmStructUnsorted();
    s.sortFragment();

    REQUIRE(s.fragments.size() == 3);
    CHECK(s.fragments[0].beat == 0);
    CHECK(s.fragments[1].beat == 4);
    CHECK(s.fragments[2].beat == 8);
}

TEST_CASE("core: bpm struct calcFrame rejects empty and non-positive bpm fragments")
{
    BpmStruct empty;
    CHECK_FALSE(empty.calcFrame());

    BpmStruct invalid;
    invalid.fragments.push_back(BpmFragment{ .beat = 0, .subBeat = 0, .separate = 4, .bpm = 0.0 });
    invalid.fragments.push_back(BpmFragment{ .beat = 4, .subBeat = 0, .separate = 4, .bpm = 120.0 });
    CHECK_FALSE(invalid.calcFrame());
}

TEST_CASE("core: bpm struct calcFrame computes cumulative frame positions")
{
    auto s = MakeSampleBpmStructUnsorted();
    s.sortFragment();
    REQUIRE(s.calcFrame());

    REQUIRE(s.fragments.size() == 3);
    CHECK(s.fragments[0].frame_to_here == 0);
    CHECK(s.fragments[1].frame_to_here == 96000);
    CHECK(s.fragments[2].frame_to_here == 288000);
}

TEST_CASE("core: bpm struct getAffected returns expected fragment by position and frame")
{
    auto s = MakeSampleBpmStructUnsorted();
    s.sortFragment();
    REQUIRE(s.calcFrame());

    BpmFragment search;
    search.beat     = 5;
    search.subBeat  = 0;
    search.separate = 4;

    const auto &by_pos = s.getAffected(search);
    CHECK(by_pos.beat == 4);
    CHECK(by_pos.bpm == doctest::Approx(60.0));

    const auto &by_frame = s.getAffected(static_cast<unsigned long long>(100000));
    CHECK(by_frame.beat == 4);
    CHECK(by_frame.frame_to_here == 96000);
}

TEST_CASE("core: bpm struct getAffectedList respects same-fragment empty contract and ranges")
{
    auto s = MakeSampleBpmStructUnsorted();
    s.sortFragment();
    REQUIRE(s.calcFrame());

    const auto same = s.getAffectedList(1000, 10000);
    CHECK(same.empty());

    const auto ranged = s.getAffectedList(1000, 100000);
    REQUIRE(ranged.size() == 2);
    REQUIRE(ranged[0] != nullptr);
    REQUIRE(ranged[1] != nullptr);
    CHECK(ranged[0]->beat == 0);
    CHECK(ranged[1]->beat == 4);
}
