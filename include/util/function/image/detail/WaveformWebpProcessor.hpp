#pragma once

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
    void
    Prepare(const WaveformJob &job)
    {
        (void)job;
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
    struct Args {
        const EncodeWaveformWebpArgs &waveform;
        const EncodeWaveformWebpStftArgs &stft;
        const WaveformBufferSizes &buffers;
        std::size_t chunk_sample_count;
    };

    explicit StftColorMapper(const Args &args);

    void
    Prepare(const WaveformJob &job);

    std::array<std::uint8_t, 3>
    ColorAt(std::size_t x) const;

  private:
    void
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
                         MapperArgs                   &&...mapper_args)
        : rasterizer_(args, buffer_sizes),
          context_(args.x_pixels_per_image, buffer_sizes.image_byte_count),
          color_mapper_(std::forward<MapperArgs>(mapper_args)...)
    {
    }

    void
    Process(const WaveformJob &job, EncodedWebpBytes &output)
    {
        rasterizer_.ComputeExtrema(job, context_);
        color_mapper_.Prepare(job);

        rasterizer_.Rasterize(context_,
                              [&](const std::size_t x) {
                                  return color_mapper_.ColorAt(x);
                              });
        rasterizer_.Encode(job, context_, output);
    }

  private:
    WaveformRasterizer    rasterizer_;
    WaveformWorkerContext context_;
    ColorMapper           color_mapper_;
};

} // namespace PDJE_UTIL::function::image::detail
