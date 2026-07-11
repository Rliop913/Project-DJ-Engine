#include "util/function/image/WaveformWebp.hpp"

#include "util/function/image/detail/WaveformWebpEncoder.hpp"

namespace PDJE_UTIL::function::image {

WaveformWebpBatch
encode_waveform_webps(const EncodeWaveformWebpArgs &args)
{
    return detail::WaveformWebpEncoder(args).Encode();
}

WaveformWebpBatch
encode_waveform_webps(const EncodeWaveformWebpArgs     &args,
                      const EncodeWaveformWebpStftArgs &stft_args)
{
    return detail::WaveformWebpEncoder(args, stft_args).Encode();
}

} // namespace PDJE_UTIL::function::image
