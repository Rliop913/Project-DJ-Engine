#pragma once

#include "util/ai/beat_this/BeatThis.hpp"
#include "util/ai/beat_this/BeatThisInference.hpp"

#include <span>
#include <vector>

namespace PDJE_UTIL::ai::beat_this {

class MinimalBeatPostprocessor {
  public:
    explicit MinimalBeatPostprocessor(double fps = 50.0);

    BeatDetectionResult
    Process(const FrameLogits &logits) const;

    double
    fps() const noexcept
    {
        return fps_;
    }

  private:
    double fps_ = 50.0;
};

class PostprocessPipeline {
  public:
    static void
    ValidateFrameLogits(const FrameLogits &logits);

    static std::vector<int>
    CollectPeakFrames(std::span<const float> logits);

    static std::vector<double>
    DeduplicatePeaks(std::span<const int> peaks);

    static std::vector<double>
    ConvertFramesToSeconds(std::span<const double> frame_positions,
                           double                  fps);

    static std::vector<double>
    SnapDownbeatsToNearestBeats(std::span<const double> beat_times,
                                std::span<const double> downbeat_times);

  private:
    static constexpr int kPeakWindowSize   = 7;
    static constexpr int kPeakRadius       = kPeakWindowSize / 2;
    static constexpr int kDeduplicateWidth = 1;

    static bool
    IsLocalMaximum(std::span<const float> values, int index);
};

} // namespace PDJE_UTIL::ai::beat_this
