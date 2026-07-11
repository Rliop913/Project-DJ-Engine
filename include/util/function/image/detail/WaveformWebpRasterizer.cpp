#include "WaveformWebpRasterizer.hpp"

#include "util/function/image/WebpWriter.hpp"
#include "util/function/image/detail/WaveformWebpHighway-inl.h"

#include <stdexcept>

HWY_EXPORT(ComputeWaveformColumnExtremaSIMD);

namespace PDJE_UTIL::function::image::detail {

WaveformRasterizer::WaveformRasterizer(
    const EncodeWaveformWebpArgs &args,
    const WaveformBufferSizes &buffer_sizes)
    : args_(args), buffer_sizes_(buffer_sizes)
{
}

void WaveformRasterizer::ComputeExtrema(
    const WaveformJob &job, WaveformWorkerContext &context) const
{
    const auto expected = support::checked_multiply(
        args_.pcm_per_pixel, args_.x_pixels_per_image);
    if (job.sample_count != expected) {
        throw support::job_error(job, "chunk size does not match waveform layout");
    }
    if (context.column_mins.size() != args_.x_pixels_per_image ||
        context.column_maxs.size() != args_.x_pixels_per_image) {
        throw support::job_error(job, "extrema buffers do not match image width");
    }
    HWY_DYNAMIC_DISPATCH(ComputeWaveformColumnExtremaSIMD)(
        job.samples,
        args_.pcm_per_pixel,
        args_.x_pixels_per_image,
        context.column_mins.data(),
        context.column_maxs.data());
}

void WaveformRasterizer::Encode(const WaveformJob &,
                                const WaveformWorkerContext &context,
                                EncodedWebpBytes &output) const
{
    output = encode_webp({
        .image = {
            .pixels = context.rgba,
            .width = args_.x_pixels_per_image,
            .height = args_.y_pixels,
            .stride = buffer_sizes_.row_stride,
            .pixel_format = RasterPixelFormat::rgba8,
        },
        .compression_level = args_.compression_level,
    });
}

} // namespace PDJE_UTIL::function::image::detail
