#include "FrameCalcCore.hpp"
#include <doctest/doctest.h>

TEST_CASE("core: framecalc CountFrame computes basic beat ranges at 48kHz")
{
    CHECK(FrameCalc::CountFrame(0, 0, 4, 0, 0, 4, 120.0) == 0);
    CHECK(FrameCalc::CountFrame(0, 0, 4, 1, 0, 4, 120.0) == 24000);
    CHECK(FrameCalc::CountFrame(0, 0, 4, 4, 0, 4, 120.0) == 96000);
    CHECK(FrameCalc::CountFrame(0, 0, 4, 0, 1, 4, 120.0) == 6000);
}

TEST_CASE("core: framecalc CountFrame guards zero separators and non-positive bpm")
{
    CHECK(FrameCalc::CountFrame(0, 0, 0, 1, 0, 0, 120.0) == 24000);
    CHECK(FrameCalc::CountFrame(0, 0, 4, 1, 0, 4, 0.0) == 2880000);
    CHECK(FrameCalc::CountFrame(0, 0, 4, 1, 0, 4, -12.0) == 2880000);
}

TEST_CASE("core: framecalc CountFrame uses round-based characterization")
{
    CHECK(FrameCalc::CountFrame(0, 0, 1, 1, 0, 1, 123.0) == 23415);
    CHECK(FrameCalc::CountFrame(0, 0, 8, 0, 1, 8, 137.0) == 2628);
}
