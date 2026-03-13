#pragma once

#include "util/common/Result.hpp"
#include "util/function/FunctionContext.hpp"

#include <cstddef>
#include <cstdint>
#include <span>
#include <vector>

namespace PDJE_UTIL::function::image {

using EncodedPngBytes = std::vector<std::uint8_t>;
using ChannelWaveformPngs = std::vector<EncodedPngBytes>;
using WaveformPngBatch = std::vector<ChannelWaveformPngs>;

struct EncodeWaveformPngArgs {
    std::span<const float> pcm;
    std::size_t            channel_count = 0;
    std::size_t            y_pixels = 0;
    std::size_t            pcm_per_pixel = 0;
    std::size_t            x_pixels_per_png = 0;
    int                    compression_level = -1;
    std::size_t            worker_thread_count = 0;
};

common::Result<WaveformPngBatch>
encode_waveform_pngs(const EncodeWaveformPngArgs &args,
                     function::EvalOptions        options = {});

} // namespace PDJE_UTIL::function::image
