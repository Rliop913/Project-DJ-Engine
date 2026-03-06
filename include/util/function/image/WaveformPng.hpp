#pragma once

#include "util/function/image/PngWriter.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <future>
#include <limits>
#include <span>
#include <string>
#include <utility>
#include <vector>

namespace PDJE_UTIL::function::image {

using EncodedPngBytes = std::vector<std::uint8_t>;
using ChannelWaveformPngs = std::vector<EncodedPngBytes>;
using WaveformPngBatch = std::vector<ChannelWaveformPngs>;

struct EncodeWaveformPngArgs {
    std::span<const float> pcm;
    std::size_t            channel_count = 0;
    std::size_t            y_pixels = 0;
    std::size_t            pcm_per_pixel = 0;
    std::size_t            x_pixels_per_png = 0;
    int                    compression_level = -1;
};

namespace detail {

inline common::Result<void>
validate_waveform_args(const EncodeWaveformPngArgs &args)
{
    if (args.pcm.data() == nullptr) {
        return common::Result<void>::failure(
            { common::StatusCode::invalid_argument,
              "EncodeWaveformPngArgs.pcm must reference valid PCM data." });
    }

    if (args.pcm.empty()) {
        return common::Result<void>::failure(
            { common::StatusCode::invalid_argument,
              "EncodeWaveformPngArgs.pcm must not be empty." });
    }

    if (args.channel_count == 0) {
        return common::Result<void>::failure(
            { common::StatusCode::invalid_argument,
              "EncodeWaveformPngArgs.channel_count must be greater than zero." });
    }

    if (args.y_pixels == 0) {
        return common::Result<void>::failure(
            { common::StatusCode::invalid_argument,
              "EncodeWaveformPngArgs.y_pixels must be greater than zero." });
    }

    if (args.pcm_per_pixel == 0) {
        return common::Result<void>::failure(
            { common::StatusCode::invalid_argument,
              "EncodeWaveformPngArgs.pcm_per_pixel must be greater than zero." });
    }

    if (args.x_pixels_per_png == 0) {
        return common::Result<void>::failure(
            { common::StatusCode::invalid_argument,
              "EncodeWaveformPngArgs.x_pixels_per_png must be greater than zero." });
    }

    if (args.compression_level < -1 || args.compression_level > 9) {
        return common::Result<void>::failure(
            { common::StatusCode::invalid_argument,
              "EncodeWaveformPngArgs.compression_level must be between -1 and 9." });
    }

    return common::Result<void>::success();
}

inline bool
checked_multiply_waveform(std::size_t lhs,
                          std::size_t rhs,
                          std::size_t &result) noexcept
{
    if (lhs != 0 && rhs > (std::numeric_limits<std::size_t>::max() / lhs)) {
        return false;
    }

    result = lhs * rhs;
    return true;
}

inline common::Result<std::size_t>
compute_chunk_sample_count(const EncodeWaveformPngArgs &args)
{
    std::size_t chunk_sample_count = 0;
    if (!checked_multiply_waveform(
            args.pcm_per_pixel, args.x_pixels_per_png, chunk_sample_count)) {
        return common::Result<std::size_t>::failure(
            { common::StatusCode::invalid_argument,
              "Waveform chunk sample count overflows size_t." });
    }

    return common::Result<std::size_t>::success(chunk_sample_count);
}

inline float
clamp_pcm_sample(float value) noexcept
{
    return std::clamp(value, -1.0f, 1.0f);
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

inline common::Result<std::vector<std::vector<float>>>
split_channels(const EncodeWaveformPngArgs &args)
{
    std::vector<float> padded_pcm(args.pcm.begin(), args.pcm.end());
    const std::size_t  remainder = padded_pcm.size() % args.channel_count;
    if (remainder != 0) {
        const std::size_t missing = args.channel_count - remainder;
        padded_pcm.resize(padded_pcm.size() + missing, 0.0f);
    }

    const std::size_t frame_count = padded_pcm.size() / args.channel_count;
    std::vector<std::vector<float>> channels(
        args.channel_count,
        std::vector<float>(frame_count, 0.0f));

    for (std::size_t frame_index = 0; frame_index < frame_count; ++frame_index) {
        const std::size_t frame_offset = frame_index * args.channel_count;
        for (std::size_t channel_index = 0; channel_index < args.channel_count;
             ++channel_index) {
            channels[channel_index][frame_index] =
                padded_pcm[frame_offset + channel_index];
        }
    }

    return common::Result<std::vector<std::vector<float>>>::success(
        std::move(channels));
}

inline common::Result<ChannelWaveformPngs>
encode_channel_waveforms(std::vector<float> channel_pcm,
                         const EncodeWaveformPngArgs &args,
                         function::EvalOptions options)
{
    const auto chunk_sample_count_result = compute_chunk_sample_count(args);
    if (!chunk_sample_count_result.ok()) {
        return common::Result<ChannelWaveformPngs>::failure(
            chunk_sample_count_result.status());
    }

    const std::size_t chunk_sample_count = chunk_sample_count_result.value();
    const std::size_t chunk_remainder = channel_pcm.size() % chunk_sample_count;
    if (chunk_remainder != 0) {
        const std::size_t missing = chunk_sample_count - chunk_remainder;
        channel_pcm.resize(channel_pcm.size() + missing, 0.0f);
    }

    const std::size_t png_count = channel_pcm.size() / chunk_sample_count;
    ChannelWaveformPngs channel_pngs;
    channel_pngs.reserve(png_count);

    std::size_t row_stride = 0;
    std::size_t image_byte_count = 0;
    if (!checked_multiply_waveform(
            args.x_pixels_per_png, std::size_t { 4 }, row_stride) ||
        !checked_multiply_waveform(row_stride, args.y_pixels, image_byte_count)) {
        return common::Result<ChannelWaveformPngs>::failure(
            { common::StatusCode::invalid_argument,
              "Waveform image size overflows size_t." });
    }

    for (std::size_t png_index = 0; png_index < png_count; ++png_index) {
        std::vector<std::uint8_t> rgba(image_byte_count, 0);
        const std::size_t png_sample_offset = png_index * chunk_sample_count;

        for (std::size_t x = 0; x < args.x_pixels_per_png; ++x) {
            const std::size_t column_sample_offset =
                png_sample_offset + (x * args.pcm_per_pixel);

            float min_sample = 1.0f;
            float max_sample = -1.0f;
            for (std::size_t i = 0; i < args.pcm_per_pixel; ++i) {
                const float sample =
                    clamp_pcm_sample(channel_pcm[column_sample_offset + i]);
                min_sample = std::min(min_sample, sample);
                max_sample = std::max(max_sample, sample);
            }

            std::size_t top_row =
                map_sample_to_row_floor(max_sample, args.y_pixels);
            std::size_t bottom_row =
                map_sample_to_row_ceil(min_sample, args.y_pixels);
            if (top_row > bottom_row) {
                std::swap(top_row, bottom_row);
            }

            for (std::size_t y = top_row; y <= bottom_row; ++y) {
                const std::size_t pixel_offset = (y * row_stride) + (x * 4);
                rgba[pixel_offset + 0] = 255;
                rgba[pixel_offset + 1] = 255;
                rgba[pixel_offset + 2] = 255;
                rgba[pixel_offset + 3] = 255;
            }
        }

        auto encoded = encode_png(
            { .image =
                  {
                      .pixels = rgba,
                      .width = args.x_pixels_per_png,
                      .height = args.y_pixels,
                      .stride = row_stride,
                      .pixel_format = PngPixelFormat::rgba8,
                  },
              .compression_level = args.compression_level },
            options);
        if (!encoded.ok()) {
            return common::Result<ChannelWaveformPngs>::failure(encoded.status());
        }

        channel_pngs.push_back(std::move(encoded).value());
    }

    return common::Result<ChannelWaveformPngs>::success(std::move(channel_pngs));
}

} // namespace detail

inline common::Result<WaveformPngBatch>
encode_waveform_pngs(const EncodeWaveformPngArgs &args,
                     function::EvalOptions options = {})
{
    const auto validation = detail::validate_waveform_args(args);
    if (!validation.ok()) {
        return common::Result<WaveformPngBatch>::failure(validation.status());
    }

    const auto split = detail::split_channels(args);
    if (!split.ok()) {
        return common::Result<WaveformPngBatch>::failure(split.status());
    }

    auto channels = std::move(split).value();
    std::vector<std::future<common::Result<ChannelWaveformPngs>>> futures;
    futures.reserve(channels.size());

    try {
        for (auto &channel_pcm : channels) {
            futures.push_back(std::async(
                std::launch::async,
                [channel_pcm = std::move(channel_pcm), args, options]() mutable {
                    return detail::encode_channel_waveforms(
                        std::move(channel_pcm), args, options);
                }));
        }
    } catch (const std::exception &e) {
        return common::Result<WaveformPngBatch>::failure(
            { common::StatusCode::internal_error,
              std::string("Failed to launch waveform worker: ") + e.what() });
    }

    WaveformPngBatch batch;
    batch.reserve(futures.size());

    for (auto &future : futures) {
        try {
            auto channel_result = future.get();
            if (!channel_result.ok()) {
                return common::Result<WaveformPngBatch>::failure(
                    channel_result.status());
            }
            batch.push_back(std::move(channel_result).value());
        } catch (const std::exception &e) {
            return common::Result<WaveformPngBatch>::failure(
                { common::StatusCode::internal_error,
                  std::string("Waveform worker failed: ") + e.what() });
        }
    }

    return common::Result<WaveformPngBatch>::success(std::move(batch));
}

} // namespace PDJE_UTIL::function::image
