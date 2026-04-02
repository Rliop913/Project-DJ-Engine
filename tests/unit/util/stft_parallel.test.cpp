#include "STFT_Parallel.hpp"
#include "BackendLess.hpp"
#include "MelFilterBank.hpp"

#include <cmath>
#include <cstddef>
#include <limits>
#include <memory>
#include <stdexcept>
#include <vector>

#include <doctest/doctest.h>

namespace {

constexpr float kPi                 = 3.14159265358979323846f;
constexpr float kReferenceTolerance = 1.0e-3f;
constexpr int   kMelBins            = 80;
constexpr int   kDefaultSampleRate  = 48000;

std::vector<float>
BuildSignal(const std::size_t size)
{
    std::vector<float> pcm(size, 0.0f);

    for (std::size_t idx = 0; idx < pcm.size(); ++idx) {
        const float x = static_cast<float>(idx);
        pcm[idx]      = std::sin(x * 0.1f) + (0.35f * std::cos(x * 0.031f)) +
                        (0.1f * std::sin(x * 0.27f));
    }

    return pcm;
}

unsigned int
ReferenceToQuot(const std::size_t fullSize,
                const float       overlapRatio,
                const int         windowSize)
{
    if (overlapRatio == 0.0f) {
        return static_cast<unsigned int>(fullSize / windowSize) + 1;
    }

    return static_cast<unsigned int>(fullSize /
                                     (windowSize * (1.0f - overlapRatio))) +
           1;
}

std::vector<float>
BuildReferenceOverlap(const std::vector<float> &pcm,
                      const int                 windowSizeExp,
                      const float               overlapRatio)
{
    const unsigned int windowSize = 1u << windowSizeExp;
    const unsigned int outputSize =
        ReferenceToQuot(
            pcm.size(), overlapRatio, static_cast<int>(windowSize)) *
        windowSize;
    const unsigned int moveSize =
        static_cast<unsigned int>(windowSize * (1.0f - overlapRatio));

    std::vector<float> overlapped(outputSize, 0.0f);

    for (unsigned int idx = 0; idx < outputSize; ++idx) {
        const unsigned int windowIdx = idx >> windowSizeExp;
        const unsigned int localIdx  = idx & (windowSize - 1u);
        const unsigned int originIdx = windowIdx * moveSize + localIdx;

        if (originIdx < pcm.size()) {
            overlapped[idx] = pcm[originIdx];
        }
    }

    return overlapped;
}

void
ApplyReferenceDcRemove(std::vector<float> &data, const unsigned int windowSize)
{
    for (unsigned int frameBase = 0; frameBase < data.size();
         frameBase += windowSize) {
        float sum = 0.0f;

        for (unsigned int idx = 0; idx < windowSize; ++idx) {
            sum += data[frameBase + idx];
        }

        const float mean = sum / static_cast<float>(windowSize);

        for (unsigned int idx = 0; idx < windowSize; ++idx) {
            data[frameBase + idx] -= mean;
        }
    }
}

float
ReferenceHanning(const unsigned int index, const unsigned int windowSize)
{
    const float angle = 2.0f * kPi * static_cast<float>(index) /
                        static_cast<float>(windowSize - 1u);
    return 0.5f * (1.0f - std::cos(angle));
}

void
ApplyReferenceHanning(std::vector<float> &data, const unsigned int windowSize)
{
    for (unsigned int idx = 0; idx < data.size(); ++idx) {
        data[idx] *= ReferenceHanning(idx & (windowSize - 1u), windowSize);
    }
}

PDJE_PARALLEL::StftResult
ReferenceDftStft(const std::vector<float> &pcm,
                 const int                 windowSizeExp,
                 const float               overlapRatio)
{
    const unsigned int windowSize = 1u << windowSizeExp;
    auto overlapped = BuildReferenceOverlap(pcm, windowSizeExp, overlapRatio);
    std::vector<float> realOut(overlapped.size(), 0.0f);
    std::vector<float> imagOut(overlapped.size(), 0.0f);

    ApplyReferenceDcRemove(overlapped, windowSize);
    ApplyReferenceHanning(overlapped, windowSize);

    for (unsigned int frameBase = 0; frameBase < overlapped.size();
         frameBase += windowSize) {
        for (unsigned int k = 0; k < windowSize; ++k) {
            float realSum = 0.0f;
            float imagSum = 0.0f;

            for (unsigned int n = 0; n < windowSize; ++n) {
                const float sample = overlapped[frameBase + n];
                const float angle  = -2.0f * kPi * static_cast<float>(k * n) /
                                     static_cast<float>(windowSize);
                realSum += sample * std::cos(angle);
                imagSum += sample * std::sin(angle);
            }

            realOut[frameBase + k] = realSum;
            imagOut[frameBase + k] = imagSum;
        }
    }

    return { std::move(realOut), std::move(imagOut) };
}

PDJE_PARALLEL::StftResult
ReferenceBinOnly(const std::vector<float> &realOut,
                 const std::vector<float> &imagOut,
                 const int                 windowSizeExp)
{
    const unsigned int windowSize = 1u << windowSizeExp;
    const unsigned int binSize    = (windowSize >> 1) + 1u;
    const unsigned int qtConst =
        static_cast<unsigned int>(realOut.size() / windowSize);

    std::vector<float> binReal(static_cast<std::size_t>(qtConst) * binSize,
                               0.0f);
    std::vector<float> binImag(static_cast<std::size_t>(qtConst) * binSize,
                               0.0f);

    for (unsigned int frameIdx = 0; frameIdx < qtConst; ++frameIdx) {
        const unsigned int frameBase = frameIdx * windowSize;
        const unsigned int binBase   = frameIdx * binSize;

        for (unsigned int binIdx = 0; binIdx < binSize; ++binIdx) {
            binReal[binBase + binIdx] = realOut[frameBase + binIdx];
            binImag[binBase + binIdx] = imagOut[frameBase + binIdx];
        }
    }

    return { std::move(binReal), std::move(binImag) };
}

std::vector<float>
ReferencePower(const std::vector<float> &realOut,
               const std::vector<float> &imagOut)
{
    std::vector<float> power(realOut.size(), 0.0f);

    for (std::size_t idx = 0; idx < realOut.size(); ++idx) {
        power[idx] =
            (realOut[idx] * realOut[idx]) + (imagOut[idx] * imagOut[idx]);
    }

    return power;
}

std::vector<float>
ReferenceBinPower(const std::vector<float> &realOut,
                  const std::vector<float> &imagOut,
                  const int                 windowSizeExp)
{
    auto [binReal, binImag] = ReferenceBinOnly(realOut, imagOut, windowSizeExp);
    return ReferencePower(binReal, binImag);
}

std::vector<float>
ReferenceMelOutput(const std::vector<float> &binPower,
                   const int                 windowSize,
                   const unsigned int        qtConst,
                   const bool                toDb)
{
    const unsigned int binSize = (windowSize >> 1) + 1u;
    const auto         melFilterBank =
        PDJE_PARALLEL::GenMelFilterBank(kDefaultSampleRate,
                                        windowSize,
                                        kMelBins,
                                        0.0f,
                                        -1.0f,
                                        PDJE_PARALLEL::MelFormula::Slaney,
                                        PDJE_PARALLEL::MelNorm::Slaney);

    std::vector<float> mel(static_cast<std::size_t>(qtConst) *
                               static_cast<std::size_t>(kMelBins),
                           0.0f);

    for (unsigned int frameIdx = 0; frameIdx < qtConst; ++frameIdx) {
        const unsigned int frameBase = frameIdx * binSize;

        for (unsigned int melIdx = 0;
             melIdx < static_cast<unsigned int>(kMelBins);
             ++melIdx) {
            const unsigned int filterBase = melIdx * binSize;
            float              sum        = 0.0f;

            for (unsigned int binIdx = 0; binIdx < binSize; ++binIdx) {
                sum += binPower[frameBase + binIdx] *
                       melFilterBank[filterBase + binIdx];
            }

            if (toDb) {
                sum = std::log10(std::fabs(sum));
            }

            mel[frameIdx * static_cast<unsigned int>(kMelBins) + melIdx] = sum;
        }
    }

    return mel;
}

void
CheckVectorsClose(const std::vector<float> &actual,
                  const std::vector<float> &expected,
                  const float               tolerance = kReferenceTolerance)
{
    REQUIRE(actual.size() == expected.size());

    for (std::size_t idx = 0; idx < actual.size(); ++idx) {
        if (std::isfinite(expected[idx])) {
            CHECK(std::isfinite(actual[idx]));
            CHECK(std::fabs(actual[idx] - expected[idx]) < tolerance);
            continue;
        }

        CHECK(std::isinf(actual[idx]));
        CHECK(std::signbit(actual[idx]) == std::signbit(expected[idx]));
    }
}

void
CheckUnitRange(const std::vector<float> &values)
{
    for (const float value : values) {
        CHECK(std::isfinite(value));
        CHECK(value >= 0.0f);
        CHECK(value <= 1.0f);
    }
}

void
CheckOpenclMatchesSerial(const std::vector<float>          &pcm,
                         const int                          windowSizeExp,
                         const float                        overlapRatio,
                         const PDJE_PARALLEL::POST_PROCESS &postProcess,
                         const float                        tolerance)
{
    auto serialPcm = pcm;
    auto openclPcm = pcm;

    PDJE_PARALLEL::STFT serialStft;
    serialStft.backend_now = PDJE_PARALLEL::BACKEND_T::SERIAL;
    const auto [expectedReal, expectedImag] =
        serialStft.calculate(serialPcm,
                             PDJE_PARALLEL::WINDOW_LIST::HANNING,
                             windowSizeExp,
                             overlapRatio,
                             postProcess);

    PDJE_PARALLEL::STFT openclStft;
    REQUIRE(openclStft.backend_now == PDJE_PARALLEL::BACKEND_T::OPENCL);
    const auto [actualReal, actualImag] =
        openclStft.calculate(openclPcm,
                             PDJE_PARALLEL::WINDOW_LIST::HANNING,
                             windowSizeExp,
                             overlapRatio,
                             postProcess);

    REQUIRE(openclStft.backend_now == PDJE_PARALLEL::BACKEND_T::OPENCL);
    REQUIRE(actualImag.empty() == expectedImag.empty());

    CheckVectorsClose(actualReal, expectedReal, tolerance);
    if (!expectedImag.empty()) {
        CheckVectorsClose(actualImag, expectedImag, tolerance);
    }
}

class ThrowingBackend final : public PDJE_PARALLEL::IStftBackend {
  public:
    PDJE_PARALLEL::StftResult
    Execute(std::vector<float> &,
            PDJE_PARALLEL::WINDOW_LIST,
            PDJE_PARALLEL::POST_PROCESS,
            unsigned int,
            const PDJE_PARALLEL::StftArgs &) override
    {
        throw std::runtime_error("Injected backend failure.");
    }
};

class EmptyBackend final : public PDJE_PARALLEL::IStftBackend {
  public:
    PDJE_PARALLEL::StftResult
    Execute(std::vector<float> &,
            PDJE_PARALLEL::WINDOW_LIST,
            PDJE_PARALLEL::POST_PROCESS,
            unsigned int,
            const PDJE_PARALLEL::StftArgs &) override
    {
        return {};
    }
};

} // namespace

