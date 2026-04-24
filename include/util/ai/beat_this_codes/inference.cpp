#include "inference.hpp"

#include <onnxruntime_cxx_api.h>

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <iterator>
#include <memory>
#include <span>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace beat_this::inference {

using beat_this::preprocess::Spectrogram;

Ort::Env& InferencePipeline::OrtEnv() {
  static Ort::Env env(ORT_LOGGING_LEVEL_WARNING, "beat_this_cpp");
  return env;
}

std::string InferencePipeline::ReadIoName(
    Ort::Session& session,
    const bool input,
    const std::size_t index,
    Ort::AllocatorWithDefaultOptions& allocator) {
  Ort::AllocatedStringPtr name = input ? session.GetInputNameAllocated(index, allocator)
                                       : session.GetOutputNameAllocated(index, allocator);
  if (!name || name.get()[0] == '\0') {
    throw std::runtime_error(input ? "onnx model input name is empty" : "onnx model output name is empty");
  }
  return std::string(name.get());
}

void InferencePipeline::RequireIoName(const std::string& actual,
                                      const char* expected,
                                      const char* kind) {
  if (actual != expected) {
    throw std::runtime_error(std::string("unexpected onnx model ") + kind + " name: expected '" + expected +
                             "', got '" + actual + "'");
  }
}

void InferencePipeline::ValidateSpectrogram(const Spectrogram& spectrogram) {
  if (spectrogram.num_frames <= 0) {
    throw std::invalid_argument("spectrogram must contain at least one frame");
  }
  if (spectrogram.num_bins != kNumMels) {
    throw std::invalid_argument("spectrogram must contain 128 mel bins");
  }
  const std::size_t expected_values = static_cast<std::size_t>(spectrogram.num_frames) *
                                      static_cast<std::size_t>(spectrogram.num_bins);
  if (spectrogram.values.size() != expected_values) {
    throw std::invalid_argument("spectrogram flattened storage size does not match its shape");
  }
}

std::vector<int> InferencePipeline::ComputeChunkStarts(const int num_frames) {
  const int step = kChunkSize - (2 * kChunkBorderSize);
  std::vector<int> starts;
  for (int start = -kChunkBorderSize; start < num_frames - kChunkBorderSize; start += step) {
    starts.push_back(start);
  }
  if (num_frames > step && !starts.empty()) {
    starts.back() = num_frames - (kChunkSize - kChunkBorderSize);
  }
  return starts;
}

std::vector<SpectrogramChunk> InferencePipeline::SplitSpectrogram(
    const Spectrogram& spectrogram) {
  const std::vector<int> starts = ComputeChunkStarts(spectrogram.num_frames);
  std::vector<SpectrogramChunk> chunks;
  chunks.reserve(starts.size());

  for (const int start : starts) {
    const int content_begin = std::max(start, 0);
    const int content_end = std::min(start + kChunkSize, spectrogram.num_frames);
    const int left_pad = std::max(0, -start);
    const int right_pad = std::max(0, std::min(kChunkBorderSize, start + kChunkSize - spectrogram.num_frames));
    const int chunk_num_frames = left_pad + (content_end - content_begin) + right_pad;

    SpectrogramChunk chunk{
        .start_frame = start,
        .num_frames = chunk_num_frames,
        .num_bins = spectrogram.num_bins,
        .values = std::vector<float>(
            static_cast<std::size_t>(chunk_num_frames) *
                static_cast<std::size_t>(spectrogram.num_bins),
            0.0F),
    };

    const std::size_t source_offset =
        static_cast<std::size_t>(content_begin) * static_cast<std::size_t>(spectrogram.num_bins);
    const std::size_t copy_count =
        static_cast<std::size_t>(content_end - content_begin) *
        static_cast<std::size_t>(spectrogram.num_bins);
    const std::size_t destination_offset =
        static_cast<std::size_t>(left_pad) * static_cast<std::size_t>(spectrogram.num_bins);
    std::copy_n(spectrogram.values.begin() + static_cast<std::ptrdiff_t>(source_offset),
                static_cast<std::ptrdiff_t>(copy_count),
                chunk.values.begin() + static_cast<std::ptrdiff_t>(destination_offset));

    chunks.push_back(std::move(chunk));
  }

  return chunks;
}

FrameLogits InferencePipeline::RunSpectrogramChunk(Ort::Session& session,
                                                   const std::string& input_name,
                                                   const std::string& output_name,
                                                   const std::span<const float> values,
                                                   const int num_frames,
                                                   const int num_bins) {
  static const Ort::MemoryInfo memory_info =
      Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);
  const std::array<int64_t, 3> input_shape = {1, static_cast<int64_t>(num_frames),
                                              static_cast<int64_t>(num_bins)};
  Ort::Value input_tensor = Ort::Value::CreateTensor<float>(
      memory_info,
      const_cast<float*>(values.data()),
      values.size(),
      input_shape.data(),
      input_shape.size());

  const std::array<const char*, 1> input_names = {input_name.c_str()};
  const std::array<const char*, 1> output_names = {output_name.c_str()};
  auto outputs = session.Run(Ort::RunOptions{nullptr},
                             input_names.data(),
                             &input_tensor,
                             input_names.size(),
                             output_names.data(),
                             output_names.size());
  if (outputs.size() != 1U || !outputs[0].IsTensor()) {
    throw std::runtime_error("onnx runtime did not return the expected tensor output");
  }

  const Ort::TensorTypeAndShapeInfo output_info = outputs[0].GetTensorTypeAndShapeInfo();
  const std::vector<int64_t> output_shape = output_info.GetShape();
  if (output_shape.size() != 3U || output_shape[0] != 1 ||
      output_shape[1] != static_cast<int64_t>(num_frames) ||
      output_shape[2] != static_cast<int64_t>(kModelOutputChannels)) {
    throw std::runtime_error("onnx model returned an unexpected logits shape");
  }

  const std::size_t expected_output_values =
      static_cast<std::size_t>(num_frames) * static_cast<std::size_t>(kModelOutputChannels);
  if (output_info.GetElementCount() != expected_output_values) {
    throw std::runtime_error("onnx model returned an unexpected logits element count");
  }

  const float* output_data = outputs[0].GetTensorData<float>();
  FrameLogits logits{
      .num_frames = num_frames,
      .beat = std::vector<float>(static_cast<std::size_t>(num_frames), 0.0F),
      .downbeat = std::vector<float>(static_cast<std::size_t>(num_frames), 0.0F),
  };

  for (int frame = 0; frame < num_frames; ++frame) {
    const std::size_t offset = static_cast<std::size_t>(frame) *
                               static_cast<std::size_t>(kModelOutputChannels);
    logits.beat[static_cast<std::size_t>(frame)] = output_data[offset];
    logits.downbeat[static_cast<std::size_t>(frame)] = output_data[offset + 1U];
  }

  return logits;
}

