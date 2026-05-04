#include <doctest/doctest.h>

#include "util/ai/AI.hpp"
#include "util/ai/beat_this/BeatThisFrontend.hpp"
#include "util/ai/beat_this/BeatThisInference.hpp"
#include "util/ai/beat_this/BeatThisMelBackend.hpp"
#include "util/ai/beat_this/BeatThisPostprocessor.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <filesystem>
#include <limits>
#include <memory>
#include <span>
#include <stdexcept>
#include <vector>

namespace {

namespace beat_this = PDJE_UTIL::ai::beat_this;

constexpr double kPi = 3.14159265358979323846;

std::vector<float>
BuildSineWave(const int sampleRate,
              const double seconds,
              const double frequencyHz)
{
    const std::size_t sampleCount = static_cast<std::size_t>(
        static_cast<double>(sampleRate) * seconds);
    std::vector<float> samples(sampleCount, 0.0f);
    for (std::size_t idx = 0; idx < sampleCount; ++idx) {
        const double phase =
            (2.0 * kPi * frequencyHz * static_cast<double>(idx)) /
            static_cast<double>(sampleRate);
        samples[idx] = static_cast<float>(std::sin(phase));
    }

    return samples;
}

std::filesystem::path
RepoRoot()
{
    auto path = std::filesystem::path(__FILE__);
    for (int idx = 0; idx < 4; ++idx) {
        path = path.parent_path();
    }

    return path;
}

std::filesystem::path
BeatThisModelPath()
{
    return RepoRoot() / "third_party" / "onnx_models" /
           "beat_this_model_final0.onnx";
}

PDJE_UTIL::ai::NamedFloatTensor
BuildSpectrogramInput(const int numFrames)
{
    PDJE_UTIL::ai::NamedFloatTensor input;
    input.name         = "spect";
    input.tensor.shape = { 1, static_cast<int64_t>(numFrames), 128 };
    input.tensor.values.resize(static_cast<std::size_t>(numFrames) * 128u, 0.0f);

    for (std::size_t idx = 0; idx < input.tensor.values.size(); ++idx) {
        input.tensor.values[idx] =
            static_cast<float>((idx % 17u) + 1u) / 17.0f;
    }

    return input;
}

class InspectingBackend final : public beat_this::MelSpectrogramBackend {
  public:
    beat_this::Spectrogram
    ComputeLinearMel(std::span<const float> samples,
                     const PDJE_UTIL::ai::BeatThisFrontendConfig &config) override
    {
        observed_input.assign(samples.begin(), samples.end());
        observed_config = config;
        return beat_this::Spectrogram{
            .num_frames = 1,
            .num_bins = config.num_mels,
            .values = std::vector<float>(
                static_cast<std::size_t>(config.num_mels), 0.5f),
        };
    }

    std::vector<float>                        observed_input;
    PDJE_UTIL::ai::BeatThisFrontendConfig observed_config{};
};

void
CheckSortedFinite(const std::vector<double> &values)
{
    for (const double value : values) {
        CHECK(std::isfinite(value));
        CHECK(value >= 0.0);
    }
    CHECK(std::is_sorted(values.begin(), values.end()));
}

} // namespace

TEST_CASE("onnx session loads checked-in model and reports beat-this io contract")
{
    const auto modelPath = BeatThisModelPath();
    REQUIRE(std::filesystem::exists(modelPath));

    PDJE_UTIL::ai::OnnxSession session(modelPath);

    CHECK(session.model_path() == modelPath);
    CHECK(session.input_count() == 1u);
    CHECK(session.output_count() == 1u);
    REQUIRE(session.input_names().size() == 1u);
    REQUIRE(session.output_names().size() == 1u);
    CHECK(session.input_name(0u) == "spect");
    CHECK(session.output_name(0u) == "logits");
}

