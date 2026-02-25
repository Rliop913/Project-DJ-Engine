#include "TrackLinterRules.hpp"
#include <doctest/doctest.h>

TEST_CASE("core: track linter rules accumulate initializes ids and counts conditionally")
{
    PDJE_TRACK_LINTER_RULES::TrackLinterIdCount counts;

    PDJE_TRACK_LINTER_RULES::AccumulateIf(counts, 42, false);
    REQUIRE(counts.contains(42));
    CHECK(counts[42] == 0);

    PDJE_TRACK_LINTER_RULES::AccumulateIf(counts, 42, true);
    PDJE_TRACK_LINTER_RULES::AccumulateIf(counts, 42, true);
    CHECK(counts[42] == 2);
}

TEST_CASE("core: track linter rules validation succeeds when every id appears exactly once")
{
    PDJE_TRACK_LINTER_RULES::TrackLinterIdCount counts;
    PDJE_TRACK_LINTER_RULES::AccumulateIf(counts, 1, true);
    PDJE_TRACK_LINTER_RULES::AccumulateIf(counts, 2, true);
    PDJE_TRACK_LINTER_RULES::AccumulateIf(counts, 3, true);

    UNSANITIZED msg;
    CHECK(PDJE_TRACK_LINTER_RULES::ValidateExactlyOne(counts, "load", msg));
    CHECK(msg.empty());
}

TEST_CASE("core: track linter rules validation reports duplicate and missing commands")
{
    PDJE_TRACK_LINTER_RULES::TrackLinterIdCount counts;
    PDJE_TRACK_LINTER_RULES::AccumulateIf(counts, 10, true);
    PDJE_TRACK_LINTER_RULES::AccumulateIf(counts, 10, true);
    PDJE_TRACK_LINTER_RULES::AccumulateIf(counts, 11, false);

    UNSANITIZED msg;
    CHECK_FALSE(PDJE_TRACK_LINTER_RULES::ValidateExactlyOne(counts, "unload", msg));
    CHECK(msg.find("ID 10") != std::string::npos);
    CHECK(msg.find("2 unload command") != std::string::npos);
    CHECK(msg.find("ID 11") != std::string::npos);
    CHECK(msg.find("no unload command") != std::string::npos);
}

TEST_CASE("core: track linter rules keep counts independent across mixed ids")
{
    PDJE_TRACK_LINTER_RULES::TrackLinterIdCount counts;
    PDJE_TRACK_LINTER_RULES::AccumulateIf(counts, 100, true);
    PDJE_TRACK_LINTER_RULES::AccumulateIf(counts, 101, false);
    PDJE_TRACK_LINTER_RULES::AccumulateIf(counts, 102, true);
    PDJE_TRACK_LINTER_RULES::AccumulateIf(counts, 102, false);

    CHECK(counts[100] == 1);
    CHECK(counts[101] == 0);
    CHECK(counts[102] == 1);
}
