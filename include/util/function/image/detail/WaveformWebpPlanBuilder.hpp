#pragma once

#include "util/common/Result.hpp"
#include "util/function/image/detail/WaveformWebpInternal.hpp"

namespace PDJE_UTIL::function::image::detail {

class WaveformPlanBuilder {
  public:
    WaveformPlanBuilder(const EncodeWaveformWebpArgs     &args,
                        const EncodeWaveformWebpStftArgs *stft_args);

    common::Result<WaveformEncodePlan>
    Build() const;

  private:
    common::Result<void>
    Validate() const;

    common::Result<std::size_t>
    ComputeChunkSampleCount() const;

    common::Result<WaveformBufferSizes>
    ComputeBufferSizes() const;

    common::Result<std::vector<std::vector<float>>>
    SplitChannels(std::size_t chunk_sample_count) const;

    const EncodeWaveformWebpArgs     &args_;
    const EncodeWaveformWebpStftArgs *stft_args_ = nullptr;
};

} // namespace PDJE_UTIL::function::image::detail