TEST_CASE("onnx session runs dense float tensors and returns named float outputs")
{
    PDJE_UTIL::ai::OnnxSession session(BeatThisModelPath());
    std::vector<PDJE_UTIL::ai::NamedFloatTensor> inputs{
        BuildSpectrogramInput(32),
    };
    std::vector<std::string> requestedOutputs{ "logits" };

    const auto outputs = session.run(inputs, requestedOutputs);

    REQUIRE(outputs.size() == 1u);
    CHECK(outputs[0].name == "logits");
    REQUIRE(outputs[0].tensor.shape.size() == 3u);
    CHECK(outputs[0].tensor.shape[0] == 1);
    CHECK(outputs[0].tensor.shape[1] == 32);
    CHECK(outputs[0].tensor.shape[2] == 2);
    REQUIRE(outputs[0].tensor.values.size() == 64u);
    for (const float value : outputs[0].tensor.values) {
        CHECK(std::isfinite(value));
    }
}

TEST_CASE("onnx session rejects invalid model path and tensor inputs")
{
    const auto missingModel = RepoRoot() / "third_party" / "onnx_models" /
                              "missing_beat_this_model.onnx";
    CHECK_THROWS_AS(
        [&]() {
            PDJE_UTIL::ai::OnnxSession session(missingModel);
        }(),
        std::runtime_error);

    PDJE_UTIL::ai::OnnxSession session(BeatThisModelPath());

    SUBCASE("invalid input name")
    {
        auto input = BuildSpectrogramInput(8);
        input.name = "wrong";
        std::vector<PDJE_UTIL::ai::NamedFloatTensor> inputs{ std::move(input) };

        CHECK_THROWS_AS(session.run(inputs), std::invalid_argument);
    }

    SUBCASE("invalid flattened size")
    {
        auto input = BuildSpectrogramInput(8);
        input.tensor.values.pop_back();
        std::vector<PDJE_UTIL::ai::NamedFloatTensor> inputs{ std::move(input) };

        CHECK_THROWS_AS(session.run(inputs), std::invalid_argument);
    }
}

TEST_CASE("beat this detector requires an explicit model path")
{
    CHECK_THROWS_AS(
        []() {
            PDJE_UTIL::ai::BeatThisDetector detector;
        }(),
        std::invalid_argument);

    CHECK_THROWS_AS(
        []() {
            PDJE_UTIL::ai::BeatThisDetector detector(
                PDJE_UTIL::ai::BeatThisFrontendConfig{});
        }(),
        std::invalid_argument);

    CHECK_THROWS_AS(
        []() {
            PDJE_UTIL::ai::BeatThisDetector detector(std::filesystem::path{});
        }(),
        std::invalid_argument);
}

TEST_CASE("beat this preprocess resamples waveform to the frontend sample rate")
{
    const auto input = BuildSineWave(44100, 0.1, 440.0);
    const auto output =
        beat_this::FrontendPipeline::PrepareMonoWaveform(input, 44100);

    REQUIRE(!output.empty());
    CHECK(std::abs(static_cast<int>(output.size()) - 2205) <= 4);
}

TEST_CASE("beat this preprocess reflect pads input and applies log mel scaling")
{
    auto backend = std::make_shared<InspectingBackend>();

    std::vector<float> mono(600, 0.0f);
    for (std::size_t idx = 0; idx < mono.size(); ++idx) {
        mono[idx] = static_cast<float>(idx);
    }

    const auto spectrogram =
        beat_this::FrontendPipeline::ComputeLogMelSpectrogram(mono, *backend);

    REQUIRE(spectrogram.num_frames == 1);
    REQUIRE(spectrogram.num_bins == 128);
    REQUIRE(spectrogram.values.size() == 128u);
    CHECK(spectrogram.values.front() == doctest::Approx(std::log1p(500.0f)));

    REQUIRE(backend->observed_input.size() == mono.size() + 1024u);
    CHECK(backend->observed_input.front() == doctest::Approx(mono[512]));
    CHECK(backend->observed_input[511] == doctest::Approx(mono[1]));
    CHECK(backend->observed_input[512] == doctest::Approx(mono[0]));
    CHECK(backend->observed_input.back() ==
          doctest::Approx(mono[mono.size() - 513u]));
}

