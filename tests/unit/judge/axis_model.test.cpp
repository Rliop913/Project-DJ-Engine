#include "AxisModel.hpp"
#include <doctest/doctest.h>

#include <cmath>
#include <limits>

using PDJE_JUDGE::AXIS_MODEL;

TEST_CASE("axis: absolute positions map endpoints and center and saturate")
{
    AXIS_MODEL axis(10.0, 30.0, 90.0);
    CHECK(axis.Value() == 0.0);
    CHECK(axis.Set(10.0) == -1.0);
    CHECK(axis.Set(20.0) == -0.5);
    CHECK(axis.Set(30.0) == 0.0);
    CHECK(axis.Set(60.0) == 0.5);
    CHECK(axis.Set(90.0) == 1.0);
    CHECK(axis.Set(-100.0) == -1.0);
    CHECK(axis.Set(100.0) == 1.0);
}

TEST_CASE(
    "axis: relative motion saturates without accumulating hidden overshoot")
{
    AXIS_MODEL axis(-100.0, 0.0, 100.0);
    CHECK(axis.Move(25.0) == 0.25);
    CHECK(axis.Move(25.0) == 0.5);
    CHECK(axis.Move(1000.0) == 1.0);
    CHECK(axis.Move(-25.0) == 0.75);
    CHECK(axis.Move(-1000.0) == -1.0);
    CHECK(axis.Move(25.0) == -0.75);
    CHECK(axis.Move(0.0) == -0.75);
    axis.Reset();
    CHECK(axis.Value() == 0.0);
    CHECK(axis.Set(50.0) == 0.5);
    CHECK(axis.Move(-25.0) == 0.25);
}

TEST_CASE("axis: invalid samples preserve the previous state")
{
    AXIS_MODEL axis;
    REQUIRE(axis.Set(0.5) == 0.5);
    for (double value : { std::numeric_limits<double>::quiet_NaN(),
                          std::numeric_limits<double>::infinity(),
                          -std::numeric_limits<double>::infinity() }) {
        CHECK_FALSE(axis.Set(value).has_value());
        CHECK(axis.Value() == 0.5);
        CHECK_FALSE(axis.Move(value).has_value());
        CHECK(axis.Value() == 0.5);
    }
}

TEST_CASE("axis: invalid calibration is rejected at construction")
{
    const double inf = std::numeric_limits<double>::infinity();
    const double nan = std::numeric_limits<double>::quiet_NaN();
    const double max = std::numeric_limits<double>::max();
    CHECK_THROWS_AS(AXIS_MODEL(0.0, 0.0, 1.0), std::invalid_argument);
    CHECK_THROWS_AS(AXIS_MODEL(0.0, 1.0, 1.0), std::invalid_argument);
    CHECK_THROWS_AS(AXIS_MODEL(2.0, 1.0, 0.0), std::invalid_argument);
    CHECK_THROWS_AS(AXIS_MODEL(nan, 0.0, 1.0), std::invalid_argument);
    CHECK_THROWS_AS(AXIS_MODEL(-1.0, nan, 1.0), std::invalid_argument);
    CHECK_THROWS_AS(AXIS_MODEL(-1.0, 0.0, nan), std::invalid_argument);
    CHECK_THROWS_AS(AXIS_MODEL(-inf, 0.0, 1.0), std::invalid_argument);
    CHECK_THROWS_AS(AXIS_MODEL(-1.0, inf, 1.0), std::invalid_argument);
    CHECK_THROWS_AS(AXIS_MODEL(-1.0, 0.0, inf), std::invalid_argument);
    CHECK_THROWS_AS(AXIS_MODEL(-max, 0.0, max), std::invalid_argument);
}

TEST_CASE("axis: finite extreme deltas do not overflow and tiny ranges work")
{
    const double max = std::numeric_limits<double>::max();
    AXIS_MODEL   positive(0.0, max / 2.0, max);
    CHECK(positive.Move(max) == 1.0);
    CHECK(positive.Move(-max) == -1.0);
    AXIS_MODEL negative(-max, -max / 2.0, 0.0);
    CHECK(negative.Move(-max) == -1.0);
    CHECK(negative.Move(max) == 1.0);

    const double small = std::numeric_limits<double>::min();
    AXIS_MODEL   tiny(-small, 0.0, small);
    CHECK(tiny.Set(-small) == -1.0);
    CHECK(tiny.Set(0.0) == 0.0);
    CHECK(tiny.Set(small) == 1.0);
}

TEST_CASE(
    "axis: full pitch bend domain is monotonic bounded and exactly centered")
{
    AXIS_MODEL axis(0.0, 8192.0, 16383.0);
    double     previous = -1.0;
    for (int raw = 0; raw <= 16383; ++raw) {
        const auto value = axis.Set(raw);
        REQUIRE(value.has_value());
        CHECK(std::isfinite(*value));
        CHECK(*value >= previous);
        CHECK(*value >= -1.0);
        CHECK(*value <= 1.0);
        previous = *value;
    }
    CHECK(axis.Set(8192.0) == 0.0);
    CHECK(axis.Set(0.0) == -1.0);
    CHECK(axis.Set(16383.0) == 1.0);
}

TEST_CASE("axis: instances do not share state")
{
    AXIS_MODEL first;
    AXIS_MODEL second;
    CHECK(first.Move(1.0) == 1.0);
    CHECK(second.Value() == 0.0);
    auto copy = first;
    copy.Reset();
    CHECK(first.Value() == 1.0);
    CHECK(copy.Value() == 0.0);
}
