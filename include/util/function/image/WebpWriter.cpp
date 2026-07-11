#include "WebpWriter.hpp"

#include <webp/encode.h>

#include <cstring>
#include <fstream>
#include <limits>
#include <stdexcept>

namespace PDJE_UTIL::function::image {
namespace {

struct ImageLayout {
    std::size_t row_bytes = 0;
    std::size_t stride    = 0;
};

class WebpPicture {
  public:
    WebpPicture()
    {
        if (WebPPictureInit(&value) == 0)
            throw std::runtime_error("Failed to initialize WebP picture.");
    }
    ~WebpPicture()
    {
        WebPPictureFree(&value);
    }

    WebpPicture(const WebpPicture &) = delete;
    WebpPicture &
    operator=(const WebpPicture &) = delete;

    WebPPicture value{};
};

class WebpMemoryOutput {
  public:
    WebpMemoryOutput()
    {
        WebPMemoryWriterInit(&value);
    }
    ~WebpMemoryOutput()
    {
        WebPMemoryWriterClear(&value);
    }

    WebpMemoryOutput(const WebpMemoryOutput &) = delete;
    WebpMemoryOutput &
    operator=(const WebpMemoryOutput &) = delete;

    WebPMemoryWriter value{};
};

std::size_t
checked_multiply(std::size_t left, std::size_t right)
{
    if (left != 0 && right > std::numeric_limits<std::size_t>::max() / left) {
        throw std::overflow_error("Image buffer size calculation overflowed.");
    }
    return left * right;
}

std::size_t
bytes_per_pixel(RasterPixelFormat format)
{
    switch (format) {
    case RasterPixelFormat::gray8:
        return 1;
    case RasterPixelFormat::gray_alpha8:
        return 2;
    case RasterPixelFormat::rgb8:
        return 3;
    case RasterPixelFormat::rgba8:
        return 4;
    }
    throw std::invalid_argument("Unsupported raster pixel format.");
}

ImageLayout
validate(const RasterImageView &image)
{
    if (image.pixels.data() == nullptr || image.width == 0 ||
        image.height == 0) {
        throw std::invalid_argument(
            "Raster image data and dimensions are required.");
    }
    if (image.width > 16383 || image.height > 16383) {
        throw std::invalid_argument(
            "Raster image dimensions exceed WebP limits.");
    }
    ImageLayout layout{ .row_bytes = checked_multiply(
                            image.width, bytes_per_pixel(image.pixel_format)) };
    layout.stride = image.stride == 0 ? layout.row_bytes : image.stride;
    if (layout.stride < layout.row_bytes) {
        throw std::invalid_argument(
            "Raster image stride is smaller than a packed row.");
    }
    const auto tail = checked_multiply(layout.stride, image.height - 1);
    if (tail > std::numeric_limits<std::size_t>::max() - layout.row_bytes) {
        throw std::overflow_error(
            "Raster image buffer size calculation overflowed.");
    }
    if (image.pixels.size() < tail + layout.row_bytes) {
        throw std::invalid_argument("Raster image pixel span is too small.");
    }
    return layout;
}

std::vector<std::uint8_t>
pack_rgba(const RasterImageView &image, const ImageLayout &layout)
{
    std::vector<std::uint8_t> output(
        checked_multiply(checked_multiply(image.width, image.height), 4));
    const auto pixel_size = bytes_per_pixel(image.pixel_format);
    for (std::size_t row = 0; row < image.height; ++row) {
        const auto *source = image.pixels.data() + row * layout.stride;
        auto       *target = output.data() + row * image.width * 4;
        for (std::size_t column = 0; column < image.width; ++column) {
            const auto *pixel = source + column * pixel_size;
            auto       *rgba  = target + column * 4;
            switch (image.pixel_format) {
            case RasterPixelFormat::gray8:
                rgba[0] = rgba[1] = rgba[2] = pixel[0];
                rgba[3]                     = 255;
                break;
            case RasterPixelFormat::gray_alpha8:
                rgba[0] = rgba[1] = rgba[2] = pixel[0];
                rgba[3]                     = pixel[1];
                break;
            case RasterPixelFormat::rgb8:
                rgba[0] = pixel[0];
                rgba[1] = pixel[1];
                rgba[2] = pixel[2];
                rgba[3] = 255;
                break;
            case RasterPixelFormat::rgba8:
                std::memcpy(rgba, pixel, 4);
                break;
            }
        }
    }
    return output;
}

} // namespace

std::vector<std::uint8_t>
encode_webp(const EncodeWebpArgs &args)
{
    if (args.compression_level < -1 || args.compression_level > 9) {
        throw std::invalid_argument(
            "WebP compression level must be between -1 and 9.");
    }
    const auto                layout = validate(args.image);
    std::vector<std::uint8_t> packed;
    const std::uint8_t       *pixels = args.image.pixels.data();
    int                       stride = 0;
    if (args.image.pixel_format == RasterPixelFormat::rgba8) {
        if (layout.stride >
            static_cast<std::size_t>(std::numeric_limits<int>::max())) {
            throw std::overflow_error("WebP stride exceeds encoder limits.");
        }
        stride = static_cast<int>(layout.stride);
    } else {
        packed = pack_rgba(args.image, layout);
        pixels = packed.data();
        stride = static_cast<int>(args.image.width * 4);
    }
    WebPConfig config{};
    if (WebPConfigInit(&config) == 0)
        throw std::runtime_error("Failed to initialize WebP config.");
    const int level = args.compression_level < 0 ? 6 : args.compression_level;
    if (WebPConfigLosslessPreset(&config, level) == 0 ||
        WebPValidateConfig(&config) == 0) {
        throw std::invalid_argument("WebP lossless preset is invalid.");
    }
    config.exact = 0;

    WebpPicture picture;
    picture.value.use_argb = 1;
    picture.value.width    = static_cast<int>(args.image.width);
    picture.value.height   = static_cast<int>(args.image.height);
    if (WebPPictureImportRGBA(&picture.value, pixels, stride) == 0)
        throw std::runtime_error("Failed to import WebP RGBA pixels.");

    WebpMemoryOutput writer;
    picture.value.writer     = &WebPMemoryWrite;
    picture.value.custom_ptr = &writer.value;
    if (WebPEncode(&config, &picture.value) == 0) {
        throw std::runtime_error(
            "WebP encoder failed with error code " +
            std::to_string(static_cast<int>(picture.value.error_code)) + ".");
    }
    if (writer.value.mem == nullptr || writer.value.size == 0u)
        throw std::runtime_error("WebP encoder returned an empty payload.");

    return { writer.value.mem, writer.value.mem + writer.value.size };
}

void
write_webp(const WriteWebpArgs &args)
{
    if (args.output_path.empty()) {
        throw std::invalid_argument("WebP output path must not be empty.");
    }
    const auto bytes = encode_webp(
        { .image = args.image, .compression_level = args.compression_level });
    std::ofstream output(args.output_path, std::ios::binary | std::ios::trunc);
    if (!output)
        throw std::runtime_error("Failed to open WebP output path.");
    output.write(reinterpret_cast<const char *>(bytes.data()),
                 static_cast<std::streamsize>(bytes.size()));
    if (!output.good())
        throw std::runtime_error("Failed to write WebP output.");
}

} // namespace PDJE_UTIL::function::image