TEST_CASE("pdje mel backend produces exact-frame 128-bin linear mel output")
{
    beat_this::PdjeMelSpectrogramBackend    backend;
    const PDJE_UTIL::ai::BeatThisFrontendConfig config;
    std::vector<float> padded(config.nfft + (2 * config.hop_length), 0.25f);

    const auto spectrogram = backend.ComputeLinearMel(padded, config);

    REQUIRE(spectrogram.num_frames == 3);
    REQUIRE(spectrogram.num_bins == config.num_mels);
    REQUIRE(spectrogram.values.size() ==
            static_cast<std::size_t>(spectrogram.num_frames *
                                     spectrogram.num_bins));

    for (const float value : spectrogram.values) {
        CHECK(std::isfinite(value));
        CHECK(value >= 0.0f);
    }
}

TEST_CASE("beat this inference chunk helpers split and aggregate full spectrogram coverage")
{
    beat_this::Spectrogram spectrogram{
        .num_frames = 3010,
        .num_bins = 128,
        .values = std::vector<float>(3010u * 128u, 1.0f),
    };

    const auto chunks = beat_this::InferencePipeline::SplitSpectrogram(spectrogram);
    REQUIRE(chunks.size() >= 3u);

    std::vector<beat_this::ChunkFrameLogits> chunkLogits;
    chunkLogits.reserve(chunks.size());

    for (const auto &chunk : chunks) {
        beat_this::FrameLogits logits{
            .num_frames = chunk.num_frames,
            .beat = std::vector<float>(
                static_cast<std::size_t>(chunk.num_frames), 0.0f),
            .downbeat = std::vector<float>(
                static_cast<std::size_t>(chunk.num_frames), 0.0f),
        };

        for (int frame = 0; frame < chunk.num_frames; ++frame) {
            const float globalFrame =
                static_cast<float>(chunk.start_frame + frame);
            logits.beat[static_cast<std::size_t>(frame)] = globalFrame;
            logits.downbeat[static_cast<std::size_t>(frame)] =
                globalFrame * 2.0f;
        }

        chunkLogits.push_back(beat_this::ChunkFrameLogits{
            .start_frame = chunk.start_frame,
            .logits = std::move(logits),
        });
    }

    const auto aggregated = beat_this::InferencePipeline::AggregateChunkLogits(
        chunkLogits, spectrogram.num_frames);

    REQUIRE(aggregated.num_frames == spectrogram.num_frames);
    for (int frame = 0; frame < aggregated.num_frames; ++frame) {
        CHECK(aggregated.beat[static_cast<std::size_t>(frame)] ==
              doctest::Approx(static_cast<float>(frame)));
        CHECK(aggregated.downbeat[static_cast<std::size_t>(frame)] ==
              doctest::Approx(static_cast<float>(frame) * 2.0f));
    }
}

TEST_CASE("beat this postprocessor extracts peaks and snaps downbeats to beats")
{
    const beat_this::FrameLogits logits{
        .num_frames = 8,
        .beat = { -1.0f, 1.0f, 0.2f, 0.3f, -0.1f, 0.4f, 0.9f, -0.5f },
        .downbeat = { 0.7f, -0.2f, 0.2f, 0.1f, -0.1f, 0.3f, 0.2f, 0.8f },
    };

    beat_this::MinimalBeatPostprocessor postprocessor(50.0);
    const auto result = postprocessor.Process(logits);

    REQUIRE(result.beats.size() == 2u);
    REQUIRE(result.downbeats.size() == 2u);
    CHECK(result.beats[0] == doctest::Approx(1.0 / 50.0));
    CHECK(result.beats[1] == doctest::Approx(6.0 / 50.0));
    CHECK(result.downbeats[0] == doctest::Approx(result.beats[0]));
    CHECK(result.downbeats[1] == doctest::Approx(result.beats[1]));
}

TEST_CASE("beat this detector loads checked-in model and returns sorted finite timestamps")
{
    const auto modelPath = BeatThisModelPath();
    REQUIRE(std::filesystem::exists(modelPath));

    PDJE_UTIL::ai::BeatThisDetector detector(modelPath);
    const auto input  = BuildSineWave(44100, 4.0, 120.0);
    const auto result = detector.detect(input, 44100);

    CHECK(detector.model_path() == modelPath);
    CHECK(detector.frontend_config().target_sample_rate == 22050);
    CheckSortedFinite(result.beats);
    CheckSortedFinite(result.downbeats);
}
