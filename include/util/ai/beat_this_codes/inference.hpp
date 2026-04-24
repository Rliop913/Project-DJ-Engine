#pragma once

#include "preprocessor.hpp"

#include <cstddef>
#include <filesystem>
#include <memory>
#include <span>
#include <string>
#include <vector>

namespace Ort {
struct AllocatorWithDefaultOptions;
struct Env;
struct Session;
}

namespace beat_this::inference {

struct FrameLogits {
  int num_frames = 0;
  std::vector<float> beat;
  std::vector<float> downbeat;

  [[nodiscard]] bool empty() const noexcept { return num_frames == 0; }
};

class BeatThisOnnxRunner {
 public:
  BeatThisOnnxRunner(std::filesystem::path model_path,
                     std::shared_ptr<beat_this::preprocess::MelSpectrogramBackend> backend);
  ~BeatThisOnnxRunner();

  BeatThisOnnxRunner(BeatThisOnnxRunner&&) noexcept;
  BeatThisOnnxRunner& operator=(BeatThisOnnxRunner&&) noexcept;

  BeatThisOnnxRunner(const BeatThisOnnxRunner&) = delete;
  BeatThisOnnxRunner& operator=(const BeatThisOnnxRunner&) = delete;

  static constexpr int kInputNumChannels = beat_this::preprocess::BeatThisPreprocessor::kInputNumChannels;

  // Mono float PCM input: 1 channel; input_sample_rate is in Hz.
  [[nodiscard]] FrameLogits Execute(std::span<const float> samples, int input_sample_rate) const;

 private:
  std::filesystem::path model_path_;
  beat_this::preprocess::BeatThisPreprocessor preprocessor_;
  std::unique_ptr<Ort::Session> session_;
  std::string input_name_;
  std::string output_name_;
};

struct SpectrogramChunk {
  int start_frame = 0;
  int num_frames = 0;
  int num_bins = 0;
  std::vector<float> values;
};

struct ChunkFrameLogits {
  int start_frame = 0;
  FrameLogits logits;
};

class InferencePipeline {
 public:
  [[nodiscard]] static Ort::Env& OrtEnv();
  [[nodiscard]] static std::string ReadIoName(Ort::Session& session,
                                              bool input,
                                              std::size_t index,
                                              Ort::AllocatorWithDefaultOptions& allocator);
  static void RequireIoName(const std::string& actual, const char* expected, const char* kind);
  [[nodiscard]] static FrameLogits RunFullSpectrogram(
      Ort::Session& session,
      const std::string& input_name,
      const std::string& output_name,
      const beat_this::preprocess::Spectrogram& spectrogram);

 private:
  static constexpr int kNumMels = 128;
  static constexpr int kChunkSize = 1500;
  static constexpr int kChunkBorderSize = 6;
  static constexpr int kModelOutputChannels = 2;
  static constexpr float kPaddingLogit = -1000.0F;

  static void ValidateSpectrogram(const beat_this::preprocess::Spectrogram& spectrogram);
  [[nodiscard]] static std::vector<int> ComputeChunkStarts(int num_frames);
  [[nodiscard]] static std::vector<SpectrogramChunk> SplitSpectrogram(
      const beat_this::preprocess::Spectrogram& spectrogram);
  [[nodiscard]] static FrameLogits RunSpectrogramChunk(Ort::Session& session,
                                                       const std::string& input_name,
                                                       const std::string& output_name,
                                                       std::span<const float> values,
                                                       int num_frames,
                                                       int num_bins);
  [[nodiscard]] static FrameLogits AggregateChunkLogits(std::span<const ChunkFrameLogits> chunks,
                                                        int full_num_frames);
};

}  // namespace beat_this::inference
