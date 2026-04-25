#include "util/ai/beat_this/BeatThisMelBackend.hpp"

#include <stdexcept>
#include <vector>

namespace PDJE_UTIL::ai::beat_this {

namespace {

PDJE_PARALLEL::MelFilterBankSpec
BuildMelFilterBankSpec(const BeatThisFrontendConfig &config)
{
    return PDJE_PARALLEL::MelFilterBankSpec{
        .sample_rate = config.target_sample_rate,
        .n_fft = config.nfft,
        .n_mels = config.num_mels,
        .f_min = config.f_min_hz,
        .f_max = config.f_max_hz,
        .mel_formula = config.mel_formula,
        .norm = config.norm,
    };
}

PDJE_PARALLEL::STFTRequest
BuildStftRequest(const BeatThisFrontendConfig  &config,
                 const PDJE_PARALLEL::WINDOW_LIST window)
{
    PDJE_PARALLEL::POST_PROCESS postProcess;
    postProcess.to_bin    = true;
    postProcess.toPower   = true;
    postProcess.mel_scale = true;
    postProcess.check_values();

    return PDJE_PARALLEL::STFTRequest{
        .sample_rate = config.target_sample_rate,
        .n_fft = config.nfft,
        .hop_length = static_cast<unsigned int>(config.hop_length),
        .target_window = window,
        .post_process = postProcess,
        .frame_policy = PDJE_PARALLEL::FRAME_POLICY::EXACT_WINDOWED,
        .mel_filter_bank = BuildMelFilterBankSpec(config),
    };
}

} // namespace

PdjeMelSpectrogramBackend::PdjeMelSpectrogramBackend(
    const PDJE_PARALLEL::WINDOW_LIST window,
    const PDJE_PARALLEL::MelFormula  melFormula,
    const PDJE_PARALLEL::MelNorm     melNorm)
    : window_(window),
      mel_formula_(melFormula),
      mel_norm_(melNorm)
{
}

Spectrogram
PdjeMelSpectrogramBackend::ComputeLinearMel(
    const std::span<const float> samples,
    const BeatThisFrontendConfig &config)
{
    if (config.hop_length <= 0 || config.nfft <= 0 || config.num_mels <= 0) {
        throw std::invalid_argument("beat this frontend config is invalid");
    }

    std::vector<float> waveform(samples.begin(), samples.end());
    auto               request = BuildStftRequest(config, window_);
    request.mel_filter_bank->mel_formula = mel_formula_;
    request.mel_filter_bank->norm        = mel_norm_;
    auto [mel, imag] = stft_.calculate(waveform, request);
    if (!imag.empty()) {
        throw std::runtime_error(
            "mel spectrogram backend returned unexpected imaginary output");
    }
    if (mel.empty()) {
        throw std::runtime_error(
            "mel spectrogram backend returned empty output");
    }
    if ((mel.size() % static_cast<std::size_t>(config.num_mels)) != 0u) {
        throw std::runtime_error(
            "mel spectrogram backend returned misaligned output");
    }

    return Spectrogram{
        .num_frames =
            static_cast<int>(mel.size() /
                             static_cast<std::size_t>(config.num_mels)),
        .num_bins = config.num_mels,
        .values = std::move(mel),
    };
}

} // namespace PDJE_UTIL::ai::beat_this
