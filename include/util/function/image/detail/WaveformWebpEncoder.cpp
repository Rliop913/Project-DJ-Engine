#include "WaveformWebpEncoder.hpp"

#include "util/function/image/detail/WaveformWebpPlanBuilder.hpp"
#include "util/function/image/detail/WaveformWebpProcessor.hpp"
#include "util/function/image/detail/WaveformWebpWorkerRunner.hpp"

namespace PDJE_UTIL::function::image::detail {

WaveformWebpEncoder::WaveformWebpEncoder(const EncodeWaveformWebpArgs &args)
    : args_(args)
{
}

WaveformWebpEncoder::WaveformWebpEncoder(
    const EncodeWaveformWebpArgs     &args,
    const EncodeWaveformWebpStftArgs &stft_args)
    : args_(args), stft_args_(&stft_args), mode_(Mode::Stft)
{
}

WaveformWebpBatch
WaveformWebpEncoder::Encode() const
{
    auto                 plan = WaveformPlanBuilder(args_, stft_args_).Build();
    WaveformWorkerRunner runner(args_.worker_thread_count);
    if (mode_ == Mode::Stft) {
        return runner.Run(plan, [&] {
            return WaveformJobProcessor<StftColorMapper>(
                args_,
                plan.buffer_sizes,
                StftColorMapper::Args{ args_,
                                       *stft_args_,
                                       plan.buffer_sizes,
                                       plan.chunk_sample_count });
        });
    }
    return runner.Run(plan, [&] {
        return WaveformJobProcessor<MonochromeColorMapper>(args_,
                                                           plan.buffer_sizes);
    });
}

} // namespace PDJE_UTIL::function::image::detail
