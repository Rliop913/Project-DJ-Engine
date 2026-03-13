#pragma once

#include "util/common/Result.hpp"
#include "util/function/FunctionContext.hpp"

#include <webp/encode.h>

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <limits>
#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace PDJE_UTIL::function::image {

enum class PngPixelFormat { gray8, gray_alpha8, rgb8, rgba8 };

struct RasterImageView {
    std::span<const std::uint8_t> pixels       = {};
    std::size_t                   width        = 0;
    std::size_t                   height       = 0;
    std::size_t                   stride       = 0;
    PngPixelFormat                pixel_format = PngPixelFormat::rgba8;
};

struct EncodePngArgs {
    RasterImageView image;
    int             compression_level = -1;
};

struct WritePngArgs {
    RasterImageView       image;
    std::filesystem::path output_path;
    int                   compression_level = -1;
};

namespace detail {

struct ImageLayout {
    std::size_t row_bytes        = 0;
    std::size_t effective_stride = 0;
};

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

inline constexpr std::size_t
bytes_per_pixel(PngPixelFormat pixel_format) noexcept
{
    switch (pixel_format) {
    case PngPixelFormat::gray8:
        return 1;
    case PngPixelFormat::gray_alpha8:
        return 2;
    case PngPixelFormat::rgb8:
        return 3;
    case PngPixelFormat::rgba8:
        return 4;
    }

    return 0;
}

inline common::Result<ImageLayout>
validate_image(const RasterImageView &image)
{
    if (image.pixels.data() == nullptr) {
        return common::Result<ImageLayout>::failure(
            { common::StatusCode::invalid_argument,
              "RasterImageView.pixels must reference valid image data." });
    }

    if (image.width == 0 || image.height == 0) {
        return common::Result<ImageLayout>::failure(
            { common::StatusCode::invalid_argument,
              "RasterImageView width and height must be greater than zero." });
    }

    if (image.width > 16383 || image.height > 16383) {
        return common::Result<ImageLayout>::failure(
            { common::StatusCode::invalid_argument,
              "RasterImageView dimensions must fit within WebP limits." });
    }

    ImageLayout layout;
    if (!checked_multiply(image.width,
                          bytes_per_pixel(image.pixel_format),
                          layout.row_bytes)) {
        return common::Result<ImageLayout>::failure(
            { common::StatusCode::invalid_argument,
              "RasterImageView row size overflows size_t." });
    }

    layout.effective_stride =
        image.stride == 0 ? layout.row_bytes : image.stride;
    if (layout.effective_stride < layout.row_bytes) {
        return common::Result<ImageLayout>::failure(
            { common::StatusCode::invalid_argument,
              "RasterImageView.stride must be zero or greater than or equal to "
              "the packed row size." });
    }

    std::size_t required_bytes = layout.row_bytes;
    if (image.height > 1) {
        std::size_t tail_bytes = 0;
        if (!checked_multiply(
                layout.effective_stride, image.height - 1, tail_bytes) ||
            !checked_add(tail_bytes, layout.row_bytes, required_bytes)) {
            return common::Result<ImageLayout>::failure(
                { common::StatusCode::invalid_argument,
                  "RasterImageView buffer size calculation overflows "
                  "size_t." });
        }
    }

    if (image.pixels.size() < required_bytes) {
        return common::Result<ImageLayout>::failure(
            { common::StatusCode::invalid_argument,
              "RasterImageView.pixels is smaller than the specified width, "
              "height, and stride require." });
    }

    return common::Result<ImageLayout>::success(layout);
}

inline common::Result<std::vector<std::uint8_t>>
pack_rgba(const RasterImageView &image, const ImageLayout &layout)
{
    std::size_t packed_size = 0;
    if (!checked_multiply(
            image.width * image.height, std::size_t{ 4 }, packed_size)) {
        return common::Result<std::vector<std::uint8_t>>::failure(
            { common::StatusCode::invalid_argument,
              "RasterImageView packed RGBA buffer size overflows size_t." });
    }

    std::vector<std::uint8_t> rgba(packed_size, 0);
    for (std::size_t y = 0; y < image.height; ++y) {
        const auto *src_row =
            image.pixels.data() + (y * layout.effective_stride);
        auto *dst_row = rgba.data() + ((y * image.width) * 4);

        for (std::size_t x = 0; x < image.width; ++x) {
            const auto *src =
                src_row + (x * bytes_per_pixel(image.pixel_format));
            auto *dst = dst_row + (x * 4);

            switch (image.pixel_format) {
            case PngPixelFormat::gray8:
                dst[0] = src[0];
                dst[1] = src[0];
                dst[2] = src[0];
                dst[3] = 255;
                break;
            case PngPixelFormat::gray_alpha8:
                dst[0] = src[0];
                dst[1] = src[0];
                dst[2] = src[0];
                dst[3] = src[1];
                break;
            case PngPixelFormat::rgb8:
                dst[0] = src[0];
                dst[1] = src[1];
                dst[2] = src[2];
                dst[3] = 255;
                break;
            case PngPixelFormat::rgba8:
                dst[0] = src[0];
                dst[1] = src[1];
                dst[2] = src[2];
                dst[3] = src[3];
                break;
            }
        }
    }

    return common::Result<std::vector<std::uint8_t>>::success(std::move(rgba));
}

} // namespace detail

