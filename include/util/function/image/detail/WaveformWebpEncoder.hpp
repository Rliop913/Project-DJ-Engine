#pragma once

#include "util/function/image/WaveformWebp.hpp"

namespace PDJE_UTIL::function::image::detail {

class WaveformWebpEncoder {
  public:
    WaveformWebpEncoder(const EncodeWaveformWebpArgs &args,
                        function::EvalOptions         options);
    WaveformWebpEncoder(const EncodeWaveformWebpArgs     &args,
                        const EncodeWaveformWebpStftArgs &stft_args,
                        function::EvalOptions             options);

    common::Result<WaveformWebpBatch>
    Encode() const;

  private:
    enum class Mode { Monochrome, Stft };

    const EncodeWaveformWebpArgs     &args_;
    const EncodeWaveformWebpStftArgs *stft_args_ = nullptr;
    function::EvalOptions             options_;
    Mode                              mode_ = Mode::Monochrome;
};

} // namespace PDJE_UTIL::function::image::detail