TEST_CASE("backendless minmax normalization operates independently per chunk")
{
    std::vector<float> values{ 2.0f, 4.0f, 10.0f, 20.0f };

    PDJE_PARALLEL::Normalize_minmax(values, 2);

    CheckVectorsClose(values, { 0.0f, 1.0f, 0.0f, 1.0f }, 1.0e-6f);
}

TEST_CASE(
    "backendless minmax normalization handles partial and constant chunks")
{
    std::vector<float> values{ 1.0f, 2.0f, 3.0f, 5.0f, 5.0f };

    PDJE_PARALLEL::Normalize_minmax(values, 3);

    CheckVectorsClose(values, { 0.0f, 0.5f, 1.0f, 0.0f, 0.0f }, 1.0e-6f);
}

TEST_CASE("backendless minmax normalization ignores non-finite values")
{
    std::vector<float> values{ 1.0f,
                               std::numeric_limits<float>::infinity(),
                               3.0f,
                               std::numeric_limits<float>::quiet_NaN(),
                               -std::numeric_limits<float>::infinity(),
                               5.0f };

    PDJE_PARALLEL::Normalize_minmax(values, 3);

    CheckVectorsClose(values, { 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f }, 1.0e-6f);
}

TEST_CASE("backendless minmax normalization treats zero chunk size as no-op")
{
    std::vector<float> values{ 4.0f, 8.0f, 12.0f };
    std::vector<float> emptyValues;
    const auto         original = values;

    PDJE_PARALLEL::Normalize_minmax(values, 0);
    PDJE_PARALLEL::Normalize_minmax(emptyValues, 4);

    CHECK(values == original);
    CHECK(emptyValues.empty());
}

