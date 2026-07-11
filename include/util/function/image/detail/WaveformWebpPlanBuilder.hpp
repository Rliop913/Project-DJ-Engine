#pragma once

#include "util/function/image/detail/WaveformWebpInternal.hpp"

namespace PDJE_UTIL::function::image::detail {

class WaveformPlanBuilder {
  public:
    WaveformPlanBuilder(const EncodeWaveformWebpArgs &args,
                        const EncodeWaveformWebpStftArgs *stft_args);
    WaveformEncodePlan Build() const;

  private:
    void Validate() const;
    std::size_t ComputeChunkSampleCount() const;
    WaveformBufferSizes ComputeBufferSizes() const;
    std::vector<std::vector<float>> SplitChannels(
        std::size_t chunk_sample_count) const;

    const EncodeWaveformWebpArgs &args_;
    const EncodeWaveformWebpStftArgs *stft_args_ = nullptr;
};

} // namespace PDJE_UTIL::function::image::detail
