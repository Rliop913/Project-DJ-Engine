#pragma once

#include "util/ai/beat_this/BeatThisFrontend.hpp"
#include "util/function/stft/STFT_Parallel.hpp"

#include <span>

namespace PDJE_UTIL::ai::beat_this {

class PdjeMelSpectrogramBackend final : public MelSpectrogramBackend {
  public:
    explicit PdjeMelSpectrogramBackend(
        PDJE_PARALLEL::WINDOW_LIST window = PDJE_PARALLEL::WINDOW_LIST::HANNING,
        PDJE_PARALLEL::MelFormula  mel_formula =
            PDJE_PARALLEL::MelFormula::Slaney,
        PDJE_PARALLEL::MelNorm mel_norm = PDJE_PARALLEL::MelNorm::Slaney);

    Spectrogram
    ComputeLinearMel(std::span<const float>           padded_mono,
                     const BeatThisFrontendConfig &config) override;

    PDJE_PARALLEL::WINDOW_LIST
    window() const noexcept
    {
        return window_;
    }

  private:
    PDJE_PARALLEL::WINDOW_LIST window_;
    PDJE_PARALLEL::MelFormula  mel_formula_;
    PDJE_PARALLEL::MelNorm     mel_norm_;
    PDJE_PARALLEL::STFT        stft_;
};

} // namespace PDJE_UTIL::ai::beat_this
