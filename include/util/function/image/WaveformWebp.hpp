#pragma once

#include "global/PDJE_EXPORT_SETTER.hpp"
#include "util/common/Result.hpp"
#include "util/function/FunctionContext.hpp"
#include "util/function/stft/STFT_Parallel.hpp"

#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>
#include <vector>

namespace PDJE_UTIL::function::image {

using EncodedWebpBytes = std::vector<std::uint8_t>;
using ChannelWaveformWebps = std::vector<EncodedWebpBytes>;
using WaveformWebpBatch = std::vector<ChannelWaveformWebps>;

struct EncodeWaveformWebpArgs {
    std::span<const float> pcm;
    std::size_t            channel_count = 0;
    std::size_t            y_pixels      = 0;
    std::size_t            pcm_per_pixel = 0;
    std::size_t            x_pixels_per_image = 0;
    int                    compression_level  = -1;
    std::size_t            worker_thread_count = 0;
};

struct EncodeWaveformWebpStftArgs {
    PDJE_PARALLEL::WINDOW_LIST  target_window =
        PDJE_PARALLEL::WINDOW_LIST::HANNING;
    int                         window_size_exp = 10;
    float                       overlap_ratio   = 0.5f;
    PDJE_PARALLEL::POST_PROCESS post_process    = {};
    std::optional<PDJE_PARALLEL::MelFilterBankSpec> mel_filter_bank {};
};

PDJE_API common::Result<WaveformWebpBatch>
encode_waveform_webps(const EncodeWaveformWebpArgs &args,
                      function::EvalOptions         options = {});

PDJE_API common::Result<WaveformWebpBatch>
encode_waveform_webps(const EncodeWaveformWebpArgs     &args,
                      const EncodeWaveformWebpStftArgs &stft_args,
                      function::EvalOptions             options = {});

} // namespace PDJE_UTIL::function::image
