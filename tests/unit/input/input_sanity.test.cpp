#include "Input_State.hpp"
#include "PDJE_Input_Device_Data.hpp"
#include <doctest/doctest.h>

#if defined(__linux__)
#include "evdev_codemap.hpp"
#include <libevdev/libevdev.h>
#endif

TEST_CASE("input: device data struct stores values")
{
    DeviceData d;
    d.Type               = PDJE_Dev_Type::KEYBOARD;
    d.Name               = "Keyboard";
    d.device_specific_id = "dev://kbd0";

    CHECK(d.Type == PDJE_Dev_Type::KEYBOARD);
    CHECK(d.Name == "Keyboard");
    CHECK(d.device_specific_id == "dev://kbd0");
}

TEST_CASE("input: key enum contains expected sentinel ordering")
{
    CHECK(static_cast<int>(PDJE_KEY::F_1) == 0);
    CHECK(static_cast<int>(PDJE_KEY::UNKNOWN) >
          static_cast<int>(PDJE_KEY::A));
}

#if defined(__linux__)
TEST_CASE("input/linux: evdev codemap maps common keys")
{
    CHECK(PDJE_EVDEV_KEYMAP::keyboard_map(KEY_A) == PDJE_KEY::A);
    CHECK(PDJE_EVDEV_KEYMAP::keyboard_map(KEY_SPACE) == PDJE_KEY::SPACE);
    CHECK(PDJE_EVDEV_KEYMAP::keyboard_map(static_cast<__u16>(KEY_MAX + 1)) ==
          PDJE_KEY::UNKNOWN);
}
#endif
