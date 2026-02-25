#include <doctest/doctest.h>

#if defined(__linux__)
#include "LinuxMouseButtonMapping.hpp"

TEST_CASE("input/linux: mouse button mapping covers supported buttons")
{
    using PDJE_DEFAULT_DEVICES::LINUX_INPUT_MAP::TryMapLinuxMouseButton;

    CHECK(TryMapLinuxMouseButton(BTN_LEFT, true).value() ==
          PDJE_MOUSE_L_BTN_DOWN);
    CHECK(TryMapLinuxMouseButton(BTN_LEFT, false).value() == PDJE_MOUSE_L_BTN_UP);

    CHECK(TryMapLinuxMouseButton(BTN_RIGHT, true).value() ==
          PDJE_MOUSE_R_BTN_DOWN);
    CHECK(TryMapLinuxMouseButton(BTN_RIGHT, false).value() ==
          PDJE_MOUSE_R_BTN_UP);

    CHECK(TryMapLinuxMouseButton(BTN_MIDDLE, true).value() ==
          PDJE_MOUSE_M_BTN_DOWN);
    CHECK(TryMapLinuxMouseButton(BTN_MIDDLE, false).value() ==
          PDJE_MOUSE_M_BTN_UP);

    CHECK(TryMapLinuxMouseButton(BTN_SIDE, true).value() ==
          PDJE_MOUSE_SIDE_BTN_DOWN);
    CHECK(TryMapLinuxMouseButton(BTN_SIDE, false).value() ==
          PDJE_MOUSE_SIDE_BTN_UP);

    CHECK(TryMapLinuxMouseButton(BTN_EXTRA, true).value() ==
          PDJE_MOUSE_EX_BTN_DOWN);
    CHECK(TryMapLinuxMouseButton(BTN_EXTRA, false).value() ==
          PDJE_MOUSE_EX_BTN_UP);
}

TEST_CASE("input/linux: mouse button mapping rejects unsupported button codes")
{
    using PDJE_DEFAULT_DEVICES::LINUX_INPUT_MAP::TryMapLinuxMouseButton;

    CHECK_FALSE(TryMapLinuxMouseButton(BTN_TOUCH, true).has_value());
    CHECK_FALSE(TryMapLinuxMouseButton(0xFFFFu, false).has_value());
}
#endif
