#include "util/ai/beat_this/BeatThisPostprocessor.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>

namespace PDJE_UTIL::ai::beat_this {

void
PostprocessPipeline::ValidateFrameLogits(const FrameLogits &logits)
{
    if (logits.num_frames < 0) {
        throw std::invalid_argument(
            "frame logits num_frames must not be negative");
    }

    const std::size_t expectedSize =
        static_cast<std::size_t>(logits.num_frames);
    if (logits.beat.size() != expectedSize) {
        throw std::invalid_argument(
            "frame logits beat size does not match num_frames");
    }
    if (logits.downbeat.size() != expectedSize) {
        throw std::invalid_argument(
            "frame logits downbeat size does not match num_frames");
    }
}

bool
PostprocessPipeline::IsLocalMaximum(const std::span<const float> values,
                                    const int                     index)
{
    const int start = std::max(0, index - kPeakRadius);
    const int end =
        std::min(static_cast<int>(values.size()) - 1, index + kPeakRadius);

    float localMax = -std::numeric_limits<float>::infinity();
    for (int cursor = start; cursor <= end; ++cursor) {
        localMax = std::max(localMax, values[static_cast<std::size_t>(cursor)]);
    }

    return values[static_cast<std::size_t>(index)] == localMax;
}

std::vector<int>
PostprocessPipeline::CollectPeakFrames(const std::span<const float> logits)
{
    std::vector<int> peaks;
    peaks.reserve(logits.size());

    for (int frame = 0; frame < static_cast<int>(logits.size()); ++frame) {
        const float value = logits[static_cast<std::size_t>(frame)];
        if (value <= 0.0f) {
            continue;
        }
        if (IsLocalMaximum(logits, frame)) {
            peaks.push_back(frame);
        }
    }

    return peaks;
}

std::vector<double>
PostprocessPipeline::DeduplicatePeaks(const std::span<const int> peaks)
{
    std::vector<double> deduplicated;
    if (peaks.empty()) {
        return deduplicated;
    }

    deduplicated.reserve(peaks.size());
    double mean  = static_cast<double>(peaks.front());
    int    count = 1;

    for (std::size_t index = 1; index < peaks.size(); ++index) {
        const int current = peaks[index];
        if ((static_cast<double>(current) - mean) <=
            static_cast<double>(kDeduplicateWidth)) {
            ++count;
            mean += (static_cast<double>(current) - mean) /
                    static_cast<double>(count);
            continue;
        }

        deduplicated.push_back(mean);
        mean  = static_cast<double>(current);
        count = 1;
    }

    deduplicated.push_back(mean);
    return deduplicated;
}

std::vector<double>
PostprocessPipeline::ConvertFramesToSeconds(
    const std::span<const double> frame_positions,
    const double                  fps)
{
    std::vector<double> times;
    times.reserve(frame_positions.size());

    for (const double frame : frame_positions) {
        times.push_back(frame / fps);
    }

    return times;
}

std::vector<double>
PostprocessPipeline::SnapDownbeatsToNearestBeats(
    const std::span<const double> beat_times,
    const std::span<const double> downbeat_times)
{
    std::vector<double> snapped(downbeat_times.begin(), downbeat_times.end());
    if (beat_times.empty()) {
        std::sort(snapped.begin(), snapped.end());
        snapped.erase(std::unique(snapped.begin(), snapped.end()),
                      snapped.end());
        return snapped;
    }

    for (double &downbeat : snapped) {
        const auto upper =
            std::lower_bound(beat_times.begin(), beat_times.end(), downbeat);
        if (upper == beat_times.begin()) {
            downbeat = *upper;
            continue;
        }
        if (upper == beat_times.end()) {
            downbeat = beat_times.back();
            continue;
        }

        const double previous = *(upper - 1);
        const double next     = *upper;
        downbeat =
            (std::abs(downbeat - previous) <= std::abs(next - downbeat))
                ? previous
                : next;
    }

    std::sort(snapped.begin(), snapped.end());
    snapped.erase(std::unique(snapped.begin(), snapped.end()), snapped.end());
    return snapped;
}

MinimalBeatPostprocessor::MinimalBeatPostprocessor(const double fps)
    : fps_(fps)
{
    if (!std::isfinite(fps_) || fps_ <= 0.0) {
        throw std::invalid_argument(
            "minimal beat postprocessor fps must be positive and finite");
    }
}

BeatDetectionResult
MinimalBeatPostprocessor::Process(const FrameLogits &logits) const
{
    PostprocessPipeline::ValidateFrameLogits(logits);

    const std::vector<int> beatPeaks =
        PostprocessPipeline::CollectPeakFrames(logits.beat);
    const std::vector<int> downbeatPeaks =
        PostprocessPipeline::CollectPeakFrames(logits.downbeat);

    const std::vector<double> beatFrames =
        PostprocessPipeline::DeduplicatePeaks(beatPeaks);
    const std::vector<double> downbeatFrames =
        PostprocessPipeline::DeduplicatePeaks(downbeatPeaks);

    const std::vector<double> beatTimes =
        PostprocessPipeline::ConvertFramesToSeconds(beatFrames, fps_);
    const std::vector<double> downbeatTimes =
        PostprocessPipeline::ConvertFramesToSeconds(downbeatFrames, fps_);

    return BeatDetectionResult{
        .beats = beatTimes,
        .downbeats =
            PostprocessPipeline::SnapDownbeatsToNearestBeats(beatTimes,
                                                             downbeatTimes),
    };
}

} // namespace PDJE_UTIL::ai::beat_this
