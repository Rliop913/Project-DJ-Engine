#pragma once

#include <algorithm>
#include <cmath>
#include <optional>
#include <stdexcept>

namespace PDJE_JUDGE {
/**
 * One bounded axis, initially at center. Use one instance per input
 * source/axis. Set consumes an absolute coordinate; Move accumulates a relative
 * delta. Output is piecewise linear: minimum -> -1, center -> 0, maximum -> 1.
 * Finite out-of-range inputs saturate. Non-finite inputs leave state unchanged
 * and return nullopt. This mutable model requires caller synchronization.
 */
class AXIS_MODEL {
  private:
    double minimum_;
    double center_;
    double maximum_;
    double position_;

  public:
    /** Require finite minimum < center < maximum and finite total width. */
    AXIS_MODEL(double minimum = -1.0, double center = 0.0, double maximum = 1.0)
        : minimum_(minimum), center_(center), maximum_(maximum),
          position_(center)
    {
        if (!std::isfinite(minimum) || !std::isfinite(center) ||
            !std::isfinite(maximum) ||
            !(minimum < center && center < maximum) ||
            !std::isfinite(maximum - minimum)) {
            throw std::invalid_argument("AXIS_MODEL: invalid axis range");
        }
    }

    [[nodiscard]] std::optional<double>
    Set(double coordinate) noexcept
    {
        if (!std::isfinite(coordinate)) {
            return std::nullopt;
        }
        position_ = std::clamp(coordinate, minimum_, maximum_);
        return Value();
    }

    [[nodiscard]] std::optional<double>
    Move(double delta) noexcept
    {
        if (!std::isfinite(delta)) {
            return std::nullopt;
        }
        // Compare before adding: even finite deltas can overflow a sum.
        if (delta >= maximum_ - position_) {
            position_ = maximum_;
        } else if (delta <= minimum_ - position_) {
            position_ = minimum_;
        } else {
            position_ = std::clamp(position_ + delta, minimum_, maximum_);
        }
        return Value();
    }

    [[nodiscard]] double
    Value() const noexcept
    {
        const double span =
            position_ < center_ ? center_ - minimum_ : maximum_ - center_;
        return std::clamp((position_ - center_) / span, -1.0, 1.0);
    }

    void
    Reset() noexcept
    {
        position_ = center_;
    }
};
} // namespace PDJE_JUDGE