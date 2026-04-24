#pragma once

#include <cstddef>
#include <memory>
#include <span>
#include <vector>

namespace beat_this::preprocess {

struct Spectrogram {
  int num_frames = 0;
  int num_bins = 128;
  std::vector<float> values;

  [[nodiscard]] bool empty() const noexcept { return values.empty(); }
};

struct FrontendConfig {
  int target_sample_rate = 22050;
  int nfft = 1024;
  int hop_length = 441;
  int num_mels = 128;
  int pad = 512;
  float f_min_hz = 30.0F;
  float f_max_hz = 11000.0F;
  float log_multiplier = 1000.0F;
};

class MelSpectrogramBackend {
 public:
  virtual ~MelSpectrogramBackend() = default;

  [[nodiscard]] virtual Spectrogram ComputeLinearMel(std::span<const float> padded_mono,
                                                     const FrontendConfig& config) = 0;
};

class BeatThisPreprocessor {
 public:
  static constexpr int kInputNumChannels = 1;

  explicit BeatThisPreprocessor(std::shared_ptr<MelSpectrogramBackend> backend);

  // Mono float PCM input: 1 channel; input_sample_rate is in Hz.
  [[nodiscard]] Spectrogram Execute(std::span<const float> samples, int input_sample_rate) const;

 private:
  std::shared_ptr<MelSpectrogramBackend> backend_;
};

class PreprocessPipeline {
 public:
  static constexpr int kInputNumChannels = BeatThisPreprocessor::kInputNumChannels;

  // Mono float PCM input: 1 channel; input_sample_rate is in Hz.
  [[nodiscard]] static std::vector<float> PrepareMonoWaveform(std::span<const float> samples,
                                                              int input_sample_rate);
  [[nodiscard]] static Spectrogram ComputeLogMelSpectrogram(std::span<const float> mono_waveform,
                                                            MelSpectrogramBackend& backend);
  [[nodiscard]] static const FrontendConfig& Config() noexcept;

 private:
  static constexpr std::size_t kResampleBufferFloorFrames = 1024U;
  static constexpr std::size_t kFlushChunkFrames = 1024U;

  // Mono float PCM input: 1 channel; input_sample_rate is in Hz.
  static void ValidateAudioBuffer(std::span<const float> samples, int input_sample_rate);
  [[nodiscard]] static std::vector<double> CopyMonoToDouble(std::span<const float> samples);
  static void EnsureResampleCapacity(std::vector<double>& output, std::size_t out_pos);
  [[nodiscard]] static std::vector<double> ResampleMonoWaveform(std::span<const double> mono,
                                                                int input_sample_rate,
                                                                int output_sample_rate);
  [[nodiscard]] static std::vector<float> ReflectPad(std::span<const float> input);
};

}  // namespace beat_this::preprocess