TEST_CASE("backendless rgb conversion averages equal mel thirds per frame")
{
    const std::vector<float> values{ 0.1f, 0.2f, 0.4f, 0.6f, 0.8f, 1.0f };

    const auto rgb = PDJE_PARALLEL::TO_RGB(values, 6);

    REQUIRE(rgb.size() == 3u);
    CheckVectorsClose(rgb, { 0.15f, 0.5f, 0.9f }, 1.0e-6f);
}

TEST_CASE("backendless rgb conversion emits one triplet per frame")
{
    const auto rgb =
        PDJE_PARALLEL::TO_RGB({ 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f }, 3);

    REQUIRE(rgb.size() == 6u);
    CheckVectorsClose(rgb, { 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f }, 1.0e-6f);
}

TEST_CASE(
    "backendless rgb conversion handles uneven thirds and non-finite bins")
{
    const std::vector<float> values{ 0.2f,
                                     0.4f,
                                     std::numeric_limits<float>::quiet_NaN(),
                                     std::numeric_limits<float>::infinity(),
                                     0.6f };

    const auto rgb = PDJE_PARALLEL::TO_RGB(values, 5);

    REQUIRE(rgb.size() == 3u);
    CheckVectorsClose(rgb, { 0.2f, 0.4f, 0.6f }, 1.0e-6f);
}

