#include "util/function/image/WaveformWebpPlanBuilder.hpp"

#include "util/function/image/WaveformWebpSupport.hpp"

#include <cstddef>
#include <utility>
#include <vector>

namespace PDJE_UTIL::function::image::detail {

namespace {

std::size_t
ceil_divide(const std::size_t dividend, const std::size_t divisor) noexcept
{
    return (dividend / divisor) + ((dividend % divisor) != 0 ? 1u : 0u);
}

} // namespace

WaveformPlanBuilder::WaveformPlanBuilder(
    const EncodeWaveformWebpArgs     &args,
    const EncodeWaveformWebpStftArgs *stft_args)
    : args_(args), stft_args_(stft_args)
{
}

common::Result<WaveformEncodePlan>
WaveformPlanBuilder::Build() const
{
    const auto validation = Validate();
    if (!validation.ok()) {
        return common::Result<WaveformEncodePlan>::failure(validation.status());
    }

    WaveformEncodePlan plan;

    auto chunk_sample_count = ComputeChunkSampleCount();
    if (!chunk_sample_count.ok()) {
        return common::Result<WaveformEncodePlan>::failure(
            chunk_sample_count.status());
    }
    plan.chunk_sample_count = chunk_sample_count.value();

    auto buffer_sizes = ComputeBufferSizes();
    if (!buffer_sizes.ok()) {
        return common::Result<WaveformEncodePlan>::failure(buffer_sizes.status());
    }
    plan.buffer_sizes = buffer_sizes.value();

    auto split_channels = SplitChannels(plan.chunk_sample_count);
    if (!split_channels.ok()) {
        return common::Result<WaveformEncodePlan>::failure(
            split_channels.status());
    }
    plan.channels = std::move(split_channels).value();
    plan.batch.resize(plan.channels.size());

    const std::size_t image_count_per_channel =
        plan.channels.front().size() / plan.chunk_sample_count;
    for (auto &channel_batch : plan.batch) {
        channel_batch.resize(image_count_per_channel);
    }

    std::size_t total_job_count = 0;
    if (!support::checked_multiply(
            plan.channels.size(), image_count_per_channel, total_job_count)) {
        return common::Result<WaveformEncodePlan>::failure(
            { common::StatusCode::invalid_argument,
              "Waveform job count overflows size_t." });
    }

    plan.jobs.reserve(total_job_count);
    for (std::size_t channel_index = 0; channel_index < plan.channels.size();
         ++channel_index) {
        auto &channel_pcm = plan.channels[channel_index];
        for (std::size_t image_index = 0;
             image_index < plan.batch[channel_index].size();
             ++image_index) {
            const std::size_t sample_offset = image_index * plan.chunk_sample_count;
            plan.jobs.push_back({ .samples = channel_pcm.data() + sample_offset,
                                  .sample_count = plan.chunk_sample_count,
                                  .channel_index = channel_index,
                                  .image_index = image_index,
                                  .output_slot =
                                      &plan.batch[channel_index][image_index] });
        }
    }

    return common::Result<WaveformEncodePlan>::success(std::move(plan));
}

common::Result<void>
WaveformPlanBuilder::Validate() const
{
    if (args_.pcm.data() == nullptr) {
        return common::Result<void>::failure(
            { common::StatusCode::invalid_argument,
              "EncodeWaveformWebpArgs.pcm must reference valid PCM data." });
    }

    if (args_.pcm.empty()) {
        return common::Result<void>::failure(
            { common::StatusCode::invalid_argument,
              "EncodeWaveformWebpArgs.pcm must not be empty." });
    }

    if (args_.channel_count == 0) {
        return common::Result<void>::failure(
            { common::StatusCode::invalid_argument,
              "EncodeWaveformWebpArgs.channel_count must be greater than zero." });
    }

    if (args_.y_pixels == 0) {
        return common::Result<void>::failure(
            { common::StatusCode::invalid_argument,
              "EncodeWaveformWebpArgs.y_pixels must be greater than zero." });
    }

    if (args_.pcm_per_pixel == 0) {
        return common::Result<void>::failure(
            { common::StatusCode::invalid_argument,
              "EncodeWaveformWebpArgs.pcm_per_pixel must be greater than zero." });
    }

    if (args_.x_pixels_per_image == 0) {
        return common::Result<void>::failure(
            { common::StatusCode::invalid_argument,
              "EncodeWaveformWebpArgs.x_pixels_per_image must be greater than "
              "zero." });
    }

    if (args_.compression_level < -1 || args_.compression_level > 9) {
        return common::Result<void>::failure(
            { common::StatusCode::invalid_argument,
              "EncodeWaveformWebpArgs.compression_level must be between -1 and "
              "9." });
    }

    if (stft_args_ != nullptr) {
        if (stft_args_->window_size_exp < 6 || stft_args_->window_size_exp >= 31) {
            return common::Result<void>::failure(
                { common::StatusCode::invalid_argument,
                  "EncodeWaveformWebpStftArgs.window_size_exp must be between "
                  "6 and 30." });
        }

        if (stft_args_->overlap_ratio < 0.0f ||
            stft_args_->overlap_ratio >= 1.0f) {
            return common::Result<void>::failure(
                { common::StatusCode::invalid_argument,
                  "EncodeWaveformWebpStftArgs.overlap_ratio must be greater "
                  "than or equal to 0.0 and less than 1.0." });
        }
    }

    return common::Result<void>::success();
}

common::Result<std::size_t>
WaveformPlanBuilder::ComputeChunkSampleCount() const
{
    std::size_t chunk_sample_count = 0;
    if (!support::checked_multiply(
            args_.pcm_per_pixel, args_.x_pixels_per_image, chunk_sample_count)) {
        return common::Result<std::size_t>::failure(
            { common::StatusCode::invalid_argument,
              "Waveform chunk sample count overflows size_t." });
    }

    return common::Result<std::size_t>::success(chunk_sample_count);
}

common::Result<WaveformBufferSizes>
WaveformPlanBuilder::ComputeBufferSizes() const
{
    WaveformBufferSizes buffer_sizes;
    if (!support::checked_multiply(
            args_.x_pixels_per_image, std::size_t { 4 }, buffer_sizes.row_stride) ||
        !support::checked_multiply(buffer_sizes.row_stride,
                                   args_.y_pixels,
                                   buffer_sizes.image_byte_count)) {
        return common::Result<WaveformBufferSizes>::failure(
            { common::StatusCode::invalid_argument,
              "Waveform image size overflows size_t." });
    }

    if (!support::checked_multiply(args_.x_pixels_per_image,
                                   std::size_t { 3 },
                                   buffer_sizes.column_rgb_byte_count)) {
        return common::Result<WaveformBufferSizes>::failure(
            { common::StatusCode::invalid_argument,
              "Waveform column RGB size overflows size_t." });
    }

    return common::Result<WaveformBufferSizes>::success(buffer_sizes);
}

common::Result<std::vector<std::vector<float>>>
WaveformPlanBuilder::SplitChannels(const std::size_t chunk_sample_count) const
{
    const std::size_t frame_count =
        ceil_divide(args_.pcm.size(), args_.channel_count);
    const std::size_t image_count_per_channel =
        ceil_divide(frame_count, chunk_sample_count);

    std::size_t final_channel_sample_count = 0;
    if (!support::checked_multiply(image_count_per_channel,
                                   chunk_sample_count,
                                   final_channel_sample_count)) {
        return common::Result<std::vector<std::vector<float>>>::failure(
            { common::StatusCode::invalid_argument,
              "Waveform channel sample count overflows size_t." });
    }

    std::vector<std::vector<float>> channels(
        args_.channel_count,
        std::vector<float>(final_channel_sample_count, 0.0f));

    for (std::size_t sample_index = 0; sample_index < args_.pcm.size();
         ++sample_index) {
        const std::size_t channel_index = sample_index % args_.channel_count;
        const std::size_t frame_index   = sample_index / args_.channel_count;
        channels[channel_index][frame_index] = args_.pcm[sample_index];
    }

    return common::Result<std::vector<std::vector<float>>>::success(
        std::move(channels));
}

} // namespace PDJE_UTIL::function::image::detail
