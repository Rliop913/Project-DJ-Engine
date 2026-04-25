#pragma once

#include "util/ai/beat_this/BeatThisFrontend.hpp"

#include <span>
#include <vector>

namespace PDJE_UTIL::ai {
class OnnxSession;
}

namespace PDJE_UTIL::ai::beat_this {

struct FrameLogits {
    int                num_frames = 0;
    std::vector<float> beat;
    std::vector<float> downbeat;

    bool
    empty() const noexcept
    {
        return num_frames == 0;
    }
};

struct SpectrogramChunk {
    int                start_frame = 0;
    int                num_frames  = 0;
    int                num_bins    = 0;
    std::vector<float> values;
};

struct ChunkFrameLogits {
    int         start_frame = 0;
    FrameLogits logits;
};

class InferencePipeline {
  public:
    static void
    ValidateSession(const OnnxSession &session);

    static FrameLogits
    RunFullSpectrogram(const OnnxSession &session,
                       const Spectrogram &spectrogram);

    static std::vector<int>
    ComputeChunkStarts(int num_frames);

    static std::vector<SpectrogramChunk>
    SplitSpectrogram(const Spectrogram &spectrogram);

    static FrameLogits
    AggregateChunkLogits(std::span<const ChunkFrameLogits> chunks,
                         int                               full_num_frames);

  private:
    static constexpr int   kNumMels             = 128;
    static constexpr int   kChunkSize           = 1500;
    static constexpr int   kChunkBorderSize     = 6;
    static constexpr int   kModelOutputChannels = 2;
    static constexpr float kPaddingLogit        = -1000.0f;

    static void
    ValidateSpectrogram(const Spectrogram &spectrogram);

    static FrameLogits
    RunSpectrogramChunk(const OnnxSession       &session,
                        std::span<const float>   values,
                        int                      num_frames,
                        int                      num_bins);
};

} // namespace PDJE_UTIL::ai::beat_this