TEST_CASE("backendless rgb conversion rejects invalid mel layouts")
{
    CHECK(PDJE_PARALLEL::TO_RGB({}, 80).empty());
    CHECK(PDJE_PARALLEL::TO_RGB({ 0.1f, 0.2f }, 0).empty());
    CHECK(PDJE_PARALLEL::TO_RGB({ 0.1f, 0.2f }, 2).empty());
    CHECK(PDJE_PARALLEL::TO_RGB({ 0.1f, 0.2f, 0.3f, 0.4f }, 3).empty());
}

TEST_CASE("stft parallel calculate returns raw complex output by default")
{
    auto pcm                            = BuildSignal(128);
    auto [referenceReal, referenceImag] = ReferenceDftStft(pcm, 6, 0.5f);

    PDJE_PARALLEL::STFT stft;
    stft.backend_now = PDJE_PARALLEL::BACKEND_T::SERIAL;
    auto [realOut, imagOut] =
        stft.calculate(pcm, PDJE_PARALLEL::WINDOW_LIST::HANNING, 6, 0.5f);

    CheckVectorsClose(realOut, referenceReal);
    CheckVectorsClose(imagOut, referenceImag);
}

TEST_CASE("stft parallel calculate returns power-only output when requested")
{
    auto pcm                            = BuildSignal(128);
    auto [referenceReal, referenceImag] = ReferenceDftStft(pcm, 6, 0.5f);
    const auto referencePower = ReferencePower(referenceReal, referenceImag);

    PDJE_PARALLEL::POST_PROCESS postProcess;
    postProcess.toPower = true;

    PDJE_PARALLEL::STFT stft;
    stft.backend_now = PDJE_PARALLEL::BACKEND_T::SERIAL;

    auto [powerOut, imagOut] = stft.calculate(
        pcm, PDJE_PARALLEL::WINDOW_LIST::HANNING, 6, 0.5f, postProcess);

    CHECK(imagOut.empty());
    CheckVectorsClose(powerOut, referencePower);
}

TEST_CASE("stft parallel calculate returns bin-only complex output")
{
    auto pcm                            = BuildSignal(128);
    auto [referenceReal, referenceImag] = ReferenceDftStft(pcm, 6, 0.5f);
    auto [referenceBinReal, referenceBinImag] =
        ReferenceBinOnly(referenceReal, referenceImag, 6);

    PDJE_PARALLEL::POST_PROCESS postProcess;
    postProcess.to_bin = true;

    PDJE_PARALLEL::STFT stft;
    stft.backend_now = PDJE_PARALLEL::BACKEND_T::SERIAL;

    auto [binReal, binImag] = stft.calculate(
        pcm, PDJE_PARALLEL::WINDOW_LIST::HANNING, 6, 0.5f, postProcess);

    CheckVectorsClose(binReal, referenceBinReal);
    CheckVectorsClose(binImag, referenceBinImag);
}

TEST_CASE("stft parallel calculate returns mel-db chain output when requested")
{
    auto pcm                            = BuildSignal(128);
    auto [referenceReal, referenceImag] = ReferenceDftStft(pcm, 6, 0.5f);
    const auto referenceBinPower =
        ReferenceBinPower(referenceReal, referenceImag, 6);
    const auto referenceMel =
        ReferenceMelOutput(referenceBinPower,
                           1 << 6,
                           ReferenceToQuot(pcm.size(), 0.5f, 1 << 6),
                           true);

    PDJE_PARALLEL::POST_PROCESS postProcess;
    postProcess.to_bin    = true;
    postProcess.toPower   = true;
    postProcess.mel_scale = true;
    postProcess.to_db     = true;

    PDJE_PARALLEL::STFT stft;
    stft.backend_now = PDJE_PARALLEL::BACKEND_T::SERIAL;

    auto [melOut, imagOut] = stft.calculate(
        pcm, PDJE_PARALLEL::WINDOW_LIST::HANNING, 6, 0.5f, postProcess);

    CHECK(imagOut.empty());
    CheckVectorsClose(melOut, referenceMel, 8.0e-2f);
}

