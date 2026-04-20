#include "util/function/image/detail/WaveformWebpProcessor.hpp"

#include "util/function/image/detail/WaveformWebpSupport.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <span>
#include <utility>
#include <vector>

namespace PDJE_UTIL::function::image::detail {

StftColorMapper::StftColorMapper(const EncodeWaveformWebpArgs     &args,
                                 const EncodeWaveformWebpStftArgs &stft_args,
                                 const WaveformBufferSizes        &buffer_sizes,
                                 const std::size_t                 chunk_sample_count)
    : args_(args),
      stft_args_(stft_args),
      stft_pcm_(chunk_sample_count, 0.0f),
      column_rgb_(buffer_sizes.column_rgb_byte_count, 0)
{
}

common::Result<void>
StftColorMapper::Prepare(const WaveformJob &job)
{
    stft_pcm_.assign(job.samples, job.samples + job.sample_count);

    auto post_process = stft_args_.post_process;
    post_process.to_rgb = true;

    auto [stft_real, stft_imag] = stft_.calculate(stft_pcm_,
                                                  stft_args_.target_window,
                                                  stft_args_.window_size_exp,
                                                  stft_args_.overlap_ratio,
                                                  post_process);

    if (!stft_imag.empty()) {
        return common::Result<void>::failure(support::wrap_job_status(
            { common::StatusCode::internal_error,
              "STFT RGB output unexpectedly contained imaginary data." },
            job));
    }

    auto mapped_rgb = MapStftRgbToColumns(
        std::span<const float>(stft_real.data(), stft_real.size()));
    if (!mapped_rgb.ok()) {
        return common::Result<void>::failure(
            support::wrap_job_status(mapped_rgb.status(), job));
    }

    return common::Result<void>::success();
}

std::array<std::uint8_t, 3>
StftColorMapper::ColorAt(const std::size_t x) const
{
    const std::size_t color_offset = x * 3u;
    return {
        column_rgb_[color_offset + 0u],
        column_rgb_[color_offset + 1u],
        column_rgb_[color_offset + 2u],
    };
}

common::Result<void>
StftColorMapper::MapStftRgbToColumns(std::span<const float> rgb_frames)
{
    if (rgb_frames.empty()) {
        return common::Result<void>::failure(
            { common::StatusCode::internal_error,
              "STFT RGB output was empty." });
    }

    if ((rgb_frames.size() % 3u) != 0u) {
        return common::Result<void>::failure(
            { common::StatusCode::internal_error,
              "STFT RGB output must be composed of RGB triplets." });
    }

    std::size_t expected_column_rgb_size = 0;
    if (!support::checked_multiply(
            args_.x_pixels_per_image, std::size_t { 3 }, expected_column_rgb_size)) {
        return common::Result<void>::failure(
            { common::StatusCode::internal_error,
              "Waveform column RGB size overflows size_t." });
    }

    if (column_rgb_.size() != expected_column_rgb_size) {
        return common::Result<void>::failure(
            { common::StatusCode::internal_error,
              "Waveform column RGB buffer size does not match "
              "x_pixels_per_image." });
    }

    const std::size_t frame_count = rgb_frames.size() / 3u;
    for (std::size_t x = 0; x < args_.x_pixels_per_image; ++x) {
        const std::size_t frame_index = support::map_column_to_stft_frame(
            x, frame_count, args_.x_pixels_per_image);
        const std::size_t frame_offset  = frame_index * 3u;
        const std::size_t column_offset = x * 3u;

        column_rgb_[column_offset + 0u] =
            support::unit_float_to_byte(rgb_frames[frame_offset + 0u]);
        column_rgb_[column_offset + 1u] =
            support::unit_float_to_byte(rgb_frames[frame_offset + 1u]);
        column_rgb_[column_offset + 2u] =
            support::unit_float_to_byte(rgb_frames[frame_offset + 2u]);
    }

    return common::Result<void>::success();
}

} // namespace PDJE_UTIL::function::image::detail
