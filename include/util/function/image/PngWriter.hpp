#pragma once

#include "util/common/Result.hpp"
#include "util/function/FunctionContext.hpp"

#include <spng.h>

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <limits>
#include <memory>
#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace PDJE_UTIL::function::image {

enum class PngPixelFormat {
    gray8,
    gray_alpha8,
    rgb8,
    rgba8
};

struct RasterImageView {
    std::span<const std::uint8_t> pixels = {};
    std::size_t                   width  = 0;
    std::size_t                   height = 0;
    std::size_t                   stride = 0;
    PngPixelFormat                pixel_format = PngPixelFormat::rgba8;
};

struct EncodePngArgs {
    RasterImageView image;
    int             compression_level = -1;
};

struct WritePngArgs {
    RasterImageView      image;
    std::filesystem::path output_path;
    int                  compression_level = -1;
};

namespace detail {

struct ImageLayout {
    std::size_t row_bytes      = 0;
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

inline constexpr int
spng_color_type(PngPixelFormat pixel_format) noexcept
{
    switch (pixel_format) {
    case PngPixelFormat::gray8:
        return SPNG_COLOR_TYPE_GRAYSCALE;
    case PngPixelFormat::gray_alpha8:
        return SPNG_COLOR_TYPE_GRAYSCALE_ALPHA;
    case PngPixelFormat::rgb8:
        return SPNG_COLOR_TYPE_TRUECOLOR;
    case PngPixelFormat::rgba8:
        return SPNG_COLOR_TYPE_TRUECOLOR_ALPHA;
    }

    return SPNG_COLOR_TYPE_TRUECOLOR_ALPHA;
}

inline common::Status
make_spng_status(int error, std::string_view action)
{
    return { common::StatusCode::internal_error,
             std::string(action) + ": " + spng_strerror(error) };
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

    if (image.width > (std::numeric_limits<std::uint32_t>::max)() ||
        image.height > (std::numeric_limits<std::uint32_t>::max)()) {
        return common::Result<ImageLayout>::failure(
            { common::StatusCode::invalid_argument,
              "RasterImageView dimensions must fit in the PNG 32-bit IHDR fields." });
    }

    ImageLayout layout;
    if (!checked_multiply(image.width,
                          bytes_per_pixel(image.pixel_format),
                          layout.row_bytes)) {
        return common::Result<ImageLayout>::failure(
            { common::StatusCode::invalid_argument,
              "RasterImageView row size overflows size_t." });
    }

    layout.effective_stride = image.stride == 0 ? layout.row_bytes : image.stride;
    if (layout.effective_stride < layout.row_bytes) {
        return common::Result<ImageLayout>::failure(
            { common::StatusCode::invalid_argument,
              "RasterImageView.stride must be zero or greater than or equal to the packed row size." });
    }

    std::size_t required_bytes = layout.row_bytes;
    if (image.height > 1) {
        std::size_t tail_bytes = 0;
        if (!checked_multiply(layout.effective_stride, image.height - 1, tail_bytes) ||
            !checked_add(tail_bytes, layout.row_bytes, required_bytes)) {
            return common::Result<ImageLayout>::failure(
                { common::StatusCode::invalid_argument,
                  "RasterImageView buffer size calculation overflows size_t." });
        }
    }

    if (image.pixels.size() < required_bytes) {
        return common::Result<ImageLayout>::failure(
            { common::StatusCode::invalid_argument,
              "RasterImageView.pixels is smaller than the specified width, height, and stride require." });
    }

    return common::Result<ImageLayout>::success(layout);
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
        return common::Result<std::vector<std::uint8_t>>::failure(layout.status());
    }

    std::unique_ptr<spng_ctx, decltype(&spng_ctx_free)> ctx(
        spng_ctx_new(SPNG_CTX_ENCODER),
        &spng_ctx_free);
    if (ctx == nullptr) {
        return common::Result<std::vector<std::uint8_t>>::failure(
            { common::StatusCode::internal_error,
              "spng_ctx_new() failed while creating a PNG encoder context." });
    }

    int error = spng_set_option(ctx.get(), SPNG_ENCODE_TO_BUFFER, 1);
    if (error != 0) {
        return common::Result<std::vector<std::uint8_t>>::failure(
            detail::make_spng_status(error, "spng_set_option(SPNG_ENCODE_TO_BUFFER) failed"));
    }

    error = spng_set_option(
        ctx.get(), SPNG_IMG_COMPRESSION_LEVEL, args.compression_level);
    if (error != 0) {
        return common::Result<std::vector<std::uint8_t>>::failure(
            detail::make_spng_status(error, "spng_set_option(SPNG_IMG_COMPRESSION_LEVEL) failed"));
    }

    spng_ihdr ihdr = {};
    ihdr.width     = static_cast<std::uint32_t>(args.image.width);
    ihdr.height    = static_cast<std::uint32_t>(args.image.height);
    ihdr.bit_depth = 8;
    ihdr.color_type = detail::spng_color_type(args.image.pixel_format);

    error = spng_set_ihdr(ctx.get(), &ihdr);
    if (error != 0) {
        return common::Result<std::vector<std::uint8_t>>::failure(
            detail::make_spng_status(error, "spng_set_ihdr() failed"));
    }

    error = spng_encode_image(
        ctx.get(),
        nullptr,
        0,
        SPNG_FMT_PNG,
        SPNG_ENCODE_PROGRESSIVE | SPNG_ENCODE_FINALIZE);
    if (error != 0) {
        return common::Result<std::vector<std::uint8_t>>::failure(
            detail::make_spng_status(error, "spng_encode_image() initialization failed"));
    }

    const auto &validated = layout.value();
    for (std::size_t row_index = 0; row_index < args.image.height; ++row_index) {
        const auto *row = args.image.pixels.data() + (row_index * validated.effective_stride);
        error = spng_encode_row(ctx.get(), row, validated.row_bytes);
        if (error == SPNG_EOI) {
            break;
        }
        if (error != 0) {
            return common::Result<std::vector<std::uint8_t>>::failure(
                detail::make_spng_status(error, "spng_encode_row() failed"));
        }
    }

    size_t encoded_size = 0;
    error               = 0;
    std::unique_ptr<void, decltype(&std::free)> encoded_png(
        spng_get_png_buffer(ctx.get(), &encoded_size, &error),
        &std::free);
    if (error != 0) {
        return common::Result<std::vector<std::uint8_t>>::failure(
            detail::make_spng_status(error, "spng_get_png_buffer() failed"));
    }
    if (encoded_png == nullptr) {
        return common::Result<std::vector<std::uint8_t>>::failure(
            { common::StatusCode::internal_error,
              "spng_get_png_buffer() returned a null buffer." });
    }

    std::vector<std::uint8_t> bytes(encoded_size);
    if (!bytes.empty()) {
        std::memcpy(bytes.data(), encoded_png.get(), encoded_size);
    }

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
              "Failed to open the PNG output path for writing." });
    }

    const auto &bytes = encoded.value();
    output.write(reinterpret_cast<const char *>(bytes.data()),
                 static_cast<std::streamsize>(bytes.size()));
    if (!output.good()) {
        return common::Result<void>::failure(
            { common::StatusCode::io_error,
              "Failed while writing PNG bytes to the output path." });
    }

    return common::Result<void>::success();
}

} // namespace PDJE_UTIL::function::image
