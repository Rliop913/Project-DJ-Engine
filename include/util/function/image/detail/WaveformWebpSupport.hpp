#pragma once

#include "util/common/Status.hpp"
#include "util/function/image/detail/WaveformWebpInternal.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <string>

namespace PDJE_UTIL::function::image::detail::support {

inline bool
checked_multiply(std::size_t lhs, std::size_t rhs, std::size_t &result) noexcept
{
    if (lhs != 0 && rhs > (std::numeric_limits<std::size_t>::max() / lhs)) {
        return false;
    }

    result = lhs * rhs;
    return true;
}

inline bool
checked_add(std::size_t lhs, std::size_t rhs, std::size_t &result) noexcept
{
    if (rhs > (std::numeric_limits<std::size_t>::max() - lhs)) {
        return false;
    }

    result = lhs + rhs;
    return true;
}

inline common::Status
wrap_job_status(const common::Status &status, const WaveformJob &job)
{
    return { status.code,
             "Waveform job channel " + std::to_string(job.channel_index) +
                 " image " + std::to_string(job.image_index) + ": " +
                 status.message };
}

inline float
clamp_pcm_sample(float value) noexcept
{
    return std::clamp(value, -1.0f, 1.0f);
}

inline float
clamp_unit_float(float value) noexcept
{
    if (!std::isfinite(value)) {
        return 0.0f;
    }

    return std::clamp(value, 0.0f, 1.0f);
}

inline std::uint8_t
unit_float_to_byte(float value) noexcept
{
    const double scaled =
        static_cast<double>(clamp_unit_float(value)) * 255.0;
    return static_cast<std::uint8_t>(std::lround(scaled));
}

inline std::size_t
map_sample_to_row_floor(float value, std::size_t y_pixels) noexcept
{
    if (y_pixels <= 1) {
        return 0;
    }

    const double clamped = static_cast<double>(clamp_pcm_sample(value));
    const double normalized_row =
        ((1.0 - clamped) * 0.5) * static_cast<double>(y_pixels - 1);
    const auto row = static_cast<std::size_t>(std::floor(normalized_row));
    return std::min(row, y_pixels - 1);
}

inline std::size_t
map_sample_to_row_ceil(float value, std::size_t y_pixels) noexcept
{
    if (y_pixels <= 1) {
        return 0;
    }

    const double clamped = static_cast<double>(clamp_pcm_sample(value));
    const double normalized_row =
        ((1.0 - clamped) * 0.5) * static_cast<double>(y_pixels - 1);
    const auto row = static_cast<std::size_t>(std::ceil(normalized_row));
    return std::min(row, y_pixels - 1);
}

inline std::size_t
map_column_to_stft_frame(const std::size_t x,
                         const std::size_t frame_count,
                         const std::size_t x_pixels_per_image) noexcept
{
    if (frame_count <= 1 || x_pixels_per_image <= 1) {
        return 0;
    }

    const long double scaled = static_cast<long double>(x) *
                               static_cast<long double>(frame_count - 1);
    const auto frame_index = static_cast<std::size_t>(
        scaled / static_cast<long double>(x_pixels_per_image - 1));
    return std::min(frame_index, frame_count - 1);
}

} // namespace PDJE_UTIL::function::image::detail::support
