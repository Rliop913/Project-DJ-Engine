#pragma once

#include "util/common/Result.hpp"
#include "util/function/image/detail/WaveformWebpInternal.hpp"
#include "util/function/image/detail/WaveformWebpRasterizer.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <span>
#include <utility>
#include <vector>

namespace PDJE_UTIL::function::image::detail {

class MonochromeColorMapper {
  public:
    common::Result<void>
    Prepare(const WaveformJob &job)
    {
        (void)job;
        return common::Result<void>::success();
    }

    std::array<std::uint8_t, 3>
    ColorAt(std::size_t x) const
    {
        (void)x;
        return { 255, 255, 255 };
    }
};

class StftColorMapper {
  public:
    StftColorMapper(const EncodeWaveformWebpArgs     &args,
                    const EncodeWaveformWebpStftArgs &stft_args,
                    const WaveformBufferSizes        &buffer_sizes,
                    std::size_t                       chunk_sample_count);

    common::Result<void>
    Prepare(const WaveformJob &job);

    std::array<std::uint8_t, 3>
    ColorAt(std::size_t x) const;

  private:
    common::Result<void>
    MapStftRgbToColumns(std::span<const float> rgb_frames);

    const EncodeWaveformWebpArgs     &args_;
    const EncodeWaveformWebpStftArgs &stft_args_;
    std::vector<float>                stft_pcm_;
    std::vector<std::uint8_t>         column_rgb_;
    PDJE_PARALLEL::STFT               stft_;
};

template <class ColorMapper> class WaveformJobProcessor {
  public:
    template <class... MapperArgs>
    WaveformJobProcessor(const EncodeWaveformWebpArgs &args,
                         const WaveformBufferSizes    &buffer_sizes,
                         function::EvalOptions         options,
                         MapperArgs                   &&...mapper_args)
        : rasterizer_(args, buffer_sizes, options),
          context_(args.x_pixels_per_image, buffer_sizes.image_byte_count),
          color_mapper_(std::forward<MapperArgs>(mapper_args)...)
    {
    }

    common::Result<void>
    Process(const WaveformJob &job, EncodedWebpBytes &output)
    {
        auto extrema = rasterizer_.ComputeExtrema(job, context_);
        if (!extrema.ok()) {
            return extrema;
        }

        auto prepared = color_mapper_.Prepare(job);
        if (!prepared.ok()) {
            return prepared;
        }

        rasterizer_.Rasterize(context_,
                              [&](const std::size_t x) {
                                  return color_mapper_.ColorAt(x);
                              });
        return rasterizer_.Encode(job, context_, output);
    }

  private:
    WaveformRasterizer    rasterizer_;
    WaveformWorkerContext context_;
    ColorMapper           color_mapper_;
};

} // namespace PDJE_UTIL::function::image::detail
