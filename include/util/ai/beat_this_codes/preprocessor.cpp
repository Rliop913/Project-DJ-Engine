#include "preprocessor.hpp"

#include "soxr.h"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <memory>
#include <span>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace beat_this::preprocess {

const FrontendConfig& PreprocessPipeline::Config() noexcept {
  static const FrontendConfig config;
  return config;
}

void PreprocessPipeline::ValidateAudioBuffer(const std::span<const float> samples,
                                             const int input_sample_rate) {
  if (input_sample_rate <= 0) {
    throw std::invalid_argument("input_sample_rate must be positive");
  }
  if (samples.empty()) {
    throw std::invalid_argument("audio buffer must be non-empty");
  }
}

std::vector<double> PreprocessPipeline::CopyMonoToDouble(const std::span<const float> samples) {
  std::vector<double> mono(samples.size(), 0.0);
  for (std::size_t idx = 0; idx < samples.size(); ++idx) {
    mono[idx] = static_cast<double>(samples[idx]);
  }
  return mono;
}

void PreprocessPipeline::EnsureResampleCapacity(std::vector<double>& output,
                                                const std::size_t out_pos) {
  if (out_pos < output.size()) {
    return;
  }
  const std::size_t next_size = std::max(output.size() * 2U, out_pos + kFlushChunkFrames);
  output.resize(next_size, 0.0);
}

std::vector<double> PreprocessPipeline::ResampleMonoWaveform(
    const std::span<const double> mono,
    const int input_sample_rate,
    const int output_sample_rate) {
  if (input_sample_rate == output_sample_rate) {
    return std::vector<double>(mono.begin(), mono.end());
  }

  const double ratio =
      static_cast<double>(output_sample_rate) / static_cast<double>(input_sample_rate);
  const std::size_t chunk_frames = static_cast<std::size_t>(
      std::max(1000.0,
               48000.0 * static_cast<double>(input_sample_rate) /
                   static_cast<double>(output_sample_rate)));
  const soxr_io_spec_t io_spec = soxr_io_spec(SOXR_FLOAT64_I, SOXR_FLOAT64_I);
  const soxr_quality_spec_t quality_spec = soxr_quality_spec(SOXR_HQ, 0);
  soxr_error_t create_error = nullptr;
  soxr_t raw_resampler = soxr_create(static_cast<double>(input_sample_rate),
                                     static_cast<double>(output_sample_rate),
                                     1,
                                     &create_error,
                                     &io_spec,
                                     &quality_spec,
                                     nullptr);
  if (create_error != nullptr || raw_resampler == nullptr) {
    throw std::runtime_error(std::string("libsoxr streaming create failed: ") +
                             (create_error != nullptr ? create_error : "unknown error"));
  }
  std::unique_ptr<soxr, decltype(&soxr_delete)> resampler(raw_resampler, &soxr_delete);
  const double expected_frames =
      soxr_delay(resampler.get()) + (static_cast<double>(mono.size()) * ratio) + 1.0;
  const std::size_t initial_capacity = std::max(
      kResampleBufferFloorFrames,
      static_cast<std::size_t>(std::ceil(std::max(1.0, expected_frames))));

  std::vector<double> output(initial_capacity, 0.0);
  std::size_t out_pos = 0;

  for (std::size_t idx = 0; idx < mono.size(); idx += chunk_frames) {
    const double* input = mono.data() + idx;
    std::size_t input_remaining = std::min(chunk_frames, mono.size() - idx);

    while (input_remaining > 0) {
      EnsureResampleCapacity(output, out_pos);

      std::size_t input_done = 0;
      std::size_t output_done = 0;
      const soxr_error_t error = soxr_process(resampler.get(),
                                              input,
                                              input_remaining,
                                              &input_done,
                                              output.data() + out_pos,
                                              output.size() - out_pos,
                                              &output_done);
      if (error != nullptr) {
        throw std::runtime_error(std::string("libsoxr streaming process failed: ") + error);
      }
      if (input_done == 0 && output_done == 0) {
        throw std::runtime_error("libsoxr streaming process stalled");
      }

      input += input_done;
      input_remaining -= input_done;
      out_pos += output_done;
    }
  }

  while (true) {
    EnsureResampleCapacity(output, out_pos);

    std::size_t output_done = 0;
    const soxr_error_t error = soxr_process(
        resampler.get(), nullptr, 0, nullptr, output.data() + out_pos, output.size() - out_pos, &output_done);
    if (error != nullptr) {
      throw std::runtime_error(std::string("libsoxr flush failed: ") + error);
    }

    out_pos += output_done;
    if (output_done == 0) {
      break;
    }
  }

  if (out_pos == 0) {
    throw std::runtime_error("resampler produced an empty waveform");
  }

  output.resize(out_pos);
  return output;
}

