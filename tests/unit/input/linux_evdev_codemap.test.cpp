#include <doctest/doctest.h>

#if defined(__linux__)
#include "evdev_codemap.hpp"
#include <libevdev/libevdev.h>

TEST_CASE("input/linux: evdev codemap covers representative key groups")
{
    CHECK(PDJE_EVDEV_KEYMAP::keyboard_map(KEY_A) == PDJE_KEY::A);
    CHECK(PDJE_EVDEV_KEYMAP::keyboard_map(KEY_Z) == PDJE_KEY::Z);
    CHECK(PDJE_EVDEV_KEYMAP::keyboard_map(KEY_0) == PDJE_KEY::D0);
    CHECK(PDJE_EVDEV_KEYMAP::keyboard_map(KEY_9) == PDJE_KEY::D9);
    CHECK(PDJE_EVDEV_KEYMAP::keyboard_map(KEY_F1) == PDJE_KEY::F_1);
    CHECK(PDJE_EVDEV_KEYMAP::keyboard_map(KEY_F12) == PDJE_KEY::F_12);
    CHECK(PDJE_EVDEV_KEYMAP::keyboard_map(KEY_LEFT) == PDJE_KEY::LEFT);
    CHECK(PDJE_EVDEV_KEYMAP::keyboard_map(KEY_RIGHT) == PDJE_KEY::RIGHT);
    CHECK(PDJE_EVDEV_KEYMAP::keyboard_map(KEY_LEFTSHIFT) == PDJE_KEY::LSHIFT);
    CHECK(PDJE_EVDEV_KEYMAP::keyboard_map(KEY_RIGHTALT) == PDJE_KEY::RALT);
    CHECK(PDJE_EVDEV_KEYMAP::keyboard_map(KEY_KPENTER) == PDJE_KEY::KP_ENTER);
    CHECK(PDJE_EVDEV_KEYMAP::keyboard_map(KEY_KPPLUS) == PDJE_KEY::KP_PLUS);
}

TEST_CASE("input/linux: evdev codemap rejects out-of-range codes")
{
    CHECK(PDJE_EVDEV_KEYMAP::keyboard_map(static_cast<__u16>(KEY_MAX + 1)) ==
          PDJE_KEY::UNKNOWN);
}

TEST_CASE("input/linux: evdev codemap roundtrips mapped PDJE keys")
{
    for (std::size_t pk = 0; pk < PDJE_EVDEV_KEYMAP::kPdjeToEvdev.size(); ++pk) {
        const auto ev = PDJE_EVDEV_KEYMAP::kPdjeToEvdev[pk];
        if (ev == KEY_RESERVED || ev > KEY_MAX) {
            continue;
        }
        CHECK(PDJE_EVDEV_KEYMAP::keyboard_map(ev) ==
              static_cast<PDJE_KEY>(pk));
    }
}
#endif
