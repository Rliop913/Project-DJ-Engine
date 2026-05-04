#include "windows_keyboard_press_tracker.hpp"

#include <doctest/doctest.h>

TEST_CASE("input/windows: keyboard press tracker suppresses repeats per device")
{
    PDJE_RAWINPUT::KeyboardPressTracker tracker;

    constexpr std::uintptr_t keyboard_a = 1001;
    constexpr std::uintptr_t keyboard_b = 2002;

    CHECK(tracker.ShouldWrite(keyboard_a, PDJE_KEY::A, true));
    CHECK_FALSE(tracker.ShouldWrite(keyboard_a, PDJE_KEY::A, true));

    CHECK(tracker.ShouldWrite(keyboard_b, PDJE_KEY::A, true));
    CHECK_FALSE(tracker.ShouldWrite(keyboard_b, PDJE_KEY::A, true));

    CHECK(tracker.ShouldWrite(keyboard_a, PDJE_KEY::A, false));
    CHECK_FALSE(tracker.ShouldWrite(keyboard_b, PDJE_KEY::A, true));

    CHECK(tracker.ShouldWrite(keyboard_a, PDJE_KEY::A, true));
}

TEST_CASE("input/windows: keyboard press tracker keeps keys independent")
{
    PDJE_RAWINPUT::KeyboardPressTracker tracker;

    constexpr std::uintptr_t keyboard = 3003;

    CHECK(tracker.ShouldWrite(keyboard, PDJE_KEY::A, true));
    CHECK(tracker.ShouldWrite(keyboard, PDJE_KEY::B, true));
    CHECK_FALSE(tracker.ShouldWrite(keyboard, PDJE_KEY::A, true));
    CHECK_FALSE(tracker.ShouldWrite(keyboard, PDJE_KEY::B, true));

    CHECK(tracker.ShouldWrite(keyboard, PDJE_KEY::A, false));
    CHECK(tracker.ShouldWrite(keyboard, PDJE_KEY::A, true));
    CHECK_FALSE(tracker.ShouldWrite(keyboard, PDJE_KEY::B, true));
}
