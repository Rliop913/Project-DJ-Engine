#include "util/ai/AI.hpp"

#include "util/ai/beat_this/BeatThisFrontend.hpp"
#include "util/ai/beat_this/BeatThisInference.hpp"
#include "util/ai/beat_this/BeatThisMelBackend.hpp"
#include "util/ai/beat_this/BeatThisPostprocessor.hpp"
#include "util/ai/beat_this/detail/BeatThisValidation.hpp"

#include <memory>
#include <stdexcept>
#include <utility>

namespace PDJE_UTIL::ai {

namespace {

BeatThisFrontendConfig
ValidateAndReturn(BeatThisFrontendConfig config)
{
    beat_this::detail::ValidateFrontendConfig(config);
    return config;
}

double
FramesPerSecond(const BeatThisFrontendConfig &config) noexcept
{
    if (config.hop_length <= 0) {
        return 0.0;
    }

    return static_cast<double>(config.target_sample_rate) /
           static_cast<double>(config.hop_length);
}

const BeatThisFrontendConfig &
EmptyConfig() noexcept
{
    static const BeatThisFrontendConfig config{};
    return config;
}

const std::filesystem::path &
EmptyPath() noexcept
{
    static const std::filesystem::path path;
    return path;
}

} // namespace

class BeatThisDetector::Impl {
  public:
    Impl(const std::filesystem::path  &modelPath,
         const BeatThisFrontendConfig &frontendConfig)
        : session_(modelPath),
          backend_(std::make_shared<beat_this::PdjeMelSpectrogramBackend>(
              frontendConfig.window,
              frontendConfig.mel_formula,
              frontendConfig.norm)),
          frontend_(backend_, frontendConfig),
          postprocessor_(FramesPerSecond(frontendConfig))
    {
        beat_this::InferencePipeline::ValidateSession(session_);
    }

    BeatDetectionResult
    detect(std::span<const float> samples, const int inputSampleRate) const
    {
        const beat_this::Spectrogram spectrogram =
            frontend_.Execute(samples, inputSampleRate);
        const beat_this::FrameLogits logits =
            beat_this::InferencePipeline::RunFullSpectrogram(session_,
                                                             spectrogram);
        return postprocessor_.Process(logits);
    }

    OnnxSession                                           session_;
    std::shared_ptr<beat_this::PdjeMelSpectrogramBackend> backend_;
    beat_this::FrontendProcessor                          frontend_;
    beat_this::MinimalBeatPostprocessor                   postprocessor_;
};

BeatThisDetector::BeatThisDetector()
    : BeatThisDetector(std::filesystem::path{}, {})
{
}

BeatThisDetector::BeatThisDetector(BeatThisFrontendConfig frontend_config)
    : BeatThisDetector(std::filesystem::path{}, std::move(frontend_config))
{
}

BeatThisDetector::BeatThisDetector(std::filesystem::path  model_path,
                                   BeatThisFrontendConfig frontend_config)
    : model_path(std::move(model_path)),
      frontend_config(ValidateAndReturn(std::move(frontend_config))),
      impl_(std::make_unique<Impl>(this->model_path, this->frontend_config))
{
}

BeatThisDetector::~BeatThisDetector() = default;

BeatThisDetector::BeatThisDetector(BeatThisDetector &&other)
    : model_path(other.model_path), frontend_config(other.frontend_config),
      impl_(std::move(other.impl_))
{
}

BeatDetectionResult
BeatThisDetector::detect(const std::span<const float> samples,
                         const int                    input_sample_rate) const
{
    if (!impl_) {
        throw std::runtime_error("beat this detector is not initialized");
    }

    return impl_->detect(samples, input_sample_rate);
}

} // namespace PDJE_UTIL::ai