inline common::Result<std::vector<std::uint8_t>>
encode_png(const EncodePngArgs &args, function::EvalOptions options = {})
{
    (void)options;

    if (args.compression_level < -1 || args.compression_level > 9) {
        return common::Result<std::vector<std::uint8_t>>::failure(
            { common::StatusCode::invalid_argument,
              "EncodePngArgs.compression_level must be between -1 and 9." });
    }

    auto layout = detail::validate_image(args.image);
    if (!layout.ok()) {
        return common::Result<std::vector<std::uint8_t>>::failure(
            layout.status());
    }

    auto packed_rgba = detail::pack_rgba(args.image, layout.value());
    if (!packed_rgba.ok()) {
        return common::Result<std::vector<std::uint8_t>>::failure(
            packed_rgba.status());
    }

    std::uint8_t *encoded_bytes = nullptr;
    const auto    encoded_size =
        WebPEncodeLosslessRGBA(packed_rgba.value().data(),
                               static_cast<int>(args.image.width),
                               static_cast<int>(args.image.height),
                               static_cast<int>(args.image.width * 4),
                               &encoded_bytes);
    if (encoded_size == 0 || encoded_bytes == nullptr) {
        return common::Result<std::vector<std::uint8_t>>::failure(
            { common::StatusCode::internal_error,
              "WebPEncodeLosslessRGBA() failed while encoding the image." });
    }

    std::vector<std::uint8_t> bytes(encoded_size);
    std::memcpy(bytes.data(), encoded_bytes, encoded_size);
    WebPFree(encoded_bytes);

    return common::Result<std::vector<std::uint8_t>>::success(std::move(bytes));
}

inline common::Result<void>
write_png(const WritePngArgs &args, function::EvalOptions options = {})
{
    (void)options;

    if (args.output_path.empty()) {
        return common::Result<void>::failure(
            { common::StatusCode::invalid_argument,
              "WritePngArgs.output_path must not be empty." });
    }

    auto encoded = encode_png(
        { .image = args.image, .compression_level = args.compression_level });
    if (!encoded.ok()) {
        return common::Result<void>::failure(encoded.status());
    }

    std::ofstream output(args.output_path, std::ios::binary | std::ios::trunc);
    if (!output.is_open()) {
        return common::Result<void>::failure(
            { common::StatusCode::io_error,
              "Failed to open the WebP output path for writing." });
    }

    const auto &bytes = encoded.value();
    output.write(reinterpret_cast<const char *>(bytes.data()),
                 static_cast<std::streamsize>(bytes.size()));
    if (!output.good()) {
        return common::Result<void>::failure(
            { common::StatusCode::io_error,
              "Failed while writing WebP bytes to the output path." });
    }

    return common::Result<void>::success();
}

} // namespace PDJE_UTIL::function::image