TEST_CASE("stft parallel calculate rejects unsupported window sizes")
{
    auto pcm = BuildSignal(128);

    PDJE_PARALLEL::POST_PROCESS postProcess;
    postProcess.toPower = true;

    PDJE_PARALLEL::STFT stft;
    stft.backend_now = PDJE_PARALLEL::BACKEND_T::SERIAL;

    const auto [realOut, imagOut] =
        stft.calculate(pcm, PDJE_PARALLEL::WINDOW_LIST::HANNING, 5, 0.5f);
    const auto [powerOut, powerImagOut] = stft.calculate(
        pcm, PDJE_PARALLEL::WINDOW_LIST::HANNING, 5, 0.5f, postProcess);

    CHECK(realOut.empty());
    CHECK(imagOut.empty());
    CHECK(powerOut.empty());
    CHECK(powerImagOut.empty());
}

TEST_CASE("stft serial backend returns stable results for repeated calls")
{
    auto pcm = BuildSignal(512);

    PDJE_PARALLEL::STFT stft;
    stft.backend_now = PDJE_PARALLEL::BACKEND_T::SERIAL;

    const auto [firstReal, firstImag] =
        stft.calculate(pcm, PDJE_PARALLEL::WINDOW_LIST::HANNING, 10, 0.5f);
    const auto [secondReal, secondImag] =
        stft.calculate(pcm, PDJE_PARALLEL::WINDOW_LIST::HANNING, 10, 0.5f);

    CheckVectorsClose(secondReal, firstReal);
    CheckVectorsClose(secondImag, firstImag);
}

TEST_CASE("stft serial backend survives switching between cached fft sizes")
{
    auto pcm = BuildSignal(5000);

    PDJE_PARALLEL::STFT cachedStft;
    cachedStft.backend_now = PDJE_PARALLEL::BACKEND_T::SERIAL;

    const auto [initialLargeReal, initialLargeImag] = cachedStft.calculate(
        pcm, PDJE_PARALLEL::WINDOW_LIST::HANNING, 12, 0.5f);
    const auto [smallReal, smallImag] =
        cachedStft.calculate(pcm, PDJE_PARALLEL::WINDOW_LIST::HANNING, 6, 0.5f);
    const auto [afterSwitchReal, afterSwitchImag] = cachedStft.calculate(
        pcm, PDJE_PARALLEL::WINDOW_LIST::HANNING, 12, 0.5f);

    PDJE_PARALLEL::STFT freshStft;
    freshStft.backend_now = PDJE_PARALLEL::BACKEND_T::SERIAL;
    const auto [freshLargeReal, freshLargeImag] =
        freshStft.calculate(pcm, PDJE_PARALLEL::WINDOW_LIST::HANNING, 12, 0.5f);

    CHECK_FALSE(initialLargeReal.empty());
    CHECK_FALSE(initialLargeImag.empty());
    CHECK_FALSE(smallReal.empty());
    CHECK_FALSE(smallImag.empty());

    CheckVectorsClose(afterSwitchReal, freshLargeReal);
    CheckVectorsClose(afterSwitchImag, freshLargeImag);
}

TEST_CASE("stft serial backend refreshes mel cache across fft sizes")
{
    auto pcm = BuildSignal(5000);

    PDJE_PARALLEL::POST_PROCESS postProcess;
    postProcess.mel_scale = true;
    postProcess.to_db     = true;

    PDJE_PARALLEL::STFT cachedStft;
    cachedStft.backend_now = PDJE_PARALLEL::BACKEND_T::SERIAL;

    const auto [firstMel, firstImag] = cachedStft.calculate(
        pcm, PDJE_PARALLEL::WINDOW_LIST::HANNING, 10, 0.5f, postProcess);
    const auto [secondMel, secondImag] = cachedStft.calculate(
        pcm, PDJE_PARALLEL::WINDOW_LIST::HANNING, 8, 0.5f, postProcess);
    const auto [thirdMel, thirdImag] = cachedStft.calculate(
        pcm, PDJE_PARALLEL::WINDOW_LIST::HANNING, 10, 0.5f, postProcess);

    PDJE_PARALLEL::STFT freshStft;
    freshStft.backend_now            = PDJE_PARALLEL::BACKEND_T::SERIAL;
    const auto [freshMel, freshImag] = freshStft.calculate(
        pcm, PDJE_PARALLEL::WINDOW_LIST::HANNING, 10, 0.5f, postProcess);

    CHECK(firstImag.empty());
    CHECK(secondImag.empty());
    CHECK(thirdImag.empty());
    CHECK(freshImag.empty());
    CHECK_FALSE(firstMel.empty());
    CHECK_FALSE(secondMel.empty());

    CheckVectorsClose(thirdMel, freshMel);
}

