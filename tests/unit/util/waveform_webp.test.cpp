#include <doctest/doctest.h>

#include "util/function/image/WaveformWebp.hpp"
#include "util/function/image/detail/WaveformWebpSupport.hpp"

#include <webp/decode.h>

#include <cmath>
#include <cstdint>
#include <stdexcept>
#include <vector>

namespace {

struct DecodedImage {
    int                       width  = 0;
    int                       height = 0;
    std::vector<std::uint8_t> pixels;
};

DecodedImage
decode(const std::vector<std::uint8_t> &bytes)
{
    DecodedImage output;
    REQUIRE(WebPGetInfo(
                bytes.data(), bytes.size(), &output.width, &output.height) !=
            0);
    std::uint8_t *raw = WebPDecodeRGBA(
        bytes.data(), bytes.size(), &output.width, &output.height);
    REQUIRE(raw != nullptr);
    output.pixels.assign(raw, raw + output.width * output.height * 4);
    WebPFree(raw);
    return output;
}

std::uint8_t
alpha_at(const DecodedImage &image, std::size_t x, std::size_t y)
{
    return image
        .pixels[(y * static_cast<std::size_t>(image.width) + x) * 4 + 3];
}

std::vector<float>
color_pcm(std::size_t count)
{
    std::vector<float> output(count);
    for (std::size_t index = 0; index < count; ++index) {
        output[index] = static_cast<float>(
            0.65 * std::sin(static_cast<double>(index) * 0.23) +
            0.25 * std::sin(static_cast<double>(index) * 0.61));
    }
    return output;
}

PDJE_UTIL::function::image::EncodeWaveformWebpArgs
args_for(std::span<const float> pcm, std::size_t workers)
{
    return {
        .pcm                 = pcm,
        .channel_count       = 1,
        .y_pixels            = 16,
        .pcm_per_pixel       = 8,
        .x_pixels_per_image  = 8,
        .compression_level   = 1,
        .worker_thread_count = workers,
    };
}

} // namespace

TEST_CASE("waveform support maps final column to final STFT frame")
{
    using PDJE_UTIL::function::image::detail::support::map_column_to_stft_frame;
    CHECK(map_column_to_stft_frame(0, 11, 8) == 0);
    CHECK(map_column_to_stft_frame(7, 11, 8) == 10);
}

TEST_CASE("waveform encoder validates required arguments with exceptions")
{
    const std::vector<float> pcm{ 0.0f, 0.0f };
    CHECK_THROWS_AS(PDJE_UTIL::function::image::encode_waveform_webps({
                        .pcm                = pcm,
                        .channel_count      = 0,
                        .y_pixels           = 8,
                        .pcm_per_pixel      = 1,
                        .x_pixels_per_image = 4,
                    }),
                    std::invalid_argument);
}

TEST_CASE(
    "waveform encoder splits channels and preserves transparent background")
{
    const std::vector<float> pcm{ 1.0f, -1.0f, 0.0f, 0.0f, -1.0f };
    const auto batch = PDJE_UTIL::function::image::encode_waveform_webps({
        .pcm                = pcm,
        .channel_count      = 2,
        .y_pixels           = 5,
        .pcm_per_pixel      = 1,
        .x_pixels_per_image = 2,
        .compression_level  = 1,
    });
    REQUIRE(batch.size() == 2);
    REQUIRE(batch[0].size() == 2);
    const auto left  = decode(batch[0][0]);
    const auto right = decode(batch[1][0]);
    CHECK(alpha_at(left, 0, 0) == 255);
    CHECK(alpha_at(left, 0, 1) == 0);
    CHECK(alpha_at(right, 0, 4) == 255);
    CHECK(alpha_at(right, 0, 0) == 0);
}

TEST_CASE("waveform output is deterministic across worker counts")
{
    const auto pcm = color_pcm(128);
    const auto single =
        PDJE_UTIL::function::image::encode_waveform_webps(args_for(pcm, 1));
    const auto parallel =
        PDJE_UTIL::function::image::encode_waveform_webps(args_for(pcm, 4));
    const auto automatic =
        PDJE_UTIL::function::image::encode_waveform_webps(args_for(pcm, 0));
    CHECK(single == parallel);
    CHECK(single == automatic);
}

TEST_CASE("waveform RGB overload validates STFT configuration")
{
    const auto pcm      = color_pcm(64);
    const auto waveform = args_for(pcm, 1);
    CHECK_THROWS_AS(PDJE_UTIL::function::image::encode_waveform_webps(
                        waveform, { .window_size_exp = 5 }),
                    std::invalid_argument);
    CHECK_THROWS_AS(PDJE_UTIL::function::image::encode_waveform_webps(
                        waveform, { .overlap_ratio = 1.0f }),
                    std::invalid_argument);
    CHECK_THROWS_AS(
        PDJE_UTIL::function::image::encode_waveform_webps(
            waveform,
            { .overlap_ratio = std::numeric_limits<float>::quiet_NaN() }),
        std::invalid_argument);
    CHECK_THROWS_AS(
        PDJE_UTIL::function::image::encode_waveform_webps(
            waveform,
            { .overlap_ratio = std::numeric_limits<float>::infinity() }),
        std::invalid_argument);
    CHECK_THROWS_AS(PDJE_UTIL::function::image::encode_waveform_webps(
                        waveform,
                        {
                            .window_size_exp = 6,
                            .mel_filter_bank =
                                PDJE_PARALLEL::MelFilterBankSpec{
                                    .sample_rate = 44100,
                                    .n_fft       = 128,
                                    .n_mels      = 24,
                                    .f_min       = 40.0f,
                                    .f_max       = 8000.0f,
                                },
                        }),
                    std::invalid_argument);
}

TEST_CASE("waveform RGB output honors custom mel bank and worker count")
{
    const auto pcm = color_pcm(128);
    const PDJE_UTIL::function::image::EncodeWaveformWebpStftArgs stft{
        .window_size_exp = 6,
        .overlap_ratio   = 0.5f,
        .mel_filter_bank =
            PDJE_PARALLEL::MelFilterBankSpec{
                .sample_rate = 44100,
                .n_fft       = 64,
                .n_mels      = 24,
                .f_min       = 40.0f,
                .f_max       = 9000.0f,
                .mel_formula = PDJE_PARALLEL::MelFormula::HTK,
                .norm        = PDJE_PARALLEL::MelNorm::Peak,
            },
    };
    const auto single = PDJE_UTIL::function::image::encode_waveform_webps(
        args_for(pcm, 1), stft);
    const auto parallel = PDJE_UTIL::function::image::encode_waveform_webps(
        args_for(pcm, 4), stft);
    CHECK(single == parallel);
    REQUIRE(single.size() == 1);
    REQUIRE(single[0].size() == 2);
    const auto image           = decode(single[0][0]);
    bool       saw_opaque      = false;
    bool       saw_transparent = false;
    for (std::size_t y = 0; y < static_cast<std::size_t>(image.height); ++y) {
        for (std::size_t x = 0; x < static_cast<std::size_t>(image.width);
             ++x) {
            saw_opaque |= alpha_at(image, x, y) == 255;
            saw_transparent |= alpha_at(image, x, y) == 0;
        }
    }
    CHECK(saw_opaque);
    CHECK(saw_transparent);
}
