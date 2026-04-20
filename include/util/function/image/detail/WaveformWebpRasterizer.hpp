#pragma once

#include "util/common/Result.hpp"
#include "util/function/image/detail/WaveformWebpInternal.hpp"
#include "util/function/image/detail/WaveformWebpSupport.hpp"
#include "util/function/FunctionContext.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <utility>

namespace PDJE_UTIL::function::image::detail {

class WaveformRasterizer {
  public:
    WaveformRasterizer(const EncodeWaveformWebpArgs &args,
                       const WaveformBufferSizes    &buffer_sizes,
                       function::EvalOptions         options);

    common::Result<void>
    ComputeExtrema(const WaveformJob      &job,
                   WaveformWorkerContext  &context) const;

    template <class ResolveColorFn> void
    Rasterize(WaveformWorkerContext &context,
              ResolveColorFn       &&resolve_color) const
    {
        std::fill(context.rgba.begin(), context.rgba.end(), std::uint8_t { 0 });

        for (std::size_t x = 0; x < args_.x_pixels_per_image; ++x) {
            std::size_t top_row = support::map_sample_to_row_floor(
                context.column_maxs[x], args_.y_pixels);
            std::size_t bottom_row = support::map_sample_to_row_ceil(
                context.column_mins[x], args_.y_pixels);
            if (top_row > bottom_row) {
                std::swap(top_row, bottom_row);
            }

            const auto color = resolve_color(x);
            for (std::size_t y = top_row; y <= bottom_row; ++y) {
                const std::size_t pixel_offset =
                    (y * buffer_sizes_.row_stride) + (x * 4);
                context.rgba[pixel_offset + 0] = color[0];
                context.rgba[pixel_offset + 1] = color[1];
                context.rgba[pixel_offset + 2] = color[2];
                context.rgba[pixel_offset + 3] = 255;
            }
        }
    }

    common::Result<void>
    Encode(const WaveformJob          &job,
           const WaveformWorkerContext &context,
           EncodedWebpBytes           &output) const;

  private:
    const EncodeWaveformWebpArgs &args_;
    WaveformBufferSizes           buffer_sizes_;
    function::EvalOptions         options_;
};

} // namespace PDJE_UTIL::function::image::detail