TEST_CASE("stft parallel falls back to serial when opencl backend fails")
{
    auto pcm = BuildSignal(128);

    PDJE_PARALLEL::STFT expectedStft;
    expectedStft.backend_now                = PDJE_PARALLEL::BACKEND_T::SERIAL;
    const auto [expectedReal, expectedImag] = expectedStft.calculate(
        pcm, PDJE_PARALLEL::WINDOW_LIST::HANNING, 6, 0.5f);

    SUBCASE("exception fallback")
    {
        PDJE_PARALLEL::STFT stft;
        stft.SetBackendForTesting(PDJE_PARALLEL::BACKEND_T::OPENCL,
                                  std::make_unique<ThrowingBackend>());

        const auto [realOut, imagOut] =
            stft.calculate(pcm, PDJE_PARALLEL::WINDOW_LIST::HANNING, 6, 0.5f);

        CHECK(stft.backend_now == PDJE_PARALLEL::BACKEND_T::SERIAL);
        CheckVectorsClose(realOut, expectedReal);
        CheckVectorsClose(imagOut, expectedImag);
    }

    SUBCASE("empty result fallback")
    {
        PDJE_PARALLEL::STFT stft;
        stft.SetBackendForTesting(PDJE_PARALLEL::BACKEND_T::OPENCL,
                                  std::make_unique<EmptyBackend>());

        const auto [realOut, imagOut] =
            stft.calculate(pcm, PDJE_PARALLEL::WINDOW_LIST::HANNING, 6, 0.5f);

        CHECK(stft.backend_now == PDJE_PARALLEL::BACKEND_T::SERIAL);
        CheckVectorsClose(realOut, expectedReal);
        CheckVectorsClose(imagOut, expectedImag);
    }
}

TEST_CASE("stft serial rgb output reduces mel frames into triplets")
{
    auto pcm = BuildSignal(128);

    PDJE_PARALLEL::POST_PROCESS rgbPostProcess;
    rgbPostProcess.to_rgb = true;

    PDJE_PARALLEL::POST_PROCESS melPostProcess;
    melPostProcess.to_bin            = true;
    melPostProcess.toPower           = true;
    melPostProcess.mel_scale         = true;
    melPostProcess.normalize_min_max = true;

    PDJE_PARALLEL::STFT rgbStft;
    rgbStft.backend_now              = PDJE_PARALLEL::BACKEND_T::SERIAL;
    const auto [firstRgb, firstImag] = rgbStft.calculate(
        pcm, PDJE_PARALLEL::WINDOW_LIST::HANNING, 6, 0.5f, rgbPostProcess);
    const auto [secondRgb, secondImag] = rgbStft.calculate(
        pcm, PDJE_PARALLEL::WINDOW_LIST::HANNING, 6, 0.5f, rgbPostProcess);

    PDJE_PARALLEL::STFT melStft;
    melStft.backend_now          = PDJE_PARALLEL::BACKEND_T::SERIAL;
    const auto [melOut, melImag] = melStft.calculate(
        pcm, PDJE_PARALLEL::WINDOW_LIST::HANNING, 6, 0.5f, melPostProcess);
    const auto expectedRgb = PDJE_PARALLEL::TO_RGB(melOut, kMelBins);

    REQUIRE(firstImag.empty());
    REQUIRE(secondImag.empty());
    REQUIRE(melImag.empty());
    REQUIRE(firstRgb.size() ==
            static_cast<std::size_t>(ReferenceToQuot(pcm.size(), 0.5f, 1 << 6) *
                                     3u));
    REQUIRE(firstRgb.size() == expectedRgb.size());

    CheckVectorsClose(firstRgb, expectedRgb, 1.0e-6f);
    CheckVectorsClose(secondRgb, firstRgb, 1.0e-6f);
    CheckUnitRange(firstRgb);
}

