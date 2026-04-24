#include "postprocessor.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <limits>
#include <span>
#include <stdexcept>
#include <utility>
#include <vector>

namespace beat_this::inference {

void PostprocessPipeline::ValidateFrameLogits(const FrameLogits& logits) {
  if (logits.num_frames < 0) {
    throw std::invalid_argument("frame logits num_frames must not be negative");
  }

  const std::size_t expected_size = static_cast<std::size_t>(logits.num_frames);
  if (logits.beat.size() != expected_size) {
    throw std::invalid_argument("frame logits beat size does not match num_frames");
  }
  if (logits.downbeat.size() != expected_size) {
    throw std::invalid_argument("frame logits downbeat size does not match num_frames");
  }
}

bool PostprocessPipeline::IsLocalMaximum(const std::span<const float> values,
                                         const int index) {
  const int start = std::max(0, index - kPeakRadius);
  const int end = std::min(static_cast<int>(values.size()) - 1, index + kPeakRadius);

  float local_max = -std::numeric_limits<float>::infinity();
  for (int cursor = start; cursor <= end; ++cursor) {
    local_max = std::max(local_max, values[static_cast<std::size_t>(cursor)]);
  }
  return values[static_cast<std::size_t>(index)] == local_max;
}

std::vector<int> PostprocessPipeline::CollectPeakFrames(const std::span<const float> logits) {
  std::vector<int> peaks;
  peaks.reserve(logits.size());

  for (int frame = 0; frame < static_cast<int>(logits.size()); ++frame) {
    const float value = logits[static_cast<std::size_t>(frame)];
    if (value <= 0.0F) {
      continue;
    }
    if (IsLocalMaximum(logits, frame)) {
      peaks.push_back(frame);
    }
  }
  return peaks;
}

std::vector<double> PostprocessPipeline::DeduplicatePeaks(
    const std::span<const int> peaks) {
  std::vector<double> deduplicated;
  if (peaks.empty()) {
    return deduplicated;
  }

  deduplicated.reserve(peaks.size());
  double mean = static_cast<double>(peaks.front());
  int count = 1;

  for (std::size_t index = 1; index < peaks.size(); ++index) {
    const int current = peaks[index];
    if ((static_cast<double>(current) - mean) <= static_cast<double>(kDeduplicateWidth)) {
      ++count;
      mean += (static_cast<double>(current) - mean) / static_cast<double>(count);
      continue;
    }

    deduplicated.push_back(mean);
    mean = static_cast<double>(current);
    count = 1;
  }

  deduplicated.push_back(mean);
  return deduplicated;
}

std::vector<double> PostprocessPipeline::ConvertFramesToSeconds(
    const std::span<const double> frame_positions,
    const double fps) {
  std::vector<double> times;
  times.reserve(frame_positions.size());
  for (const double frame : frame_positions) {
    times.push_back(frame / fps);
  }
  return times;
}

std::vector<double> PostprocessPipeline::SnapDownbeatsToNearestBeats(
    const std::span<const double> beat_times,
    const std::span<const double> downbeat_times) {
  std::vector<double> snapped(downbeat_times.begin(), downbeat_times.end());
  if (beat_times.empty()) {
    std::sort(snapped.begin(), snapped.end());
    snapped.erase(std::unique(snapped.begin(), snapped.end()), snapped.end());
    return snapped;
  }

  for (double& downbeat : snapped) {
    const auto upper = std::lower_bound(beat_times.begin(), beat_times.end(), downbeat);
    if (upper == beat_times.begin()) {
      downbeat = *upper;
      continue;
    }
    if (upper == beat_times.end()) {
      downbeat = beat_times.back();
      continue;
    }

    const double previous = *(upper - 1);
    const double next = *upper;
    downbeat = (std::abs(downbeat - previous) <= std::abs(next - downbeat)) ? previous : next;
  }

  std::sort(snapped.begin(), snapped.end());
  snapped.erase(std::unique(snapped.begin(), snapped.end()), snapped.end());
  return snapped;
}

MinimalBeatPostprocessor::MinimalBeatPostprocessor(const double fps) : fps_(fps) {
  if (!std::isfinite(fps_) || fps_ <= 0.0) {
    throw std::invalid_argument("minimal beat postprocessor fps must be positive and finite");
  }
}

BeatTimestamps MinimalBeatPostprocessor::Process(const FrameLogits& logits) const {
  PostprocessPipeline::ValidateFrameLogits(logits);

  const std::vector<int> beat_peaks = PostprocessPipeline::CollectPeakFrames(logits.beat);
  const std::vector<int> downbeat_peaks = PostprocessPipeline::CollectPeakFrames(logits.downbeat);

  const std::vector<double> beat_frames = PostprocessPipeline::DeduplicatePeaks(beat_peaks);
  const std::vector<double> downbeat_frames = PostprocessPipeline::DeduplicatePeaks(downbeat_peaks);

  const std::vector<double> beat_times = PostprocessPipeline::ConvertFramesToSeconds(beat_frames, fps_);
  const std::vector<double> downbeat_times =
      PostprocessPipeline::ConvertFramesToSeconds(downbeat_frames, fps_);

  return BeatTimestamps{
      .beats = beat_times,
      .downbeats = PostprocessPipeline::SnapDownbeatsToNearestBeats(beat_times, downbeat_times),
  };
}

BeatThisPipeline::BeatThisPipeline(
    std::filesystem::path model_path,
    std::shared_ptr<beat_this::preprocess::MelSpectrogramBackend> backend,
    const double fps)
    : runner_(std::move(model_path), std::move(backend)),
      postprocessor_(fps) {}

BeatTimestamps BeatThisPipeline::Execute(const std::span<const float> samples,
                                         const int input_sample_rate) const {
  return postprocessor_.Process(runner_.Execute(samples, input_sample_rate));
}

}  // namespace beat_this::inference
