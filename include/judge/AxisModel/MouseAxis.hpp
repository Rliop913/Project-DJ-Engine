#pragma once

#include "AxisModel.hpp"
#include "Input_State.hpp"
#include <array>

namespace PDJE_JUDGE {
/**
 * Caller chooses the coordinate range (including the virtual desktop range).
 * REL accumulates in that range; ABS and VIRTUAL_DESKTOP_ABS replace position.
 * Keep separate instances for devices/coordinate spaces; Reset on reconnect.
 * Update(x, y, type) can be called directly from custom_mouse_parse.
 */
class MOUSE_AXIS_MODEL {
  private:
    AXIS_MODEL x_;
    AXIS_MODEL y_;

  public:
    MOUSE_AXIS_MODEL(AXIS_MODEL x, AXIS_MODEL y) : x_(x), y_(y)
    {
    }

    [[nodiscard]] std::optional<std::array<double, 2>>
    Update(int x, int y, PDJE_Mouse_Axis_Type type) noexcept
    {
        switch (type) {
        case REL:
            return std::array<double, 2>{ *x_.Move(x), *y_.Move(y) };
        case ABS:
        case VIRTUAL_DESKTOP_ABS:
            return std::array<double, 2>{ *x_.Set(x), *y_.Set(y) };
        default:
            return std::nullopt;
        }
    }

    [[nodiscard]] std::optional<std::array<double, 2>>
    Update(const PDJE_Mouse_Event &event) noexcept
    {
        return Update(event.x, event.y, event.axis_type);
    }

    void
    Reset() noexcept
    {
        x_.Reset();
        y_.Reset();
    }
};
} // namespace PDJE_JUDGE
