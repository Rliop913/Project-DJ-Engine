#include <doctest/doctest.h>

#include "util/function/image/WaveformWebp.hpp"
#include "util/function/image/detail/WaveformWebpSupport.hpp"

#include <webp/decode.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <memory>
#include <span>
#include <vector>

namespace {

struct DecodedRgbaImage {
    std::vector<std::uint8_t> pixels;
    std::uint32_t             width  = 0;
    std::uint32_t             height = 0;
};

DecodedRgbaImage
decode_rgba8(std::span<const std::uint8_t> webp_bytes)
{
    int width  = 0;
    int height = 0;
    REQUIRE(WebPGetInfo(
                webp_bytes.data(), webp_bytes.size(), &width, &height) == 1);

    std::unique_ptr<std::uint8_t, decltype(&WebPFree)> decoded_bytes(
        WebPDecodeRGBA(webp_bytes.data(), webp_bytes.size(), &width, &height),
        &WebPFree);
    REQUIRE(decoded_bytes != nullptr);

    DecodedRgbaImage decoded;
    decoded.width  = static_cast<std::uint32_t>(width);
    decoded.height = static_cast<std::uint32_t>(height);
    decoded.pixels.assign(decoded_bytes.get(),
                          decoded_bytes.get() +
                              (static_cast<std::size_t>(width * height) * 4));
    return decoded;
}

std::size_t
rgba_offset(const DecodedRgbaImage &image, std::size_t x, std::size_t y)
{
    return ((y * image.width) + x) * 4;
}

std::uint8_t
channel_at(const DecodedRgbaImage &image,
           std::size_t             x,
           std::size_t             y,
           std::size_t             channel)
{
    return image.pixels[rgba_offset(image, x, y) + channel];
}

std::array<std::uint8_t, 3>
rgb_at(const DecodedRgbaImage &image, std::size_t x, std::size_t y)
{
    return {
        channel_at(image, x, y, 0),
        channel_at(image, x, y, 1),
        channel_at(image, x, y, 2),
    };
}

std::uint8_t
alpha_at(const DecodedRgbaImage &image, std::size_t x, std::size_t y)
{
    return channel_at(image, x, y, 3);
}

std::size_t
find_first_opaque_row(const DecodedRgbaImage &image, std::size_t x)
{
    for (std::size_t y = 0; y < image.height; ++y) {
        if (alpha_at(image, x, y) == 255) {
            return y;
        }
    }

    return image.height;
}

std::uint8_t
unit_float_to_byte(float value)
{
    if (!std::isfinite(value)) {
        value = 0.0f;
    }

    const float clamped = std::clamp(value, 0.0f, 1.0f);
    return static_cast<std::uint8_t>(
        std::lround(static_cast<double>(clamped) * 255.0));
}

std::size_t
map_column_to_stft_frame(const std::size_t x,
                         const std::size_t frame_count,
                         const std::size_t x_pixels_per_image)
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

std::vector<float>
build_color_pcm(const std::size_t sample_count)
{
    std::vector<float> pcm(sample_count, 0.0f);

    for (std::size_t idx = 0; idx < pcm.size(); ++idx) {
        const float x = static_cast<float>(idx);
        pcm[idx]      = (0.65f * std::sin(x * 0.17f)) +
                   (0.2f * std::cos(x * 0.037f)) +
                   (0.1f * std::sin(x * 0.049f));
    }

    return pcm;
}

} // namespace

TEST_CASE("waveform support maps rightmost column to final stft frame")
{
    using PDJE_UTIL::function::image::detail::support::map_column_to_stft_frame;

    CHECK(map_column_to_stft_frame(0, 3, 2) == 0u);
    CHECK(map_column_to_stft_frame(1, 3, 2) == 2u);
    CHECK(map_column_to_stft_frame(7, 11, 8) == 10u);
    CHECK(map_column_to_stft_frame(0, 11, 1) == 0u);
}

TEST_CASE("encode_waveform_webps validates required waveform arguments")
{
    const std::vector<float> pcm{ 0.0f, 0.0f };

    auto invalid = PDJE_UTIL::function::image::encode_waveform_webps(
        { .pcm                = pcm,
          .channel_count      = 0,
          .y_pixels           = 8,
          .pcm_per_pixel      = 1,
          .x_pixels_per_image = 4 });

    CHECK_FALSE(invalid.ok());
    CHECK(invalid.status().code ==
          PDJE_UTIL::common::StatusCode::invalid_argument);
}

