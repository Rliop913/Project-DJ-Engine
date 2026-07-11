#pragma once

#include "global/PDJE_EXPORT_SETTER.hpp"

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <span>
#include <vector>

namespace PDJE_UTIL::function::image {

enum class RasterPixelFormat { gray8, gray_alpha8, rgb8, rgba8 };

struct RasterImageView {
    std::span<const std::uint8_t> pixels {};
    std::size_t width = 0;
    std::size_t height = 0;
    std::size_t stride = 0;
    RasterPixelFormat pixel_format = RasterPixelFormat::rgba8;
};

struct EncodeWebpArgs {
    RasterImageView image;
    int compression_level = -1;
};

struct WriteWebpArgs {
    RasterImageView image;
    std::filesystem::path output_path;
    int compression_level = -1;
};

PDJE_API std::vector<std::uint8_t> encode_webp(const EncodeWebpArgs &args);
PDJE_API void write_webp(const WriteWebpArgs &args);

} // namespace PDJE_UTIL::function::image
