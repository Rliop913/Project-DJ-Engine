#include <doctest/doctest.h>

#include "util/function/image/PngWriter.hpp"

#include <webp/decode.h>

#include <array>
#include <chrono>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <memory>
#include <span>
#include <string>
#include <vector>

namespace fs = std::filesystem;

namespace {

constexpr std::array<char, 4> kRiffSignature{ 'R', 'I', 'F', 'F' };
constexpr std::array<char, 4> kWebPSignature{ 'W', 'E', 'B', 'P' };

struct DecodedRgbaImage {
    std::vector<std::uint8_t> pixels;
    std::uint32_t             width  = 0;
    std::uint32_t             height = 0;
};

struct ScopedFileCleanup {
    fs::path path;

    ~ScopedFileCleanup()
    {
        std::error_code ec;
        fs::remove(path, ec);
    }
};

bool
has_webp_signature(std::span<const std::uint8_t> bytes)
{
    if (bytes.size() < 12) {
        return false;
    }

    return std::equal(kRiffSignature.begin(),
                      kRiffSignature.end(),
                      reinterpret_cast<const char *>(bytes.data())) &&
           std::equal(kWebPSignature.begin(),
                      kWebPSignature.end(),
                      reinterpret_cast<const char *>(bytes.data() + 8));
}

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

fs::path
make_temp_png_path()
{
    const auto stamp =
        std::chrono::steady_clock::now().time_since_epoch().count();
    return fs::temp_directory_path() /
           ("pdje_webp_writer_" + std::to_string(stamp) + ".webp");
}

} // namespace

TEST_CASE("encode_png produces a valid WebP from padded RGBA rows")
{
    const std::vector<std::uint8_t> padded_rgba_pixels{
        255, 0, 0,   255, 0,   255, 0,   255, 9, 9, 9, 9,
        0,   0, 255, 255, 255, 255, 255, 255, 7, 7, 7, 7
    };
    const std::vector<std::uint8_t> expected_rgba_pixels{
        255, 0, 0, 255, 0, 255, 0, 255, 0, 0, 255, 255, 255, 255, 255, 255
    };

    auto encoded = PDJE_UTIL::function::image::encode_png(
        { .image =
              {
                  .pixels = padded_rgba_pixels,
                  .width = 2,
                  .height = 2,
                  .stride = 12,
                  .pixel_format = PDJE_UTIL::function::image::PngPixelFormat::rgba8,
              },
          .compression_level = 3 });

    REQUIRE(encoded.ok());
    REQUIRE(has_webp_signature(encoded.value()));

    auto decoded = decode_rgba8(encoded.value());
    CHECK(decoded.width == 2);
    CHECK(decoded.height == 2);
    CHECK(decoded.pixels == expected_rgba_pixels);
}

TEST_CASE("write_png writes a WebP file to disk")
{
    const std::vector<std::uint8_t> rgba_pixel{ 12, 34, 56, 255 };
    const fs::path                  output_path = make_temp_png_path();
    const ScopedFileCleanup         cleanup{ output_path };

    auto written = PDJE_UTIL::function::image::write_png(
        { .image =
              {
                  .pixels = rgba_pixel,
                  .width = 1,
                  .height = 1,
                  .stride = 0,
                  .pixel_format = PDJE_UTIL::function::image::PngPixelFormat::rgba8,
              },
          .output_path = output_path,
          .compression_level = -1 });

    REQUIRE(written.ok());
    REQUIRE(fs::exists(output_path));

    std::vector<std::uint8_t> file_bytes(
        static_cast<std::size_t>(fs::file_size(output_path)));
    std::ifstream input(output_path, std::ios::binary);
    REQUIRE(input.is_open());
    input.read(reinterpret_cast<char *>(file_bytes.data()),
               static_cast<std::streamsize>(file_bytes.size()));
    const bool read_completed = input.good() || input.eof();
    REQUIRE(read_completed);

    REQUIRE(has_webp_signature(file_bytes));

    auto decoded = decode_rgba8(file_bytes);
    CHECK(decoded.width == 1);
    CHECK(decoded.height == 1);
    CHECK(decoded.pixels == rgba_pixel);
}

TEST_CASE("encode_png validates buffer layout and compression range")
{
    const std::vector<std::uint8_t> too_small_pixels{ 1, 2, 3, 4, 5, 6, 7 };
    auto invalid_layout = PDJE_UTIL::function::image::encode_png(
        { .image = {
              .pixels       = too_small_pixels,
              .width        = 2,
              .height       = 1,
              .stride       = 0,
              .pixel_format = PDJE_UTIL::function::image::PngPixelFormat::rgba8,
          } });

    CHECK_FALSE(invalid_layout.ok());
    CHECK(invalid_layout.status().code ==
          PDJE_UTIL::common::StatusCode::invalid_argument);

    const std::vector<std::uint8_t> valid_pixels{ 1, 2, 3, 255 };
    auto invalid_compression = PDJE_UTIL::function::image::encode_png(
        { .image =
              {
                  .pixels = valid_pixels,
                  .width = 1,
                  .height = 1,
                  .stride = 0,
                  .pixel_format = PDJE_UTIL::function::image::PngPixelFormat::rgba8,
              },
          .compression_level = 12 });

    CHECK_FALSE(invalid_compression.ok());
    CHECK(invalid_compression.status().code ==
          PDJE_UTIL::common::StatusCode::invalid_argument);
}
