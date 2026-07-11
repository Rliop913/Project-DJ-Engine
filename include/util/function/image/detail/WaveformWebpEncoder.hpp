#pragma once

#include "util/function/image/WaveformWebp.hpp"

namespace PDJE_UTIL::function::image::detail {

class WaveformWebpEncoder {
  public:
    explicit WaveformWebpEncoder(const EncodeWaveformWebpArgs &args);
    WaveformWebpEncoder(const EncodeWaveformWebpArgs     &args,
                        const EncodeWaveformWebpStftArgs &stft_args);

    WaveformWebpBatch
    Encode() const;

  private:
    enum class Mode { Monochrome, Stft };

    const EncodeWaveformWebpArgs     &args_;
    const EncodeWaveformWebpStftArgs *stft_args_ = nullptr;
    Mode                              mode_      = Mode::Monochrome;
};

} // namespace PDJE_UTIL::function::image::detail