TEST_CASE("encode_waveform_webps splits interleaved channels and keeps "
          "background transparent")
{
    const std::vector<float> interleaved_pcm{ 1.0f, -1.0f, 0.0f, 0.0f, -1.0f };

    auto encoded = PDJE_UTIL::function::image::encode_waveform_webps(
        { .pcm                = interleaved_pcm,
          .channel_count      = 2,
          .y_pixels           = 5,
          .pcm_per_pixel      = 1,
          .x_pixels_per_image = 2,
          .compression_level  = 1 });

    REQUIRE(encoded.ok());
    REQUIRE(encoded.value().size() == 2);
    REQUIRE(encoded.value()[0].size() == 2);
    REQUIRE(encoded.value()[1].size() == 2);

    auto left_first = decode_rgba8(encoded.value()[0][0]);
    CHECK(left_first.width == 2);
    CHECK(left_first.height == 5);
    CHECK(alpha_at(left_first, 0, 0) == 255);
    CHECK(alpha_at(left_first, 0, 1) == 0);
    CHECK(alpha_at(left_first, 1, 2) == 255);
    CHECK(alpha_at(left_first, 1, 0) == 0);

    auto right_first = decode_rgba8(encoded.value()[1][0]);
    CHECK(alpha_at(right_first, 0, 4) == 255);
    CHECK(alpha_at(right_first, 0, 0) == 0);
}

TEST_CASE(
    "encode_waveform_webps aggregates min max ranges within a pixel column")
{
    const std::vector<float> mono_pcm{ 1.0f, -1.0f, 0.0f, 0.0f };

    auto encoded = PDJE_UTIL::function::image::encode_waveform_webps(
        { .pcm                = mono_pcm,
          .channel_count      = 1,
          .y_pixels           = 5,
          .pcm_per_pixel      = 2,
          .x_pixels_per_image = 2 });

    REQUIRE(encoded.ok());
    REQUIRE(encoded.value().size() == 1);
    REQUIRE(encoded.value()[0].size() == 1);

    auto decoded = decode_rgba8(encoded.value()[0][0]);
    CHECK(decoded.width == 2);
    CHECK(decoded.height == 5);

    for (std::size_t y = 0; y < decoded.height; ++y) {
        CHECK(alpha_at(decoded, 0, y) == 255);
    }
    CHECK(alpha_at(decoded, 1, 2) == 255);
    CHECK(alpha_at(decoded, 1, 1) == 0);
    CHECK(alpha_at(decoded, 1, 3) == 0);
}

TEST_CASE(
    "encode_waveform_webps pads incomplete chunks before waveform reduction")
{
    const std::vector<float> mono_pcm{ 0.5f, -0.5f, 1.0f };

    auto encoded = PDJE_UTIL::function::image::encode_waveform_webps(
        { .pcm                = mono_pcm,
          .channel_count      = 1,
          .y_pixels           = 5,
          .pcm_per_pixel      = 3,
          .x_pixels_per_image = 2,
          .compression_level  = 1 });

    REQUIRE(encoded.ok());
    REQUIRE(encoded.value().size() == 1);
    REQUIRE(encoded.value()[0].size() == 1);

    auto decoded = decode_rgba8(encoded.value()[0][0]);
    CHECK(decoded.width == 2);
    CHECK(decoded.height == 5);

    CHECK(alpha_at(decoded, 0, 0) == 255);
    CHECK(alpha_at(decoded, 0, 1) == 255);
    CHECK(alpha_at(decoded, 0, 2) == 255);
    CHECK(alpha_at(decoded, 0, 3) == 255);
    CHECK(alpha_at(decoded, 0, 4) == 0);

    CHECK(alpha_at(decoded, 1, 2) == 255);
    CHECK(alpha_at(decoded, 1, 1) == 0);
    CHECK(alpha_at(decoded, 1, 3) == 0);
}

