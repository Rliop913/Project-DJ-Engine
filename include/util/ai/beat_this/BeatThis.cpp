#include "util/ai/AI.hpp"

#include "util/ai/beat_this/BeatThisFrontend.hpp"
#include "util/ai/beat_this/BeatThisInference.hpp"
#include "util/ai/beat_this/BeatThisMelBackend.hpp"
#include "util/ai/beat_this/BeatThisPostprocessor.hpp"

#include <memory>
#include <stdexcept>
#include <utility>

namespace PDJE_UTIL::ai {

namespace {

bool
IsPowerOfTwo(const int value) noexcept
{
    if (value <= 0) {
        return false;
    }

    const unsigned int unsignedValue = static_cast<unsigned int>(value);
    return (unsignedValue & (unsignedValue - 1u)) == 0u;
}

void
ValidateFrontendConfig(const BeatThisFrontendConfig &config)
{
    if (config.target_sample_rate <= 0 || config.nfft <= 0 ||
        config.hop_length <= 0 || config.num_mels <= 0 || config.pad < 0 ||
        config.f_min_hz < 0.0f || config.f_max_hz <= config.f_min_hz ||
        config.log_multiplier <= 0.0f) {
        throw std::invalid_argument("beat this frontend config is invalid");
    }

    if (!IsPowerOfTwo(config.nfft)) {
        throw std::invalid_argument(
            "beat this frontend nfft must be a power of two");
    }
}

BeatThisFrontendConfig
ValidateAndReturn(BeatThisFrontendConfig config)
{
    ValidateFrontendConfig(config);
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
    Impl(std::filesystem::path modelPath, BeatThisFrontendConfig frontendConfig)
        : model_path_(std::move(modelPath)),
          frontend_config_(ValidateAndReturn(std::move(frontendConfig))),
          session_(model_path_),
          backend_(std::make_shared<beat_this::PdjeMelSpectrogramBackend>(
              frontend_config_.window,
              frontend_config_.mel_formula,
              frontend_config_.norm)),
          frontend_(backend_, frontend_config_),
          postprocessor_(FramesPerSecond(frontend_config_))
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

    std::filesystem::path                              model_path_;
    BeatThisFrontendConfig                             frontend_config_;
    OnnxSession                                        session_;
    std::shared_ptr<beat_this::PdjeMelSpectrogramBackend> backend_;
    beat_this::FrontendProcessor                       frontend_;
    beat_this::MinimalBeatPostprocessor                postprocessor_;
};

BeatThisDetector::BeatThisDetector()
    : BeatThisDetector(std::filesystem::path{}, {})
{
}

BeatThisDetector::BeatThisDetector(BeatThisFrontendConfig frontend_config)
    : BeatThisDetector(std::filesystem::path{}, std::move(frontend_config))
{
}

BeatThisDetector::BeatThisDetector(std::filesystem::path model_path,
                                   BeatThisFrontendConfig frontend_config)
    : impl_(std::make_unique<Impl>(std::move(model_path),
                                   std::move(frontend_config)))
{
}

BeatThisDetector::~BeatThisDetector() = default;

BeatThisDetector::BeatThisDetector(BeatThisDetector &&) noexcept = default;
BeatThisDetector &
BeatThisDetector::operator=(BeatThisDetector &&) noexcept = default;

BeatDetectionResult
BeatThisDetector::detect(const std::span<const float> samples,
                         const int                     input_sample_rate) const
{
    if (!impl_) {
        throw std::runtime_error("beat this detector is not initialized");
    }

    return impl_->detect(samples, input_sample_rate);
}

const BeatThisFrontendConfig &
BeatThisDetector::frontend_config() const noexcept
{
    return impl_ ? impl_->frontend_config_ : EmptyConfig();
}

const std::filesystem::path &
BeatThisDetector::model_path() const noexcept
{
    return impl_ ? impl_->model_path_ : EmptyPath();
}

} // namespace PDJE_UTIL::ai
