#pragma once

#include "util/ai/beat_this/BeatThis.hpp"

#include <cstddef>
#include <memory>
#include <span>
#include <vector>

namespace PDJE_UTIL::ai::beat_this {

struct Spectrogram {
    int                num_frames = 0;
    int                num_bins   = 128;
    std::vector<float> values;

    bool
    empty() const noexcept
    {
        return values.empty();
    }
};

class MelSpectrogramBackend {
  public:
    virtual ~MelSpectrogramBackend() = default;

    virtual Spectrogram
    ComputeLinearMel(std::span<const float>           padded_mono,
                     const BeatThisFrontendConfig &config) = 0;
};

class FrontendProcessor {
  public:
    static constexpr int kInputNumChannels = 1;

    explicit FrontendProcessor(std::shared_ptr<MelSpectrogramBackend> backend,
                               BeatThisFrontendConfig                 config = {});

    Spectrogram
    Execute(std::span<const float> samples, int input_sample_rate) const;

    const BeatThisFrontendConfig &
    config() const noexcept
    {
        return config_;
    }

  private:
    std::shared_ptr<MelSpectrogramBackend> backend_;
    BeatThisFrontendConfig                 config_;
};

class FrontendPipeline {
  public:
    static constexpr int kInputNumChannels = FrontendProcessor::kInputNumChannels;

    static std::vector<float>
    PrepareMonoWaveform(std::span<const float>           samples,
                        int                              input_sample_rate,
                        const BeatThisFrontendConfig &config = {});

    static Spectrogram
    ComputeLogMelSpectrogram(std::span<const float>           mono_waveform,
                             MelSpectrogramBackend           &backend,
                             const BeatThisFrontendConfig &config = {});

  private:
    static constexpr std::size_t kResampleBufferFloorFrames = 1024u;
    static constexpr std::size_t kFlushChunkFrames          = 1024u;

    static void
    ValidateAudioBuffer(std::span<const float> samples, int input_sample_rate);

    static std::vector<double>
    CopyMonoToDouble(std::span<const float> samples);

    static void
    EnsureResampleCapacity(std::vector<double> &output, std::size_t out_pos);

    static std::vector<double>
    ResampleMonoWaveform(std::span<const double> mono,
                         int                     input_sample_rate,
                         int                     output_sample_rate);

    static std::vector<float>
    ReflectPad(std::span<const float>           input,
               const BeatThisFrontendConfig &config);
};

} // namespace PDJE_UTIL::ai::beat_this