TEST_CASE(
    "stft opencl backend matches serial postprocess outputs when available")
{
    PDJE_PARALLEL::STFT openclProbe;
    if (openclProbe.backend_now != PDJE_PARALLEL::BACKEND_T::OPENCL) {
        return;
    }

    CheckOpenclMatchesSerial(BuildSignal(5000), 12, 0.5f, {}, 1.0e-3f);

    PDJE_PARALLEL::POST_PROCESS toPowerPostProcess;
    toPowerPostProcess.toPower = true;
    CheckOpenclMatchesSerial(
        BuildSignal(256), 6, 0.5f, toPowerPostProcess, 1.0e-3f);

    PDJE_PARALLEL::POST_PROCESS toBinPostProcess;
    toBinPostProcess.to_bin = true;
    CheckOpenclMatchesSerial(
        BuildSignal(256), 6, 0.5f, toBinPostProcess, 1.0e-3f);

    PDJE_PARALLEL::POST_PROCESS binPowerPostProcess;
    binPowerPostProcess.to_bin  = true;
    binPowerPostProcess.toPower = true;
    CheckOpenclMatchesSerial(
        BuildSignal(256), 6, 0.5f, binPowerPostProcess, 1.0e-3f);

    PDJE_PARALLEL::POST_PROCESS melPostProcess;
    melPostProcess.mel_scale = true;
    CheckOpenclMatchesSerial(
        BuildSignal(256), 6, 0.5f, melPostProcess, 1.0e-2f);

    PDJE_PARALLEL::POST_PROCESS melDbPostProcess;
    melDbPostProcess.mel_scale = true;
    melDbPostProcess.to_db     = true;
    CheckOpenclMatchesSerial(
        BuildSignal(256), 6, 0.5f, melDbPostProcess, 1.0e-2f);

    PDJE_PARALLEL::POST_PROCESS toDbPostProcess;
    toDbPostProcess.to_db = true;
    CheckOpenclMatchesSerial(
        BuildSignal(256), 6, 0.5f, toDbPostProcess, 1.0e-3f);

    PDJE_PARALLEL::POST_PROCESS rgbPostProcess;
    rgbPostProcess.to_rgb = true;
    CheckOpenclMatchesSerial(
        BuildSignal(256), 6, 0.5f, rgbPostProcess, 1.0e-2f);
}

TEST_CASE("mel filter bank generates contiguous mel-major triangular rows")
{
    constexpr int sampleRate = 16000;
    constexpr int nFft       = 16;
    constexpr int nMels      = 4;
    constexpr int freqBins   = (nFft / 2) + 1;

    const auto implicitNyquist =
        PDJE_PARALLEL::GenMelFilterBank(sampleRate, nFft, nMels);
    const auto explicitNyquist = PDJE_PARALLEL::GenMelFilterBank(
        sampleRate, nFft, nMels, 0.0f, static_cast<float>(sampleRate) * 0.5f);

    REQUIRE(implicitNyquist.size() ==
            static_cast<std::size_t>(nMels * freqBins));
    REQUIRE(implicitNyquist == explicitNyquist);

    for (int melIdx = 0; melIdx < nMels; ++melIdx) {
        bool  sawPositive      = false;
        bool  sawDescent       = false;
        int   firstPositiveBin = -1;
        int   lastPositiveBin  = -1;
        float previousWeight   = 0.0f;

        for (int binIdx = 0; binIdx < freqBins; ++binIdx) {
            const std::size_t flatIdx =
                static_cast<std::size_t>(melIdx * freqBins + binIdx);
            const float weight = implicitNyquist[flatIdx];

            CHECK(std::isfinite(weight));
            CHECK(weight >= 0.0f);
            CHECK(weight <= 1.0f);

            if (weight > 0.0f) {
                if (!sawPositive) {
                    firstPositiveBin = binIdx;
                    sawPositive      = true;
                }
                lastPositiveBin = binIdx;

                if (weight + 1.0e-6f < previousWeight) {
                    sawDescent = true;
                }
            }

            previousWeight = weight;
        }

        CHECK(sawPositive);
        REQUIRE(firstPositiveBin >= 0);
        REQUIRE(lastPositiveBin >= firstPositiveBin);

        for (int binIdx = 0; binIdx < firstPositiveBin; ++binIdx) {
            const std::size_t flatIdx =
                static_cast<std::size_t>(melIdx * freqBins + binIdx);
            CHECK(implicitNyquist[flatIdx] == doctest::Approx(0.0f));
        }

        for (int binIdx = lastPositiveBin + 1; binIdx < freqBins; ++binIdx) {
            const std::size_t flatIdx =
                static_cast<std::size_t>(melIdx * freqBins + binIdx);
            CHECK(implicitNyquist[flatIdx] == doctest::Approx(0.0f));
        }

        if (lastPositiveBin > firstPositiveBin) {
            CHECK(sawDescent);
        }
    }
}

