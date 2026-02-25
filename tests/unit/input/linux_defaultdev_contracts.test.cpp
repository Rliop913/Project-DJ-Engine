#include <doctest/doctest.h>

#if defined(__linux__)
#include "LinuxInputContracts.hpp"

TEST_CASE("input/linux: wayland synthetic id detection is strict")
{
    using namespace PDJE_DEFAULT_DEVICES::LINUX_INPUT_CONTRACTS;

    CHECK(IsWaylandSyntheticId("wayland://keyboard"));
    CHECK(IsWaylandSyntheticId("wayland://pointer"));

    CHECK_FALSE(IsWaylandSyntheticId(""));
    CHECK_FALSE(IsWaylandSyntheticId("xwayland://keyboard"));
    CHECK_FALSE(IsWaylandSyntheticId("wayland:/pointer"));
    CHECK_FALSE(IsWaylandSyntheticId("wayland//pointer"));
}

TEST_CASE("input/linux: wayland synthetic ids and names match contracts")
{
    using namespace PDJE_DEFAULT_DEVICES::LINUX_INPUT_CONTRACTS;

    CHECK(std::string_view(kWaylandKeyboardId) == "wayland://keyboard");
    CHECK(std::string_view(kWaylandPointerId) == "wayland://pointer");

    CHECK(std::string_view(GetWaylandSyntheticName(PDJE_Dev_Type::KEYBOARD, false)) ==
          "Wayland Keyboard (Focused Surface)");
    CHECK(std::string_view(GetWaylandSyntheticName(PDJE_Dev_Type::MOUSE, false)) ==
          "Wayland Pointer (Focused Surface)");
    CHECK(std::string_view(GetWaylandSyntheticName(PDJE_Dev_Type::KEYBOARD, true)) ==
          "Wayland Keyboard (PDJE Internal Window)");
    CHECK(std::string_view(GetWaylandSyntheticName(PDJE_Dev_Type::MOUSE, true)) ==
          "Wayland Pointer (PDJE Internal Window)");
    CHECK(GetWaylandSyntheticName(PDJE_Dev_Type::UNKNOWN, false) == nullptr);
}
#endif
