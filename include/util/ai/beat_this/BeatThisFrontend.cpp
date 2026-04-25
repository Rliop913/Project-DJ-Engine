#include "util/ai/beat_this/BeatThisFrontend.hpp"

#include "soxr.h"

#include <algorithm>
#include <cmath>
#include <memory>
#include <span>
#include <stdexcept>
#include <string>
#include <utility>

namespace PDJE_UTIL::ai::beat_this {

void
FrontendPipeline::ValidateAudioBuffer(const std::span<const float> samples,
                                      const int                     input_sample_rate)
{
    if (input_sample_rate <= 0) {
        throw std::invalid_argument("input_sample_rate must be positive");
    }
    if (samples.empty()) {
        throw std::invalid_argument("audio buffer must be non-empty");
    }
}

std::vector<double>
FrontendPipeline::CopyMonoToDouble(const std::span<const float> samples)
{
    std::vector<double> mono(samples.size(), 0.0);
    for (std::size_t idx = 0; idx < samples.size(); ++idx) {
        mono[idx] = static_cast<double>(samples[idx]);
    }

    return mono;
}

void
FrontendPipeline::EnsureResampleCapacity(std::vector<double> &output,
                                         const std::size_t     out_pos)
{
    if (out_pos < output.size()) {
        return;
    }

    const std::size_t nextSize =
        std::max(output.size() * 2u, out_pos + kFlushChunkFrames);
    output.resize(nextSize, 0.0);
}

std::vector<double>
FrontendPipeline::ResampleMonoWaveform(const std::span<const double> mono,
                                       const int                     input_sample_rate,
                                       const int                     output_sample_rate)
{
    if (input_sample_rate == output_sample_rate) {
        return std::vector<double>(mono.begin(), mono.end());
    }

    const double ratio =
        static_cast<double>(output_sample_rate) /
        static_cast<double>(input_sample_rate);
    const std::size_t chunkFrames = static_cast<std::size_t>(
        std::max(1000.0,
                 48000.0 * static_cast<double>(input_sample_rate) /
                     static_cast<double>(output_sample_rate)));
    const soxr_io_spec_t ioSpec =
        soxr_io_spec(SOXR_FLOAT64_I, SOXR_FLOAT64_I);
    const soxr_quality_spec_t qualitySpec = soxr_quality_spec(SOXR_HQ, 0);
    soxr_error_t              createError = nullptr;
    soxr_t rawResampler = soxr_create(static_cast<double>(input_sample_rate),
                                      static_cast<double>(output_sample_rate),
                                      1,
                                      &createError,
                                      &ioSpec,
                                      &qualitySpec,
                                      nullptr);
    if (createError != nullptr || rawResampler == nullptr) {
        throw std::runtime_error(
            std::string("libsoxr streaming create failed: ") +
            (createError != nullptr ? createError : "unknown error"));
    }

    std::unique_ptr<soxr, decltype(&soxr_delete)> resampler(rawResampler,
                                                            &soxr_delete);
    const double expectedFrames =
        soxr_delay(resampler.get()) + (static_cast<double>(mono.size()) * ratio) +
        1.0;
    const std::size_t initialCapacity = std::max(
        kResampleBufferFloorFrames,
        static_cast<std::size_t>(std::ceil(std::max(1.0, expectedFrames))));

    std::vector<double> output(initialCapacity, 0.0);
    std::size_t         outPos = 0;

    for (std::size_t idx = 0; idx < mono.size(); idx += chunkFrames) {
        const double *input = mono.data() + idx;
        std::size_t   inputRemaining =
            std::min(chunkFrames, mono.size() - idx);

        while (inputRemaining > 0) {
            EnsureResampleCapacity(output, outPos);

            std::size_t inputDone  = 0;
            std::size_t outputDone = 0;
            const soxr_error_t error =
                soxr_process(resampler.get(),
                             input,
                             inputRemaining,
                             &inputDone,
                             output.data() + outPos,
                             output.size() - outPos,
                             &outputDone);
            if (error != nullptr) {
                throw std::runtime_error(
                    std::string("libsoxr streaming process failed: ") + error);
            }
            if (inputDone == 0 && outputDone == 0) {
                throw std::runtime_error(
                    "libsoxr streaming process stalled");
            }

            input += inputDone;
            inputRemaining -= inputDone;
            outPos += outputDone;
        }
    }

    while (true) {
        EnsureResampleCapacity(output, outPos);

        std::size_t outputDone = 0;
        const soxr_error_t error =
            soxr_process(resampler.get(),
                         nullptr,
                         0,
                         nullptr,
                         output.data() + outPos,
                         output.size() - outPos,
                         &outputDone);
        if (error != nullptr) {
            throw std::runtime_error(
                std::string("libsoxr flush failed: ") + error);
        }

        outPos += outputDone;
        if (outputDone == 0) {
            break;
        }
    }

    if (outPos == 0) {
        throw std::runtime_error("resampler produced an empty waveform");
    }

    output.resize(outPos);
    return output;
}

std::vector<float>
FrontendPipeline::PrepareMonoWaveform(const std::span<const float> samples,
                                      const int                     input_sample_rate,
                                      const BeatThisFrontendConfig &config)
{
    ValidateAudioBuffer(samples, input_sample_rate);
    const std::vector<double> mono = CopyMonoToDouble(samples);
    const std::vector<double> resampled =
        ResampleMonoWaveform(mono,
                             input_sample_rate,
                             config.target_sample_rate);

    std::vector<float> output(resampled.size(), 0.0f);
    for (std::size_t idx = 0; idx < resampled.size(); ++idx) {
        output[idx] = static_cast<float>(resampled[idx]);
    }

    return output;
}

std::vector<float>
FrontendPipeline::ReflectPad(const std::span<const float> input,
                             const BeatThisFrontendConfig &config)
{
    const int pad = config.pad;
    if (static_cast<std::size_t>(pad) >= input.size()) {
        throw std::invalid_argument(
            "reflect padding requires input longer than pad");
    }

    std::vector<float> padded(
        input.size() + (2u * static_cast<std::size_t>(pad)), 0.0f);
    const std::size_t leftPad = static_cast<std::size_t>(pad);

    for (std::size_t idx = 0; idx < leftPad; ++idx) {
        padded[idx] = input[leftPad - idx];
    }

    std::copy(input.begin(),
              input.end(),
              padded.begin() + static_cast<std::ptrdiff_t>(leftPad));
    for (std::size_t idx = 0; idx < leftPad; ++idx) {
        padded[leftPad + input.size() + idx] =
            input[input.size() - 2u - idx];
    }

    return padded;
}

Spectrogram
FrontendPipeline::ComputeLogMelSpectrogram(
    const std::span<const float> samples,
    MelSpectrogramBackend       &backend,
    const BeatThisFrontendConfig &config)
{
    const std::vector<float> padded = ReflectPad(samples, config);
    if (padded.size() < static_cast<std::size_t>(config.nfft)) {
        throw std::runtime_error("padded waveform shorter than FFT size");
    }

    Spectrogram spectrogram = backend.ComputeLinearMel(padded, config);
    if (spectrogram.num_frames <= 0) {
        throw std::runtime_error(
            "mel backend returned an empty spectrogram");
    }
    if (spectrogram.num_bins != config.num_mels) {
        throw std::runtime_error(
            "mel backend returned unexpected mel bin count");
    }

    const std::size_t expectedValues =
        static_cast<std::size_t>(spectrogram.num_frames) *
        static_cast<std::size_t>(spectrogram.num_bins);
    if (spectrogram.values.size() != expectedValues) {
        throw std::runtime_error(
            "mel backend returned spectrogram storage that does not match its shape");
    }

    for (float &value : spectrogram.values) {
        value = std::log1p(config.log_multiplier * std::max(value, 0.0f));
    }

    return spectrogram;
}

FrontendProcessor::FrontendProcessor(
    std::shared_ptr<MelSpectrogramBackend> backend,
    BeatThisFrontendConfig                 config)
    : backend_(std::move(backend)),
      config_(std::move(config))
{
    if (!backend_) {
        throw std::invalid_argument(
            "mel spectrogram backend must not be null");
    }
}

Spectrogram
FrontendProcessor::Execute(const std::span<const float> samples,
                           const int                     input_sample_rate) const
{
    const std::vector<float> mono =
        FrontendPipeline::PrepareMonoWaveform(samples,
                                              input_sample_rate,
                                              config_);
    return FrontendPipeline::ComputeLogMelSpectrogram(mono,
                                                      *backend_,
                                                      config_);
}

} // namespace PDJE_UTIL::ai::beat_this