TEST_CASE("mel filter bank rejects invalid arguments")
{
    CHECK(PDJE_PARALLEL::GenMelFilterBank(0, 16, 4).empty());
    CHECK(PDJE_PARALLEL::GenMelFilterBank(16000, 0, 4).empty());
    CHECK(PDJE_PARALLEL::GenMelFilterBank(16000, 16, 0).empty());
    CHECK(
        PDJE_PARALLEL::GenMelFilterBank(16000, 16, 4, -1.0f, 8000.0f).empty());
    CHECK(PDJE_PARALLEL::GenMelFilterBank(16000, 16, 4, 4000.0f, 3000.0f)
              .empty());
    CHECK(PDJE_PARALLEL::GenMelFilterBank(16000, 16, 4, 0.0f, 9000.0f).empty());
    CHECK(PDJE_PARALLEL::GenMelFilterBank(
              16000, 16, std::numeric_limits<int>::max())
              .empty());
    CHECK(PDJE_PARALLEL::GenMelFilterBank(
              16000,
              16,
              4,
              0.0f,
              8000.0f,
              static_cast<PDJE_PARALLEL::MelFormula>(99),
              PDJE_PARALLEL::MelNorm::None)
              .empty());
    CHECK(
        PDJE_PARALLEL::GenMelFilterBank(16000,
                                        16,
                                        4,
                                        0.0f,
                                        8000.0f,
                                        PDJE_PARALLEL::MelFormula::HTK,
                                        static_cast<PDJE_PARALLEL::MelNorm>(99))
            .empty());
}

TEST_CASE("mel filter bank supports slaney formula and normalization modes")
{
    constexpr int   sampleRate = 22050;
    constexpr int   nFft       = 512;
    constexpr int   nMels      = 16;
    constexpr int   freqBins   = (nFft / 2) + 1;
    constexpr float epsilon    = 1.0e-5f;

    const auto htkNone =
        PDJE_PARALLEL::GenMelFilterBank(sampleRate,
                                        nFft,
                                        nMels,
                                        0.0f,
                                        -1.0f,
                                        PDJE_PARALLEL::MelFormula::HTK,
                                        PDJE_PARALLEL::MelNorm::None);
    const auto htkPeak =
        PDJE_PARALLEL::GenMelFilterBank(sampleRate,
                                        nFft,
                                        nMels,
                                        0.0f,
                                        -1.0f,
                                        PDJE_PARALLEL::MelFormula::HTK,
                                        PDJE_PARALLEL::MelNorm::Peak);
    const auto htkSlaneyNorm =
        PDJE_PARALLEL::GenMelFilterBank(sampleRate,
                                        nFft,
                                        nMels,
                                        0.0f,
                                        -1.0f,
                                        PDJE_PARALLEL::MelFormula::HTK,
                                        PDJE_PARALLEL::MelNorm::Slaney);
    const auto slaneyPeak =
        PDJE_PARALLEL::GenMelFilterBank(sampleRate,
                                        nFft,
                                        nMels,
                                        0.0f,
                                        -1.0f,
                                        PDJE_PARALLEL::MelFormula::Slaney,
                                        PDJE_PARALLEL::MelNorm::Peak);

    REQUIRE(htkNone.size() == static_cast<std::size_t>(nMels * freqBins));
    REQUIRE(htkPeak.size() == htkNone.size());
    REQUIRE(htkSlaneyNorm.size() == htkNone.size());
    REQUIRE(slaneyPeak.size() == htkNone.size());

    bool sawFormulaDifference = false;
    bool sawNormDifference    = false;

    for (std::size_t idx = 0; idx < htkNone.size(); ++idx) {
        CHECK(std::isfinite(htkPeak[idx]));
        CHECK(std::isfinite(htkSlaneyNorm[idx]));
        CHECK(std::isfinite(slaneyPeak[idx]));
        CHECK(htkPeak[idx] >= 0.0f);
        CHECK(htkSlaneyNorm[idx] >= 0.0f);
        CHECK(slaneyPeak[idx] >= 0.0f);

        if (std::fabs(htkNone[idx] - slaneyPeak[idx]) > epsilon) {
            sawFormulaDifference = true;
        }

        if (std::fabs(htkNone[idx] - htkSlaneyNorm[idx]) > epsilon) {
            sawNormDifference = true;
        }
    }

    CHECK(sawFormulaDifference);
    CHECK(sawNormDifference);

    for (int melIdx = 0; melIdx < nMels; ++melIdx) {
        float peakValue = 0.0f;
        for (int binIdx = 0; binIdx < freqBins; ++binIdx) {
            const float weight =
                htkPeak[static_cast<std::size_t>(melIdx * freqBins + binIdx)];
            if (weight > peakValue) {
                peakValue = weight;
            }
        }

        CHECK(peakValue == doctest::Approx(1.0f));
    }
}