FrameLogits InferencePipeline::AggregateChunkLogits(
    const std::span<const ChunkFrameLogits> chunks,
    const int full_num_frames) {
  FrameLogits result{
      .num_frames = full_num_frames,
      .beat = std::vector<float>(static_cast<std::size_t>(full_num_frames), kPaddingLogit),
      .downbeat = std::vector<float>(static_cast<std::size_t>(full_num_frames), kPaddingLogit),
  };

  for (auto chunk_it = chunks.rbegin(); chunk_it != chunks.rend(); ++chunk_it) {
    const int target_begin = std::max(0, chunk_it->start_frame + kChunkBorderSize);
    const int target_end =
        std::min(full_num_frames, chunk_it->start_frame + kChunkSize - kChunkBorderSize);
    if (target_end <= target_begin) {
      continue;
    }

    const std::size_t copy_count = static_cast<std::size_t>(target_end - target_begin);
    const std::size_t source_offset = static_cast<std::size_t>(kChunkBorderSize);
    std::copy_n(chunk_it->logits.beat.begin() + static_cast<std::ptrdiff_t>(source_offset),
                static_cast<std::ptrdiff_t>(copy_count),
                result.beat.begin() + static_cast<std::ptrdiff_t>(target_begin));
    std::copy_n(
        chunk_it->logits.downbeat.begin() + static_cast<std::ptrdiff_t>(source_offset),
        static_cast<std::ptrdiff_t>(copy_count),
        result.downbeat.begin() + static_cast<std::ptrdiff_t>(target_begin));
  }

  return result;
}

FrameLogits InferencePipeline::RunFullSpectrogram(Ort::Session& session,
                                                  const std::string& input_name,
                                                  const std::string& output_name,
                                                  const Spectrogram& spectrogram) {
  ValidateSpectrogram(spectrogram);

  const std::vector<SpectrogramChunk> chunks = SplitSpectrogram(spectrogram);
  std::vector<ChunkFrameLogits> chunk_logits;
  chunk_logits.reserve(chunks.size());

  for (const SpectrogramChunk& chunk : chunks) {
    chunk_logits.push_back(ChunkFrameLogits{
        .start_frame = chunk.start_frame,
        .logits = RunSpectrogramChunk(
            session, input_name, output_name, chunk.values, chunk.num_frames, chunk.num_bins),
    });
  }

  return AggregateChunkLogits(chunk_logits, spectrogram.num_frames);
}

BeatThisOnnxRunner::BeatThisOnnxRunner(
    std::filesystem::path model_path,
    std::shared_ptr<beat_this::preprocess::MelSpectrogramBackend> backend)
    : model_path_(std::move(model_path)),
      preprocessor_(std::move(backend)) {
  if (!std::filesystem::exists(model_path_)) {
    throw std::runtime_error(std::string("onnx model was not found: ") + model_path_.string());
  }

  Ort::SessionOptions options;
  options.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_EXTENDED);
  options.SetIntraOpNumThreads(1);
  options.SetInterOpNumThreads(1);

  session_ = std::make_unique<Ort::Session>(InferencePipeline::OrtEnv(), model_path_.c_str(), options);

  Ort::AllocatorWithDefaultOptions allocator;
  if (session_->GetInputCount() != 1U) {
    throw std::runtime_error("onnx model must expose exactly one input");
  }
  if (session_->GetOutputCount() != 1U) {
    throw std::runtime_error("onnx model must expose exactly one output");
  }

  input_name_ = InferencePipeline::ReadIoName(*session_, true, 0U, allocator);
  output_name_ = InferencePipeline::ReadIoName(*session_, false, 0U, allocator);
  InferencePipeline::RequireIoName(input_name_, "spect", "input");
  InferencePipeline::RequireIoName(output_name_, "logits", "output");
}

BeatThisOnnxRunner::~BeatThisOnnxRunner() = default;
BeatThisOnnxRunner::BeatThisOnnxRunner(BeatThisOnnxRunner&&) noexcept = default;
BeatThisOnnxRunner& BeatThisOnnxRunner::operator=(BeatThisOnnxRunner&&) noexcept = default;

// Mono float PCM input: 1 channel; input_sample_rate is in Hz.
FrameLogits BeatThisOnnxRunner::Execute(const std::span<const float> samples,
                                        const int input_sample_rate) const {
  const beat_this::preprocess::Spectrogram spectrogram =
      preprocessor_.Execute(samples, input_sample_rate);
  return InferencePipeline::RunFullSpectrogram(*session_, input_name_, output_name_, spectrogram);
}

}  // namespace beat_this::inference
