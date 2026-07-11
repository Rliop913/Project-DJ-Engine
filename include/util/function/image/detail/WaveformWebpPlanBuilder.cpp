#include "WaveformWebpPlanBuilder.hpp"

#include "util/function/image/detail/WaveformWebpSupport.hpp"

#include <stdexcept>

namespace PDJE_UTIL::function::image::detail {
namespace {

std::size_t ceil_divide(std::size_t dividend, std::size_t divisor) noexcept
{
    return dividend / divisor + (dividend % divisor != 0 ? 1u : 0u);
}

} // namespace

WaveformPlanBuilder::WaveformPlanBuilder(
    const EncodeWaveformWebpArgs &args,
    const EncodeWaveformWebpStftArgs *stft_args)
    : args_(args), stft_args_(stft_args)
{
}

WaveformEncodePlan WaveformPlanBuilder::Build() const
{
    Validate();
    WaveformEncodePlan plan;
    plan.chunk_sample_count = ComputeChunkSampleCount();
    plan.buffer_sizes = ComputeBufferSizes();
    plan.channels = SplitChannels(plan.chunk_sample_count);
    plan.batch.resize(plan.channels.size());

    const auto images_per_channel =
        plan.channels.front().size() / plan.chunk_sample_count;
    for (auto &channel : plan.batch) channel.resize(images_per_channel);
    plan.jobs.reserve(support::checked_multiply(
        plan.channels.size(), images_per_channel));

    for (std::size_t channel = 0; channel < plan.channels.size(); ++channel) {
        for (std::size_t image = 0; image < images_per_channel; ++image) {
            plan.jobs.push_back({
                .samples = plan.channels[channel].data() +
                           image * plan.chunk_sample_count,
                .sample_count = plan.chunk_sample_count,
                .channel_index = channel,
                .image_index = image,
                .output_slot = &plan.batch[channel][image]
            });
        }
    }
    return plan;
}

void WaveformPlanBuilder::Validate() const
{
    if (args_.pcm.data() == nullptr || args_.pcm.empty()) {
        throw std::invalid_argument("Waveform PCM data must not be empty.");
    }
    if (args_.channel_count == 0 || args_.y_pixels == 0 ||
        args_.pcm_per_pixel == 0 || args_.x_pixels_per_image == 0) {
        throw std::invalid_argument("Waveform dimensions must be greater than zero.");
    }
    if (args_.compression_level < -1 || args_.compression_level > 9) {
        throw std::invalid_argument("Waveform compression level must be between -1 and 9.");
    }
    if (stft_args_ == nullptr) return;
    if (stft_args_->window_size_exp < 6 || stft_args_->window_size_exp >= 31) {
        throw std::invalid_argument("Waveform STFT window exponent must be between 6 and 30.");
    }
    if (stft_args_->overlap_ratio < 0.0f || stft_args_->overlap_ratio >= 1.0f) {
        throw std::invalid_argument("Waveform STFT overlap must be in [0, 1).");
    }
    if (!stft_args_->mel_filter_bank) return;
    const auto &mel = *stft_args_->mel_filter_bank;
    if (mel.n_fft != static_cast<int>(1u << stft_args_->window_size_exp) ||
        !PDJE_PARALLEL::CheckMelVals(mel) || mel.n_mels < 3) {
        throw std::invalid_argument("Waveform mel filter bank is invalid for RGB output.");
    }
}

std::size_t WaveformPlanBuilder::ComputeChunkSampleCount() const
{
    return support::checked_multiply(
        args_.pcm_per_pixel, args_.x_pixels_per_image);
}

WaveformBufferSizes WaveformPlanBuilder::ComputeBufferSizes() const
{
    WaveformBufferSizes output;
    output.row_stride = support::checked_multiply(args_.x_pixels_per_image, 4);
    output.image_byte_count = support::checked_multiply(
        output.row_stride, args_.y_pixels);
    output.column_rgb_byte_count = support::checked_multiply(
        args_.x_pixels_per_image, 3);
    return output;
}

std::vector<std::vector<float>> WaveformPlanBuilder::SplitChannels(
    std::size_t chunk_sample_count) const
{
    const auto frame_count = ceil_divide(args_.pcm.size(), args_.channel_count);
    const auto images = ceil_divide(frame_count, chunk_sample_count);
    const auto samples = support::checked_multiply(images, chunk_sample_count);
    std::vector<std::vector<float>> channels(
        args_.channel_count, std::vector<float>(samples, 0.0f));
    for (std::size_t index = 0; index < args_.pcm.size(); ++index) {
        channels[index % args_.channel_count][index / args_.channel_count] =
            args_.pcm[index];
    }
    return channels;
}

} // namespace PDJE_UTIL::function::image::detail
