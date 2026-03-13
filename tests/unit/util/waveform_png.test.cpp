#include <doctest/doctest.h>

#include "util/function/image/WaveformPng.hpp"

#include <webp/decode.h>

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

std::uint8_t
alpha_at(const DecodedRgbaImage &image, std::size_t x, std::size_t y)
{
    const std::size_t index = ((y * image.width) + x) * 4 + 3;
    return image.pixels[index];
}

} // namespace

TEST_CASE("encode_waveform_pngs validates required waveform arguments")
{
    const std::vector<float> pcm{ 0.0f, 0.0f };

    auto invalid = PDJE_UTIL::function::image::encode_waveform_pngs(
        { .pcm              = pcm,
          .channel_count    = 0,
          .y_pixels         = 8,
          .pcm_per_pixel    = 1,
          .x_pixels_per_png = 4 });

    CHECK_FALSE(invalid.ok());
    CHECK(invalid.status().code ==
          PDJE_UTIL::common::StatusCode::invalid_argument);
}

TEST_CASE("encode_waveform_pngs splits interleaved channels and keeps "
          "background transparent")
{
    const std::vector<float> interleaved_pcm{ 1.0f, -1.0f, 0.0f, 0.0f, -1.0f };

    auto encoded = PDJE_UTIL::function::image::encode_waveform_pngs(
        { .pcm               = interleaved_pcm,
          .channel_count     = 2,
          .y_pixels          = 5,
          .pcm_per_pixel     = 1,
          .x_pixels_per_png  = 2,
          .compression_level = 1 });

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
    "encode_waveform_pngs aggregates min max ranges within a pixel column")
{
    const std::vector<float> mono_pcm{ 1.0f, -1.0f, 0.0f, 0.0f };

    auto encoded = PDJE_UTIL::function::image::encode_waveform_pngs(
        { .pcm              = mono_pcm,
          .channel_count    = 1,
          .y_pixels         = 5,
          .pcm_per_pixel    = 2,
          .x_pixels_per_png = 2 });

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
    "encode_waveform_pngs pads incomplete chunks before waveform reduction")
{
    const std::vector<float> mono_pcm{ 0.5f, -0.5f, 1.0f };

    auto encoded = PDJE_UTIL::function::image::encode_waveform_pngs(
        { .pcm               = mono_pcm,
          .channel_count     = 1,
          .y_pixels          = 5,
          .pcm_per_pixel     = 3,
          .x_pixels_per_png  = 2,
          .compression_level = 1 });

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

TEST_CASE("encode_waveform_pngs keeps identical output across worker counts")
{
    const std::vector<float> interleaved_pcm{ 1.0f,  -1.0f, 0.25f, -0.25f,
                                              -0.5f, 0.5f,  0.75f, -0.75f,
                                              0.0f,  0.0f,  -1.0f, 1.0f,
                                              0.3f,  -0.3f, -0.2f, 0.2f };

    auto single_worker = PDJE_UTIL::function::image::encode_waveform_pngs(
        { .pcm                 = interleaved_pcm,
          .channel_count       = 2,
          .y_pixels            = 9,
          .pcm_per_pixel       = 2,
          .x_pixels_per_png    = 2,
          .compression_level   = 1,
          .worker_thread_count = 1 });

    auto multi_worker = PDJE_UTIL::function::image::encode_waveform_pngs(
        { .pcm                 = interleaved_pcm,
          .channel_count       = 2,
          .y_pixels            = 9,
          .pcm_per_pixel       = 2,
          .x_pixels_per_png    = 2,
          .compression_level   = 1,
          .worker_thread_count = 4 });

    auto auto_worker = PDJE_UTIL::function::image::encode_waveform_pngs(
        { .pcm                 = interleaved_pcm,
          .channel_count       = 2,
          .y_pixels            = 9,
          .pcm_per_pixel       = 2,
          .x_pixels_per_png    = 2,
          .compression_level   = 1,
          .worker_thread_count = 0 });

    REQUIRE(single_worker.ok());
    REQUIRE(multi_worker.ok());
    REQUIRE(auto_worker.ok());

    CHECK(single_worker.value() == multi_worker.value());
    CHECK(single_worker.value() == auto_worker.value());
}
