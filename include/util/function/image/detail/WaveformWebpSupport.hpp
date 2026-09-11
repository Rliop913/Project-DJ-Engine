#pragma once

#include "util/function/image/detail/WaveformWebpInternal.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <stdexcept>
#include <string>

namespace PDJE_UTIL::function::image::detail::support {

inline std::size_t
checked_multiply(std::size_t left, std::size_t right)
{
    if (left != 0 && right > std::numeric_limits<std::size_t>::max() / left) {
        throw std::overflow_error(
            "Waveform buffer size calculation overflowed.");
    }
    return left * right;
}

inline std::runtime_error
job_error(const WaveformJob &job, std::string_view message)
{
    return std::runtime_error("Waveform job channel " +
                              std::to_string(job.channel_index) + " image " +
                              std::to_string(job.image_index) + ": " +
                              std::string(message));
}

inline float
clamp_pcm_sample(float value) noexcept
{
    return std::clamp(value, -1.0f, 1.0f);
}

inline float
clamp_unit_float(float value) noexcept
{
    return std::isfinite(value) ? std::clamp(value, 0.0f, 1.0f) : 0.0f;
}

inline std::uint8_t
unit_float_to_byte(float value) noexcept
{
    return static_cast<std::uint8_t>(
        std::lround(static_cast<double>(clamp_unit_float(value)) * 255.0));
}

inline std::size_t
map_sample_to_row_floor(float value, std::size_t height) noexcept
{
    if (height <= 1)
        return 0;
    const auto normalized = ((1.0 - clamp_pcm_sample(value)) * 0.5) *
                            static_cast<double>(height - 1);
    return std::min(static_cast<std::size_t>(std::floor(normalized)),
                    height - 1);
}

inline std::size_t
map_sample_to_row_ceil(float value, std::size_t height) noexcept
{
    if (height <= 1)
        return 0;
    const auto normalized = ((1.0 - clamp_pcm_sample(value)) * 0.5) *
                            static_cast<double>(height - 1);
    return std::min(static_cast<std::size_t>(std::ceil(normalized)),
                    height - 1);
}

inline std::size_t
map_column_to_stft_frame(std::size_t column,
                         std::size_t frame_count,
                         std::size_t width) noexcept
{
    if (frame_count <= 1 || width <= 1)
        return 0;
    const long double scaled = static_cast<long double>(column) *
                               static_cast<long double>(frame_count - 1);
    return std::min(
        static_cast<std::size_t>(scaled / static_cast<long double>(width - 1)),
        frame_count - 1);
}

} // namespace PDJE_UTIL::function::image::detail::support
