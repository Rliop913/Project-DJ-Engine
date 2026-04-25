#include "util/ai/beat_this/BeatThisInference.hpp"

#include "util/ai/AI.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <vector>

namespace PDJE_UTIL::ai::beat_this {

void
InferencePipeline::ValidateSession(const OnnxSession &session)
{
    if (session.input_count() != 1u) {
        throw std::runtime_error(
            "onnx model must expose exactly one input");
    }
    if (session.output_count() != 1u) {
        throw std::runtime_error(
            "onnx model must expose exactly one output");
    }
    if (session.input_name(0u) != "spect") {
        throw std::runtime_error(
            "unexpected onnx model input name: expected 'spect', got '" +
            session.input_name(0u) + "'");
    }
    if (session.output_name(0u) != "logits") {
        throw std::runtime_error(
            "unexpected onnx model output name: expected 'logits', got '" +
            session.output_name(0u) + "'");
    }
}

void
InferencePipeline::ValidateSpectrogram(const Spectrogram &spectrogram)
{
    if (spectrogram.num_frames <= 0) {
        throw std::invalid_argument(
            "spectrogram must contain at least one frame");
    }
    if (spectrogram.num_bins != kNumMels) {
        throw std::invalid_argument(
            "spectrogram must contain 128 mel bins");
    }

    const std::size_t expectedValues =
        static_cast<std::size_t>(spectrogram.num_frames) *
        static_cast<std::size_t>(spectrogram.num_bins);
    if (spectrogram.values.size() != expectedValues) {
        throw std::invalid_argument(
            "spectrogram flattened storage size does not match its shape");
    }
}

std::vector<int>
InferencePipeline::ComputeChunkStarts(const int num_frames)
{
    const int step = kChunkSize - (2 * kChunkBorderSize);
    std::vector<int> starts;

    for (int start = -kChunkBorderSize; start < num_frames - kChunkBorderSize;
         start += step) {
        starts.push_back(start);
    }
    if (num_frames > step && !starts.empty()) {
        starts.back() = num_frames - (kChunkSize - kChunkBorderSize);
    }

    return starts;
}

std::vector<SpectrogramChunk>
InferencePipeline::SplitSpectrogram(const Spectrogram &spectrogram)
{
    const std::vector<int> starts = ComputeChunkStarts(spectrogram.num_frames);
    std::vector<SpectrogramChunk> chunks;
    chunks.reserve(starts.size());

    for (const int start : starts) {
        const int contentBegin = std::max(start, 0);
        const int contentEnd =
            std::min(start + kChunkSize, spectrogram.num_frames);
        const int leftPad  = std::max(0, -start);
        const int rightPad = std::max(
            0,
            std::min(kChunkBorderSize,
                     start + kChunkSize - spectrogram.num_frames));
        const int chunkNumFrames =
            leftPad + (contentEnd - contentBegin) + rightPad;

        SpectrogramChunk chunk{
            .start_frame = start,
            .num_frames = chunkNumFrames,
            .num_bins = spectrogram.num_bins,
            .values = std::vector<float>(
                static_cast<std::size_t>(chunkNumFrames) *
                    static_cast<std::size_t>(spectrogram.num_bins),
                0.0f),
        };

        const std::size_t sourceOffset =
            static_cast<std::size_t>(contentBegin) *
            static_cast<std::size_t>(spectrogram.num_bins);
        const std::size_t copyCount =
            static_cast<std::size_t>(contentEnd - contentBegin) *
            static_cast<std::size_t>(spectrogram.num_bins);
        const std::size_t destinationOffset =
            static_cast<std::size_t>(leftPad) *
            static_cast<std::size_t>(spectrogram.num_bins);

        std::copy_n(
            spectrogram.values.begin() +
                static_cast<std::ptrdiff_t>(sourceOffset),
            static_cast<std::ptrdiff_t>(copyCount),
            chunk.values.begin() +
                static_cast<std::ptrdiff_t>(destinationOffset));

        chunks.push_back(std::move(chunk));
    }

    return chunks;
}

FrameLogits
InferencePipeline::RunSpectrogramChunk(const OnnxSession       &session,
                                       const std::span<const float> values,
                                       const int                num_frames,
                                       const int                num_bins)
{
    std::array<NamedFloatTensor, 1> inputs;
    inputs[0].name         = session.input_name(0u);
    inputs[0].tensor.shape = { 1, static_cast<int64_t>(num_frames),
                               static_cast<int64_t>(num_bins) };
    inputs[0].tensor.values.assign(values.begin(), values.end());

    std::vector<std::string> requestedOutputs{ session.output_name(0u) };
    const std::vector<NamedFloatTensor> outputs =
        session.run(inputs, requestedOutputs);
    if (outputs.size() != 1u) {
        throw std::runtime_error(
            "onnx runtime did not return the expected tensor output");
    }

    const FloatTensor &outputTensor = outputs[0].tensor;
    if (outputTensor.shape.size() != 3u || outputTensor.shape[0] != 1 ||
        outputTensor.shape[1] != static_cast<int64_t>(num_frames) ||
        outputTensor.shape[2] !=
            static_cast<int64_t>(kModelOutputChannels)) {
        throw std::runtime_error(
            "onnx model returned an unexpected logits shape");
    }

    const std::size_t expectedValues =
        static_cast<std::size_t>(num_frames) *
        static_cast<std::size_t>(kModelOutputChannels);
    if (outputTensor.values.size() != expectedValues) {
        throw std::runtime_error(
            "onnx model returned an unexpected logits element count");
    }

    FrameLogits logits{
        .num_frames = num_frames,
        .beat = std::vector<float>(static_cast<std::size_t>(num_frames), 0.0f),
        .downbeat =
            std::vector<float>(static_cast<std::size_t>(num_frames), 0.0f),
    };

    for (int frame = 0; frame < num_frames; ++frame) {
        const std::size_t offset =
            static_cast<std::size_t>(frame) *
            static_cast<std::size_t>(kModelOutputChannels);
        logits.beat[static_cast<std::size_t>(frame)] =
            outputTensor.values[offset];
        logits.downbeat[static_cast<std::size_t>(frame)] =
            outputTensor.values[offset + 1u];
    }

    return logits;
}

FrameLogits
InferencePipeline::AggregateChunkLogits(
    const std::span<const ChunkFrameLogits> chunks,
    const int                               full_num_frames)
{
    FrameLogits result{
        .num_frames = full_num_frames,
        .beat =
            std::vector<float>(static_cast<std::size_t>(full_num_frames),
                               kPaddingLogit),
        .downbeat =
            std::vector<float>(static_cast<std::size_t>(full_num_frames),
                               kPaddingLogit),
    };

    for (auto chunkIt = chunks.rbegin(); chunkIt != chunks.rend(); ++chunkIt) {
        const int targetBegin =
            std::max(0, chunkIt->start_frame + kChunkBorderSize);
        const int targetEnd = std::min(
            full_num_frames,
            chunkIt->start_frame + kChunkSize - kChunkBorderSize);
        if (targetEnd <= targetBegin) {
            continue;
        }

        const std::size_t copyCount =
            static_cast<std::size_t>(targetEnd - targetBegin);
        const std::size_t sourceOffset =
            static_cast<std::size_t>(kChunkBorderSize);
        std::copy_n(chunkIt->logits.beat.begin() +
                        static_cast<std::ptrdiff_t>(sourceOffset),
                    static_cast<std::ptrdiff_t>(copyCount),
                    result.beat.begin() +
                        static_cast<std::ptrdiff_t>(targetBegin));
        std::copy_n(chunkIt->logits.downbeat.begin() +
                        static_cast<std::ptrdiff_t>(sourceOffset),
                    static_cast<std::ptrdiff_t>(copyCount),
                    result.downbeat.begin() +
                        static_cast<std::ptrdiff_t>(targetBegin));
    }

    return result;
}

FrameLogits
InferencePipeline::RunFullSpectrogram(const OnnxSession &session,
                                      const Spectrogram &spectrogram)
{
    ValidateSession(session);
    ValidateSpectrogram(spectrogram);

    const std::vector<SpectrogramChunk> chunks = SplitSpectrogram(spectrogram);
    std::vector<ChunkFrameLogits>       chunkLogits;
    chunkLogits.reserve(chunks.size());

    for (const SpectrogramChunk &chunk : chunks) {
        chunkLogits.push_back(ChunkFrameLogits{
            .start_frame = chunk.start_frame,
            .logits = RunSpectrogramChunk(session,
                                          chunk.values,
                                          chunk.num_frames,
                                          chunk.num_bins),
        });
    }

    return AggregateChunkLogits(chunkLogits, spectrogram.num_frames);
}

} // namespace PDJE_UTIL::ai::beat_this