TEST_CASE("encode_waveform_webps keeps identical output across worker counts")
{
    const std::vector<float> interleaved_pcm{ 1.0f,  -1.0f, 0.25f, -0.25f,
                                              -0.5f, 0.5f,  0.75f, -0.75f,
                                              0.0f,  0.0f,  -1.0f, 1.0f,
                                              0.3f,  -0.3f, -0.2f, 0.2f };

    auto single_worker = PDJE_UTIL::function::image::encode_waveform_webps(
        { .pcm                 = interleaved_pcm,
          .channel_count       = 2,
          .y_pixels            = 9,
          .pcm_per_pixel       = 2,
          .x_pixels_per_image  = 2,
          .compression_level   = 1,
          .worker_thread_count = 1 });

    auto multi_worker = PDJE_UTIL::function::image::encode_waveform_webps(
        { .pcm                 = interleaved_pcm,
          .channel_count       = 2,
          .y_pixels            = 9,
          .pcm_per_pixel       = 2,
          .x_pixels_per_image  = 2,
          .compression_level   = 1,
          .worker_thread_count = 4 });

    auto auto_worker = PDJE_UTIL::function::image::encode_waveform_webps(
        { .pcm                 = interleaved_pcm,
          .channel_count       = 2,
          .y_pixels            = 9,
          .pcm_per_pixel       = 2,
          .x_pixels_per_image  = 2,
          .compression_level   = 1,
          .worker_thread_count = 0 });

    REQUIRE(single_worker.ok());
    REQUIRE(multi_worker.ok());
    REQUIRE(auto_worker.ok());

    CHECK(single_worker.value() == multi_worker.value());
    CHECK(single_worker.value() == auto_worker.value());
}

TEST_CASE("encode_waveform_webps rgb overload validates stft arguments")
{
    const std::vector<float> pcm{ 0.0f, 0.1f, -0.1f, 0.0f };

    auto invalid_window = PDJE_UTIL::function::image::encode_waveform_webps(
        { .pcm                = pcm,
          .channel_count      = 1,
          .y_pixels           = 8,
          .pcm_per_pixel      = 1,
          .x_pixels_per_image = 4 },
        { .window_size_exp = 5 });

    auto invalid_overlap = PDJE_UTIL::function::image::encode_waveform_webps(
        { .pcm                = pcm,
          .channel_count      = 1,
          .y_pixels           = 8,
          .pcm_per_pixel      = 1,
          .x_pixels_per_image = 4 },
        { .overlap_ratio = 1.0f });

    CHECK_FALSE(invalid_window.ok());
    CHECK(invalid_window.status().code ==
          PDJE_UTIL::common::StatusCode::invalid_argument);

    CHECK_FALSE(invalid_overlap.ok());
    CHECK(invalid_overlap.status().code ==
          PDJE_UTIL::common::StatusCode::invalid_argument);
}

TEST_CASE("encode_waveform_webps rgb overload keeps batch shape and padding")
{
    const std::vector<float> interleaved_pcm{ 1.0f, -1.0f, 0.0f, 0.0f, -1.0f };

    auto encoded = PDJE_UTIL::function::image::encode_waveform_webps(
        { .pcm                = interleaved_pcm,
          .channel_count      = 2,
          .y_pixels           = 5,
          .pcm_per_pixel      = 1,
          .x_pixels_per_image = 2,
          .compression_level  = 1 },
        { .window_size_exp = 6 });

    REQUIRE(encoded.ok());
    REQUIRE(encoded.value().size() == 2);
    REQUIRE(encoded.value()[0].size() == 2);
    REQUIRE(encoded.value()[1].size() == 2);

    auto decoded = decode_rgba8(encoded.value()[0][0]);
    CHECK(decoded.width == 2);
    CHECK(decoded.height == 5);

    bool saw_opaque      = false;
    bool saw_transparent = false;
    for (std::size_t y = 0; y < decoded.height; ++y) {
        for (std::size_t x = 0; x < decoded.width; ++x) {
            const auto alpha = alpha_at(decoded, x, y);
            CHECK((alpha == 0 || alpha == 255));
            saw_opaque      = saw_opaque || (alpha == 255);
            saw_transparent = saw_transparent || (alpha == 0);
        }
    }

    CHECK(saw_opaque);
    CHECK(saw_transparent);
}

