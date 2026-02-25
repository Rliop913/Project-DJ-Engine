#pragma once

#include "PDJE_Input_Device_Data.hpp"

#include <string_view>

namespace PDJE_DEFAULT_DEVICES::LINUX_INPUT_CONTRACTS {

inline constexpr std::string_view kWaylandSchemePrefix = "wayland://";
inline constexpr char             kWaylandKeyboardId[] = "wayland://keyboard";
inline constexpr char             kWaylandPointerId[]  = "wayland://pointer";

inline constexpr char kWaylandKeyboardName[] =
    "Wayland Keyboard (Focused Surface)";
inline constexpr char kWaylandPointerName[] = "Wayland Pointer (Focused Surface)";

inline constexpr char kWaylandInternalKeyboardName[] =
    "Wayland Keyboard (PDJE Internal Window)";
inline constexpr char kWaylandInternalPointerName[] =
    "Wayland Pointer (PDJE Internal Window)";

inline bool
IsWaylandSyntheticId(std::string_view id) noexcept
{
    return id.rfind(kWaylandSchemePrefix, 0) == 0;
}

inline const char *
GetWaylandSyntheticName(PDJE_Dev_Type type, bool internal_window) noexcept
{
    switch (type) {
    case PDJE_Dev_Type::KEYBOARD:
        return internal_window ? kWaylandInternalKeyboardName
                               : kWaylandKeyboardName;
    case PDJE_Dev_Type::MOUSE:
        return internal_window ? kWaylandInternalPointerName
                               : kWaylandPointerName;
    default:
        return nullptr;
    }
}

} // namespace PDJE_DEFAULT_DEVICES::LINUX_INPUT_CONTRACTS
