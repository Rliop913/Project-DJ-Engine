#include "TimeLineDiffTypes.hpp"
#include <doctest/doctest.h>

TEST_CASE("core: timeline diff result empty contract and default kind")
{
    PDJE_TIMELINE::TimeLineSemanticDiffResult diff;
    CHECK(diff.kind == PDJE_TIMELINE::TimeLineDiffKind::KV);
    CHECK(diff.Empty());
}

TEST_CASE("core: timeline diff result Empty becomes false when representative groups are populated")
{
    {
        PDJE_TIMELINE::TimeLineSemanticDiffResult diff;
        diff.kvAdded.push_back(KEY_VALUE{ "k", "v" });
        CHECK_FALSE(diff.Empty());
    }

    {
        PDJE_TIMELINE::TimeLineSemanticDiffResult diff;
        diff.mixRemoved.push_back(MixArgs{});
        CHECK_FALSE(diff.Empty());
    }

    {
        PDJE_TIMELINE::TimeLineSemanticDiffResult diff;
        diff.musicMetaAdded.push_back(
            PDJE_TIMELINE::MusicMetaFieldDiff{ .key = "title", .value = "x" });
        CHECK_FALSE(diff.Empty());
    }
}