TEST_CASE("encode_waveform_webps rgb overload maps stft rgb onto waveform "
          "foreground")
{
    const auto mono_pcm = build_color_pcm(128);

    const PDJE_UTIL::function::image::EncodeWaveformWebpArgs waveform_args {
        .pcm                = mono_pcm,
        .channel_count      = 1,
        .y_pixels           = 32,
        .pcm_per_pixel      = 16,
        .x_pixels_per_image = 8,
        .compression_level  = 1,
    };

    const PDJE_UTIL::function::image::EncodeWaveformWebpStftArgs stft_args {
        .target_window   = PDJE_PARALLEL::WINDOW_LIST::HANNING,
        .window_size_exp = 6,
        .overlap_ratio   = 0.5f,
    };

    auto encoded = PDJE_UTIL::function::image::encode_waveform_webps(
        waveform_args, stft_args);

    REQUIRE(encoded.ok());
    REQUIRE(encoded.value().size() == 1);
    REQUIRE(encoded.value()[0].size() == 1);

    auto decoded = decode_rgba8(encoded.value()[0][0]);
    CHECK(decoded.width == waveform_args.x_pixels_per_image);
    CHECK(decoded.height == waveform_args.y_pixels);

    auto expected_pcm         = mono_pcm;
    auto expected_postprocess = stft_args.post_process;
    expected_postprocess.to_rgb = true;

    PDJE_PARALLEL::STFT stft;
    const auto [expected_rgb, expected_imag] = stft.calculate(
        expected_pcm,
        stft_args.target_window,
        stft_args.window_size_exp,
        stft_args.overlap_ratio,
        expected_postprocess);

    REQUIRE(expected_imag.empty());
    REQUIRE_FALSE(expected_rgb.empty());
    REQUIRE((expected_rgb.size() % 3u) == 0u);

    const std::size_t frame_count = expected_rgb.size() / 3u;
    CHECK(frame_count != waveform_args.x_pixels_per_image);

    bool saw_opaque      = false;
    bool saw_transparent = false;
    bool saw_non_white   = false;

    for (std::size_t y = 0; y < decoded.height; ++y) {
        for (std::size_t x = 0; x < decoded.width; ++x) {
            const auto alpha = alpha_at(decoded, x, y);
            CHECK((alpha == 0 || alpha == 255));
            saw_opaque      = saw_opaque || (alpha == 255);
            saw_transparent = saw_transparent || (alpha == 0);
        }
    }

    for (std::size_t x = 0; x < decoded.width; ++x) {
        const std::size_t opaque_row = find_first_opaque_row(decoded, x);
        REQUIRE(opaque_row < decoded.height);

        const std::size_t frame_index =
            map_column_to_stft_frame(x, frame_count, waveform_args.x_pixels_per_image);
        const std::array<std::uint8_t, 3> expected_color {
            unit_float_to_byte(expected_rgb[(frame_index * 3u) + 0u]),
            unit_float_to_byte(expected_rgb[(frame_index * 3u) + 1u]),
            unit_float_to_byte(expected_rgb[(frame_index * 3u) + 2u]),
        };

        CHECK(rgb_at(decoded, x, opaque_row) == expected_color);
        saw_non_white =
            saw_non_white || (expected_color != std::array<std::uint8_t, 3> {
                                                   255,
                                                   255,
                                                   255,
                                               });
    }

    CHECK(saw_opaque);
    CHECK(saw_transparent);
    CHECK(saw_non_white);
}

TEST_CASE("encode_waveform_webps rgb overload keeps identical output across "
          "worker counts")
{
    const std::vector<float> interleaved_pcm{ 1.0f,  -1.0f, 0.25f, -0.25f,
                                              -0.5f, 0.5f,  0.75f, -0.75f,
                                              0.0f,  0.0f,  -1.0f, 1.0f,
                                              0.3f,  -0.3f, -0.2f, 0.2f };
    const PDJE_UTIL::function::image::EncodeWaveformWebpStftArgs stft_args {
        .window_size_exp = 6,
        .overlap_ratio   = 0.5f,
    };

    auto single_worker = PDJE_UTIL::function::image::encode_waveform_webps(
        { .pcm                 = interleaved_pcm,
          .channel_count       = 2,
          .y_pixels            = 9,
          .pcm_per_pixel       = 2,
          .x_pixels_per_image  = 2,
          .compression_level   = 1,
          .worker_thread_count = 1 },
        stft_args);

    auto multi_worker = PDJE_UTIL::function::image::encode_waveform_webps(
        { .pcm                 = interleaved_pcm,
          .channel_count       = 2,
          .y_pixels            = 9,
          .pcm_per_pixel       = 2,
          .x_pixels_per_image  = 2,
          .compression_level   = 1,
          .worker_thread_count = 4 },
        stft_args);

    auto auto_worker = PDJE_UTIL::function::image::encode_waveform_webps(
        { .pcm                 = interleaved_pcm,
          .channel_count       = 2,
          .y_pixels            = 9,
          .pcm_per_pixel       = 2,
          .x_pixels_per_image  = 2,
          .compression_level   = 1,
          .worker_thread_count = 0 },
        stft_args);

    REQUIRE(single_worker.ok());
    REQUIRE(multi_worker.ok());
    REQUIRE(auto_worker.ok());

    CHECK(single_worker.value() == multi_worker.value());
    CHECK(single_worker.value() == auto_worker.value());
}
