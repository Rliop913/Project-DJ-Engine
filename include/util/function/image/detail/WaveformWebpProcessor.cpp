#include "WaveformWebpProcessor.hpp"

#include "util/function/image/detail/WaveformWebpSupport.hpp"

#include <algorithm>
#include <stdexcept>

namespace PDJE_UTIL::function::image::detail {
namespace {

unsigned int hop_length(const EncodeWaveformWebpStftArgs &args) noexcept
{
    const auto window = static_cast<unsigned int>(1u << args.window_size_exp);
    return std::max(1u, static_cast<unsigned int>(
        static_cast<float>(window) * (1.0f - args.overlap_ratio)));
}

PDJE_PARALLEL::STFTRequest make_request(
    const EncodeWaveformWebpStftArgs &args)
{
    auto post_process = args.post_process;
    post_process.to_rgb = true;
    post_process.check_values();
    const int n_fft = static_cast<int>(1u << args.window_size_exp);
    auto mel = args.mel_filter_bank;
    if (!mel) {
        mel = PDJE_PARALLEL::MelFilterBankSpec {
            .sample_rate = 48000,
            .n_fft = n_fft,
            .n_mels = 80,
            .f_min = 0.0f,
            .f_max = -1.0f,
            .mel_formula = PDJE_PARALLEL::MelFormula::Slaney,
            .norm = PDJE_PARALLEL::MelNorm::Slaney,
        };
    }
    return {
        .sample_rate = mel->sample_rate,
        .n_fft = n_fft,
        .hop_length = hop_length(args),
        .target_window = args.target_window,
        .post_process = post_process,
        .frame_policy = PDJE_PARALLEL::FRAME_POLICY::LEGACY_ZERO_PAD,
        .mel_filter_bank = mel,
    };
}

} // namespace

StftColorMapper::StftColorMapper(const Args &args)
    : args_(args.waveform),
      stft_args_(args.stft),
      stft_pcm_(args.chunk_sample_count, 0.0f),
      column_rgb_(args.buffers.column_rgb_byte_count, 0)
{
}

void StftColorMapper::Prepare(const WaveformJob &job)
{
    stft_pcm_.assign(job.samples, job.samples + job.sample_count);
    auto [real, imaginary] = stft_.calculate(stft_pcm_, make_request(stft_args_));
    if (!imaginary.empty()) {
        throw support::job_error(job, "STFT RGB output contained imaginary data");
    }
    if (real.empty()) {
        throw support::job_error(job, "STFT RGB output was empty");
    }
    try {
        MapStftRgbToColumns(real);
    } catch (const std::exception &error) {
        throw support::job_error(job, error.what());
    }
}

std::array<std::uint8_t, 3> StftColorMapper::ColorAt(std::size_t column) const
{
    const auto offset = column * 3;
    return { column_rgb_[offset], column_rgb_[offset + 1], column_rgb_[offset + 2] };
}

void StftColorMapper::MapStftRgbToColumns(std::span<const float> frames)
{
    if (frames.empty() || frames.size() % 3 != 0) {
        throw std::runtime_error("STFT RGB output must contain RGB triplets.");
    }
    if (column_rgb_.size() !=
        support::checked_multiply(args_.x_pixels_per_image, 3)) {
        throw std::runtime_error("Waveform RGB buffer does not match image width.");
    }
    const auto frame_count = frames.size() / 3;
    for (std::size_t column = 0; column < args_.x_pixels_per_image; ++column) {
        const auto frame = support::map_column_to_stft_frame(
            column, frame_count, args_.x_pixels_per_image);
        for (std::size_t channel = 0; channel < 3; ++channel) {
            column_rgb_[column * 3 + channel] =
                support::unit_float_to_byte(frames[frame * 3 + channel]);
        }
    }
}

} // namespace PDJE_UTIL::function::image::detail
