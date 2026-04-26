#pragma once

#include "global/PDJE_EXPORT_SETTER.hpp"
#include "util/function/stft/MelFilterBank.hpp"
#include "util/function/stft/STFT_Parallel.hpp"

#include <filesystem>
#include <memory>
#include <span>
#include <vector>

namespace PDJE_UTIL::ai {

struct BeatThisFrontendConfig {
    int                       target_sample_rate = 22050;
    int                       nfft               = 1024;
    int                       hop_length         = 441;
    int                       num_mels           = 128;
    int                       pad                = 512;
    float                     f_min_hz           = 30.0f;
    float                     f_max_hz           = 11000.0f;
    float                     log_multiplier     = 1000.0f;
    PDJE_PARALLEL::MelFormula mel_formula        =
        PDJE_PARALLEL::MelFormula::Slaney;
    PDJE_PARALLEL::MelNorm norm = PDJE_PARALLEL::MelNorm::Slaney;
    PDJE_PARALLEL::WINDOW_LIST window = PDJE_PARALLEL::WINDOW_LIST::HANNING;

    bool
    operator==(const BeatThisFrontendConfig &) const = default;
};

struct BeatDetectionResult {
    std::vector<double> beats;
    std::vector<double> downbeats;

    bool
    empty() const noexcept
    {
        return beats.empty() && downbeats.empty();
    }
};

class PDJE_API BeatThisDetector {
  public:
    BeatThisDetector();
    explicit BeatThisDetector(BeatThisFrontendConfig frontend_config);
    BeatThisDetector(std::filesystem::path model_path,
                     BeatThisFrontendConfig frontend_config = {});
    ~BeatThisDetector();

    BeatThisDetector(BeatThisDetector &&) noexcept;
    BeatThisDetector &
    operator=(BeatThisDetector &&) noexcept;

    BeatThisDetector(const BeatThisDetector &)            = delete;
    BeatThisDetector &operator=(const BeatThisDetector &) = delete;

    BeatDetectionResult
    detect(std::span<const float> samples, int input_sample_rate) const;

    const BeatThisFrontendConfig &
    frontend_config() const noexcept;

    const std::filesystem::path &
    model_path() const noexcept;

  private:
    class Impl;

    std::unique_ptr<Impl> impl_;
};

} // namespace PDJE_UTIL::ai
