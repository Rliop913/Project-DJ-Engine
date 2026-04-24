#pragma once

#include "inference.hpp"

#include <filesystem>
#include <memory>
#include <span>
#include <vector>

namespace beat_this::inference {

struct BeatTimestamps {
  std::vector<double> beats;
  std::vector<double> downbeats;

  [[nodiscard]] bool empty() const noexcept { return beats.empty() && downbeats.empty(); }
};

class MinimalBeatPostprocessor {
 public:
  explicit MinimalBeatPostprocessor(double fps = 50.0);

  [[nodiscard]] BeatTimestamps Process(const FrameLogits& logits) const;
  [[nodiscard]] double fps() const noexcept { return fps_; }

 private:
  double fps_ = 50.0;
};

class PostprocessPipeline {
 public:
  static void ValidateFrameLogits(const FrameLogits& logits);
  [[nodiscard]] static std::vector<int> CollectPeakFrames(std::span<const float> logits);
  [[nodiscard]] static std::vector<double> DeduplicatePeaks(std::span<const int> peaks);
  [[nodiscard]] static std::vector<double> ConvertFramesToSeconds(std::span<const double> frame_positions,
                                                                  double fps);
  [[nodiscard]] static std::vector<double> SnapDownbeatsToNearestBeats(
      std::span<const double> beat_times,
      std::span<const double> downbeat_times);

 private:
  static constexpr int kPeakWindowSize = 7;
  static constexpr int kPeakRadius = kPeakWindowSize / 2;
  static constexpr int kDeduplicateWidth = 1;

  [[nodiscard]] static bool IsLocalMaximum(std::span<const float> values, int index);
};

class BeatThisPipeline {
 public:
  BeatThisPipeline(std::filesystem::path model_path,
                   std::shared_ptr<beat_this::preprocess::MelSpectrogramBackend> backend,
                   double fps = 50.0);

  // Mono float PCM input: 1 channel; input_sample_rate is in Hz.
  [[nodiscard]] BeatTimestamps Execute(std::span<const float> samples, int input_sample_rate) const;

 private:
  BeatThisOnnxRunner runner_;
  MinimalBeatPostprocessor postprocessor_;
};

}  // namespace beat_this::inference
