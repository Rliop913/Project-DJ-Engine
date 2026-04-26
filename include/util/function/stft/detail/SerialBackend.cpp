#include "util/function/stft/detail/SerialBackend.hpp"

#include "util/function/stft/BackendLess.hpp"
#include "util/function/stft/MelFilterBank.hpp"
#include "STFT_MAIN_SERIAL.hpp"

#include <algorithm>
#include <functional>
#include <optional>

namespace PDJE_PARALLEL::detail {

namespace {

constexpr float kDefaultGaussianSigma = 0.4f;

unsigned int
ResolveMelBins(const StftArgs &gargs) noexcept
{
    if (!gargs.mel_filter_bank.has_value() ||
        gargs.mel_filter_bank->n_mels <= 0) {
        return 0u;
    }

    return static_cast<unsigned int>(gargs.mel_filter_bank->n_mels);
}

} // namespace

void
SERIAL_STFT::EnsureMemory(const StftArgs     &gargs,
                          const POST_PROCESS &post_process,
                          const bool          needSubBuffer)
{
    if (prev_overlap_fullsize != gargs.OFullSize) {
        real.resize(gargs.OFullSize);
        imag.resize(gargs.OFullSize);
        prev_overlap_fullsize = gargs.OFullSize;
    }

    std::fill(imag.begin(), imag.end(), 0.0f);

    if (needSubBuffer && prev_overlap_subbuffer_fullsize != gargs.OFullSize) {
        subreal.resize(gargs.OFullSize);
        subimag.resize(gargs.OFullSize);
        prev_overlap_subbuffer_fullsize = gargs.OFullSize;
    }

    const uint32_t binSize =
        static_cast<uint32_t>((gargs.windowSize >> 1) + 1);
    const uint32_t binFullSize = binSize * static_cast<uint32_t>(gargs.qtConst);
    if (post_process.to_bin && prev_bin_fullsize != binFullSize) {
        bin_real.resize(binFullSize);
        bin_imag.resize(binFullSize);
        prev_bin_fullsize = binFullSize;
    }

    const uint32_t melBins = ResolveMelBins(gargs);
    const uint32_t melFullSize =
        melBins * static_cast<uint32_t>(gargs.qtConst);
    if (post_process.mel_scale && prev_mel_fullsize != melFullSize) {
        mel.resize(melFullSize);
        prev_mel_fullsize = melFullSize;
    }
}

void
SERIAL_STFT::EnsureMelFilterBank(const StftArgs &gargs)
{
    if (!gargs.mel_filter_bank.has_value()) {
        mel_filter_bank.clear();
        prev_mel_filter_bank_spec.reset();
        return;
    }

    if (prev_mel_filter_bank_spec == gargs.mel_filter_bank &&
        !mel_filter_bank.empty()) {
        return;
    }

    mel_filter_bank = GenMelFilterBank(gargs.mel_filter_bank.value());
    prev_mel_filter_bank_spec = gargs.mel_filter_bank;
}

void
SERIAL_STFT::ApplyWindow(const WINDOW_LIST target_window, const StftArgs &gargs)
{
    switch (target_window) {
    case WINDOW_LIST::BLACKMAN:
        Window_Blackman(real.data(), gargs.OFullSize, gargs.windowSize);
        break;
    case WINDOW_LIST::BLACKMAN_HARRIS:
        Window_Blackman_harris(real.data(), gargs.OFullSize, gargs.windowSize);
        break;
    case WINDOW_LIST::BLACKMAN_NUTTALL:
        Window_Blackman_Nuttall(
            real.data(), gargs.OFullSize, gargs.windowSize);
        break;
    case WINDOW_LIST::NUTTALL:
        Window_Nuttall(real.data(), gargs.OFullSize, gargs.windowSize);
        break;
    case WINDOW_LIST::FLATTOP:
        Window_FlatTop(real.data(), gargs.OFullSize, gargs.windowSize);
        break;
    case WINDOW_LIST::GAUSSIAN:
        Window_Gaussian(
            real.data(), gargs.OFullSize, gargs.windowSize, kDefaultGaussianSigma);
        break;
    case WINDOW_LIST::HAMMING:
        Window_Hamming(real.data(), gargs.OFullSize, gargs.windowSize);
        break;
    case WINDOW_LIST::HANNING:
        Window_Hanning(real.data(), gargs.OFullSize, gargs.windowSize);
        break;
    default:
        break;
    }
}

void
SERIAL_STFT::RunFft(const unsigned int windowSizeEXP, const StftArgs &gargs)
{
    switch (windowSizeEXP) {
    case 6:
        Stockhoptimized6(real.data(), imag.data(), gargs.OHalfSize);
        break;
    case 7:
        Stockhoptimized7(real.data(), imag.data(), gargs.OHalfSize);
        break;
    case 8:
        Stockhoptimized8(real.data(), imag.data(), gargs.OHalfSize);
        break;
    case 9:
        Stockhoptimized9(real.data(), imag.data(), gargs.OHalfSize);
        break;
    case 10:
        Stockhoptimized10(real.data(), imag.data(), gargs.OHalfSize);
        break;
    case 11:
        Stockhoptimized11(real.data(), imag.data(), gargs.OHalfSize);
        break;
    default: {
        const unsigned int halfWindowSize =
            static_cast<unsigned int>(gargs.windowSize >> 1);
        for (unsigned int stage = 0; stage < windowSizeEXP; ++stage) {
            if ((stage % 2) == 0) {
                StockHamCommon(real.data(),
                               imag.data(),
                               subreal.data(),
                               subimag.data(),
                               halfWindowSize,
                               stage,
                               gargs.OHalfSize,
                               windowSizeEXP);
            } else {
                StockHamCommon(subreal.data(),
                               subimag.data(),
                               real.data(),
                               imag.data(),
                               halfWindowSize,
                               stage,
                               gargs.OHalfSize,
                               windowSizeEXP);
            }
        }
        break;
    }
    }
}

StftResult
SERIAL_STFT::Execute(std::vector<float> &PCMdata,
                     const WINDOW_LIST   target_window,
                     POST_PROCESS        post_process,
                     const unsigned int  windowSizeEXP,
                     const StftArgs     &gargs)
{
    post_process.check_values();
    if (post_process.mel_scale && !gargs.mel_filter_bank.has_value()) {
        return {};
    }

    const bool needSubBuffer = windowSizeEXP > 11;
    EnsureMemory(gargs, post_process, needSubBuffer);

    Overlap_Common(PCMdata.data(),
                   gargs.OFullSize,
                   gargs.FullSize,
                   windowSizeEXP,
                   gargs.OMove,
                   real.data());

    if (gargs.dc_remove) {
        DCRemove_Common(real.data(), gargs.OFullSize, gargs.windowSize);
    }
    ApplyWindow(target_window, gargs);
    RunFft(windowSizeEXP, gargs);

    std::reference_wrapper<std::vector<float>> active_real = real;
    std::optional<std::reference_wrapper<std::vector<float>>> active_imag = imag;

    if (needSubBuffer && (windowSizeEXP % 2) != 0) {
        active_real = subreal;
        active_imag = subimag;
    }

    const uint32_t binSize =
        static_cast<uint32_t>((gargs.windowSize >> 1) + 1);
    const uint32_t binFullSize = binSize * static_cast<uint32_t>(gargs.qtConst);
    const uint32_t melBins = ResolveMelBins(gargs);
    const uint32_t melFullSize =
        melBins * static_cast<uint32_t>(gargs.qtConst);

    if (post_process.Chainable_BIN_POWER()) {
        BinPowerChain(active_real.get().data(),
                      active_imag->get().data(),
                      bin_real.data(),
                      binSize,
                      gargs.windowSize,
                      binFullSize);
        active_real = bin_real;
        active_imag.reset();
    } else if (post_process.to_bin) {
        toBinOnly(active_real.get().data(),
                  active_imag->get().data(),
                  bin_real.data(),
                  bin_imag.data(),
                  binSize,
                  gargs.windowSize,
                  binFullSize);
        active_real = bin_real;
        active_imag = bin_imag;
    } else if (post_process.toPower) {
        toPower(active_real.get().data(),
                active_real.get().data(),
                active_imag->get().data(),
                gargs.OFullSize);
        active_imag.reset();
    }

    if (post_process.mel_scale) {
        EnsureMelFilterBank(gargs);
        if (mel_filter_bank.empty()) {
            return {};
        }
    }

    if (post_process.Chainable_MEL_DB()) {
        MelDBChain(mel.data(),
                   bin_real.data(),
                   mel_filter_bank.data(),
                   melFullSize,
                   binSize,
                   melBins);
        active_real = mel;
        active_imag.reset();
    } else if (post_process.mel_scale) {
        MelScale(mel.data(),
                 bin_real.data(),
                 mel_filter_bank.data(),
                 melFullSize,
                 binSize,
                 melBins);
        active_real = mel;
        active_imag.reset();
    } else if (post_process.to_db) {
        toDB(active_real.get().data(),
             static_cast<unsigned int>(active_real.get().size()));
        active_imag.reset();
    }

    if (post_process.normalize_min_max && !post_process.to_rgb) {
        const uint32_t chunkSize =
            post_process.mel_scale
                ? melBins
                : (post_process.to_bin ? binSize
                                       : static_cast<uint32_t>(gargs.windowSize));
        Normalize_minmax(active_real.get(), chunkSize);
    }

    if (post_process.to_rgb) {
        rgb = TO_RGB(active_real.get(), melBins);
        active_real = rgb;
        active_imag.reset();
    }

    if (active_imag.has_value()) {
        return {active_real.get(), active_imag->get()};
    }

    return {active_real.get(), {}};
}

SERIAL_STFT::~SERIAL_STFT() = default;

} // namespace PDJE_PARALLEL::detail
