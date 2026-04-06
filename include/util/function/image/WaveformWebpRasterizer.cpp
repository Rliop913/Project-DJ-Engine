#include "util/function/image/WaveformWebpRasterizer.hpp"

#include "util/function/image/WebpWriter.hpp"
#include "util/function/image/WaveformWebpSupport.hpp"
#include "WaveformWebpHighway-inl.h"

#include <cstddef>
#include <span>
#include <utility>

HWY_EXPORT(ComputeWaveformColumnExtremaSIMD);

namespace PDJE_UTIL::function::image::detail {

WaveformRasterizer::WaveformRasterizer(const EncodeWaveformWebpArgs &args,
                                       const WaveformBufferSizes    &buffer_sizes,
                                       function::EvalOptions         options)
    : args_(args), buffer_sizes_(buffer_sizes), options_(options)
{
}

common::Result<void>
WaveformRasterizer::ComputeExtrema(const WaveformJob     &job,
                                   WaveformWorkerContext &context) const
{
    std::size_t expected_sample_count = 0;
    if (!support::checked_multiply(
            args_.pcm_per_pixel, args_.x_pixels_per_image, expected_sample_count)) {
        return common::Result<void>::failure(support::wrap_job_status(
            { common::StatusCode::invalid_argument,
              "Waveform chunk sample count overflows size_t." },
            job));
    }

    if (job.sample_count != expected_sample_count) {
        return common::Result<void>::failure(support::wrap_job_status(
            { common::StatusCode::invalid_argument,
              "Waveform chunk span size does not match pcm_per_pixel * "
              "x_pixels_per_image." },
            job));
    }

    if (context.column_mins.size() != args_.x_pixels_per_image ||
        context.column_maxs.size() != args_.x_pixels_per_image) {
        return common::Result<void>::failure(support::wrap_job_status(
            { common::StatusCode::invalid_argument,
              "Waveform extrema buffers must match x_pixels_per_image." },
            job));
    }

    HWY_DYNAMIC_DISPATCH(ComputeWaveformColumnExtremaSIMD)(job.samples,
                                                           args_.pcm_per_pixel,
                                                           args_.x_pixels_per_image,
                                                           context.column_mins.data(),
                                                           context.column_maxs.data());
    return common::Result<void>::success();
}

common::Result<void>
WaveformRasterizer::Encode(const WaveformJob           &job,
                           const WaveformWorkerContext &context,
                           EncodedWebpBytes            &output) const
{
    auto encoded = encode_webp(
        { .image =
              {
                  .pixels = context.rgba,
                  .width = args_.x_pixels_per_image,
                  .height = args_.y_pixels,
                  .stride = buffer_sizes_.row_stride,
                  .pixel_format = RasterPixelFormat::rgba8,
              },
          .compression_level = args_.compression_level },
        options_);
    if (!encoded.ok()) {
        return common::Result<void>::failure(
            support::wrap_job_status(encoded.status(), job));
    }

    output = std::move(encoded).value();
    return common::Result<void>::success();
}

} // namespace PDJE_UTIL::function::image::detail