std::vector<float> PreprocessPipeline::PrepareMonoWaveform(const std::span<const float> samples,
                                                           const int input_sample_rate) {
  ValidateAudioBuffer(samples, input_sample_rate);
  const std::vector<double> mono = CopyMonoToDouble(samples);
  const std::vector<double> resampled =
      ResampleMonoWaveform(mono, input_sample_rate, Config().target_sample_rate);

  std::vector<float> output(resampled.size(), 0.0F);
  for (std::size_t idx = 0; idx < resampled.size(); ++idx) {
    output[idx] = static_cast<float>(resampled[idx]);
  }
  return output;
}

std::vector<float> PreprocessPipeline::ReflectPad(const std::span<const float> input) {
  const int pad = Config().pad;
  if (static_cast<std::size_t>(pad) >= input.size()) {
    throw std::invalid_argument("reflect padding requires input longer than pad");
  }

  std::vector<float> padded(input.size() + (2U * static_cast<std::size_t>(pad)), 0.0F);
  const std::size_t left_pad = static_cast<std::size_t>(pad);

  for (std::size_t idx = 0; idx < left_pad; ++idx) {
    padded[idx] = input[left_pad - idx];
  }
  std::copy(input.begin(), input.end(), padded.begin() + static_cast<std::ptrdiff_t>(left_pad));
  for (std::size_t idx = 0; idx < left_pad; ++idx) {
    padded[left_pad + input.size() + idx] = input[input.size() - 2U - idx];
  }

  return padded;
}

Spectrogram PreprocessPipeline::ComputeLogMelSpectrogram(
    const std::span<const float> mono_waveform,
    MelSpectrogramBackend& backend) {
  const std::vector<float> padded = ReflectPad(mono_waveform);
  const FrontendConfig& config = Config();
  if (padded.size() < static_cast<std::size_t>(config.nfft)) {
    throw std::runtime_error("padded waveform shorter than FFT size");
  }

  Spectrogram spectrogram = backend.ComputeLinearMel(padded, config);
  if (spectrogram.num_frames <= 0) {
    throw std::runtime_error("mel backend returned an empty spectrogram");
  }
  if (spectrogram.num_bins != config.num_mels) {
    throw std::runtime_error("mel backend returned unexpected mel bin count");
  }
  const std::size_t expected_values = static_cast<std::size_t>(spectrogram.num_frames) *
                                      static_cast<std::size_t>(spectrogram.num_bins);
  if (spectrogram.values.size() != expected_values) {
    throw std::runtime_error("mel backend returned spectrogram storage that does not match its shape");
  }

  for (float& value : spectrogram.values) {
    value = std::log1p(config.log_multiplier * std::max(value, 0.0F));
  }
  return spectrogram;
}

BeatThisPreprocessor::BeatThisPreprocessor(std::shared_ptr<MelSpectrogramBackend> backend)
    : backend_(std::move(backend)) {
  if (!backend_) {
    throw std::invalid_argument("mel spectrogram backend must not be null");
  }
}

// Mono float PCM input: 1 channel; input_sample_rate is in Hz.
Spectrogram BeatThisPreprocessor::Execute(const std::span<const float> samples,
                                          const int input_sample_rate) const {
  const std::vector<float> mono =
      PreprocessPipeline::PrepareMonoWaveform(samples, input_sample_rate);
  return PreprocessPipeline::ComputeLogMelSpectrogram(mono, *backend_);
}

}  // namespace beat_this::preprocess
