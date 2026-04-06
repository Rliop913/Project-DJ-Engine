#include "util/function/image/WaveformWebp.hpp"

#include "util/function/image/WaveformWebpEncoder.hpp"

namespace PDJE_UTIL::function::image {

common::Result<WaveformWebpBatch>
encode_waveform_webps(const EncodeWaveformWebpArgs &args,
                      function::EvalOptions         options)
{
    return detail::WaveformWebpEncoder(args, options).Encode();
}

common::Result<WaveformWebpBatch>
encode_waveform_webps(const EncodeWaveformWebpArgs     &args,
                      const EncodeWaveformWebpStftArgs &stft_args,
                      function::EvalOptions             options)
{
    return detail::WaveformWebpEncoder(args, stft_args, options).Encode();
}

} // namespace PDJE_UTIL::function::image
