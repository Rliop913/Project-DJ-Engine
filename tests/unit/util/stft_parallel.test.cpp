#include "util/function/stft/BackendLess.hpp"
#include "util/function/stft/MelFilterBank.hpp"
#include "util/function/stft/STFT_Parallel.hpp"
#include "support/StftTestHarness.hpp"

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
constexpr float kOpenclAbsoluteFloorTolerance = 1.2e-2f;
constexpr float kOpenclRelativeTolerance = 2.5e-4f;
constexpr int   kLegacyMelBins      = 80;
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
                 const float               overlapRatio,
                 const bool                dcRemove = true)
{
    const unsigned int windowSize = 1u << windowSizeExp;
    auto overlapped = BuildReferenceOverlap(pcm, windowSizeExp, overlapRatio);
    std::vector<float> realOut(overlapped.size(), 0.0f);
    std::vector<float> imagOut(overlapped.size(), 0.0f);

    if (dcRemove) {
        ApplyReferenceDcRemove(overlapped, windowSize);
    }
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
BuildExactReferenceOverlap(const std::vector<float> &pcm,
                           const int                 windowSize,
                           const unsigned int        hopLength)
{
    if (hopLength == 0u || pcm.size() < static_cast<std::size_t>(windowSize)) {
        return {};
    }

    const unsigned int frameCount = static_cast<unsigned int>(
        ((pcm.size() - static_cast<std::size_t>(windowSize)) / hopLength) + 1u);
    std::vector<float> overlapped(
        static_cast<std::size_t>(frameCount) * static_cast<std::size_t>(windowSize),
        0.0f);

    for (unsigned int frameIdx = 0; frameIdx < frameCount; ++frameIdx) {
        const std::size_t originBase =
            static_cast<std::size_t>(frameIdx) * static_cast<std::size_t>(hopLength);
        const std::size_t outputBase =
            static_cast<std::size_t>(frameIdx) * static_cast<std::size_t>(windowSize);
        for (int sampleIdx = 0; sampleIdx < windowSize; ++sampleIdx) {
            overlapped[outputBase + static_cast<std::size_t>(sampleIdx)] =
                pcm[originBase + static_cast<std::size_t>(sampleIdx)];
        }
    }

    return overlapped;
}

PDJE_PARALLEL::StftResult
ReferenceDftStftExact(const std::vector<float> &pcm,
                      const int                 windowSize,
                      const unsigned int        hopLength,
                      const bool                dcRemove = true)
{
    auto overlapped = BuildExactReferenceOverlap(pcm, windowSize, hopLength);
    std::vector<float> realOut(overlapped.size(), 0.0f);
    std::vector<float> imagOut(overlapped.size(), 0.0f);

    if (dcRemove) {
        ApplyReferenceDcRemove(overlapped,
                               static_cast<unsigned int>(windowSize));
    }
    ApplyReferenceHanning(overlapped, static_cast<unsigned int>(windowSize));

    for (unsigned int frameBase = 0; frameBase < overlapped.size();
         frameBase += static_cast<unsigned int>(windowSize)) {
        for (unsigned int k = 0; k < static_cast<unsigned int>(windowSize); ++k) {
            float realSum = 0.0f;
            float imagSum = 0.0f;

            for (unsigned int n = 0; n < static_cast<unsigned int>(windowSize); ++n) {
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
                                        kLegacyMelBins,
                                        0.0f,
                                        -1.0f,
                                        PDJE_PARALLEL::MelFormula::Slaney,
                                        PDJE_PARALLEL::MelNorm::Slaney);

    std::vector<float> mel(static_cast<std::size_t>(qtConst) *
                               static_cast<std::size_t>(kLegacyMelBins),
                           0.0f);

    for (unsigned int frameIdx = 0; frameIdx < qtConst; ++frameIdx) {
        const unsigned int frameBase = frameIdx * binSize;

        for (unsigned int melIdx = 0;
             melIdx < static_cast<unsigned int>(kLegacyMelBins);
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

            mel[frameIdx * static_cast<unsigned int>(kLegacyMelBins) + melIdx] =
                sum;
        }
    }

    return mel;
}

std::vector<float>
ReferenceMelOutput(const std::vector<float>             &binPower,
                   const PDJE_PARALLEL::MelFilterBankSpec &melSpec,
                   const unsigned int                    qtConst,
                   const bool                            toDb)
{
    const unsigned int binSize =
        static_cast<unsigned int>((melSpec.n_fft >> 1) + 1);
    const auto melFilterBank = PDJE_PARALLEL::GenMelFilterBank(melSpec);

    std::vector<float> mel(static_cast<std::size_t>(qtConst) *
                               static_cast<std::size_t>(melSpec.n_mels),
                           0.0f);

    for (unsigned int frameIdx = 0; frameIdx < qtConst; ++frameIdx) {
        const unsigned int frameBase = frameIdx * binSize;

        for (unsigned int melIdx = 0;
             melIdx < static_cast<unsigned int>(melSpec.n_mels);
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

            mel[frameIdx * static_cast<unsigned int>(melSpec.n_mels) + melIdx] =
                sum;
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
CheckVectorsCloseWithRelativeTolerance(
    const std::vector<float> &actual,
    const std::vector<float> &expected,
    const float               absoluteTolerance,
    const float               relativeTolerance)
{
    REQUIRE(actual.size() == expected.size());

    for (std::size_t idx = 0; idx < actual.size(); ++idx) {
        if (std::isfinite(expected[idx])) {
            CHECK(std::isfinite(actual[idx]));

            const float scale = std::max(std::fabs(actual[idx]),
                                         std::fabs(expected[idx]));
            const float tolerance = std::max(
                std::max(absoluteTolerance, kOpenclAbsoluteFloorTolerance),
                scale * relativeTolerance);
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

std::array<float, 3>
RgbTriplet(const std::vector<float> &values, const std::size_t frameIndex)
{
    const std::size_t base = frameIndex * 3u;
    return { values[base + 0u], values[base + 1u], values[base + 2u] };
}

void
CheckDominantChannel(const std::array<float, 3> &rgb,
                     const std::size_t           dominantIndex)
{
    REQUIRE(dominantIndex < rgb.size());

    for (std::size_t idx = 0; idx < rgb.size(); ++idx) {
        if (idx == dominantIndex) {
            continue;
        }

        CHECK(rgb[dominantIndex] > rgb[idx]);
    }
}

float
ComputeLuminance(const std::array<float, 3> &rgb)
{
    return (0.2126f * rgb[0]) + (0.7152f * rgb[1]) + (0.0722f * rgb[2]);
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

    PDJE_TEST::util::SerialStftHarness serialStft;
    const auto [expectedReal, expectedImag] =
        serialStft.calculate(serialPcm,
                             PDJE_PARALLEL::WINDOW_LIST::HANNING,
                             windowSizeExp,
                             overlapRatio,
                             postProcess);

    PDJE_TEST::util::OpenclStftHarness openclStft;
    REQUIRE(openclStft.available());
    const auto [actualReal, actualImag] =
        openclStft.calculate(openclPcm,
                             PDJE_PARALLEL::WINDOW_LIST::HANNING,
                             windowSizeExp,
                             overlapRatio,
                             postProcess);

    REQUIRE(actualImag.empty() == expectedImag.empty());

    CheckVectorsCloseWithRelativeTolerance(actualReal,
                                          expectedReal,
                                          tolerance,
                                          kOpenclRelativeTolerance);
    if (!expectedImag.empty()) {
        CheckVectorsCloseWithRelativeTolerance(actualImag,
                                              expectedImag,
                                              tolerance,
                                              kOpenclRelativeTolerance);
    }
}

void
CheckOpenclMatchesSerial(const std::vector<float>     &pcm,
                         const PDJE_PARALLEL::STFTRequest &request,
                         const float                   tolerance)
{
    auto serialPcm = pcm;
    auto openclPcm = pcm;

    PDJE_TEST::util::SerialStftHarness serialStft;
    const auto [expectedReal, expectedImag] =
        serialStft.calculate(serialPcm, request);

    PDJE_TEST::util::OpenclStftHarness openclStft;
    REQUIRE(openclStft.available());
    const auto [actualReal, actualImag] = openclStft.calculate(openclPcm, request);

    REQUIRE(actualImag.empty() == expectedImag.empty());
    CheckVectorsCloseWithRelativeTolerance(actualReal,
                                          expectedReal,
                                          tolerance,
                                          kOpenclRelativeTolerance);
    if (!expectedImag.empty()) {
        CheckVectorsCloseWithRelativeTolerance(actualImag,
                                              expectedImag,
                                              tolerance,
                                              kOpenclRelativeTolerance);
    }
}

class ThrowingBackend final : public PDJE_PARALLEL::detail::IStftBackend {
  public:
    PDJE_PARALLEL::StftResult
    Execute(std::vector<float> &,
            PDJE_PARALLEL::WINDOW_LIST,
            PDJE_PARALLEL::POST_PROCESS,
            unsigned int,
            const PDJE_PARALLEL::detail::StftArgs &) override
    {
        throw std::runtime_error("Injected backend failure.");
    }
};

class EmptyBackend final : public PDJE_PARALLEL::detail::IStftBackend {
  public:
    PDJE_PARALLEL::StftResult
    Execute(std::vector<float> &,
            PDJE_PARALLEL::WINDOW_LIST,
            PDJE_PARALLEL::POST_PROCESS,
            unsigned int,
            const PDJE_PARALLEL::detail::StftArgs &) override
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

TEST_CASE("post process rgb mode no longer auto-enables minmax normalization")
{
    PDJE_PARALLEL::POST_PROCESS postProcess;
    postProcess.to_rgb = true;

    postProcess.check_values();

    CHECK(postProcess.to_rgb);
    CHECK(postProcess.mel_scale);
    CHECK(postProcess.to_bin);
    CHECK(postProcess.toPower);
    CHECK_FALSE(postProcess.normalize_min_max);
}

TEST_CASE("backendless rgb conversion emits one triplet per frame")
{
    const auto rgb =
        PDJE_PARALLEL::TO_RGB({ 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f }, 3);

    REQUIRE(rgb.size() == 6u);
    CheckUnitRange(rgb);
    CheckDominantChannel(RgbTriplet(rgb, 0u), 0u);
    CheckDominantChannel(RgbTriplet(rgb, 1u), 1u);
}

TEST_CASE("backendless rgb conversion maps low-only energy to red")
{
    const std::vector<float> values{ 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                                     0.0f, 0.0f, 0.0f, 0.0f, 0.0f };

    const auto rgb = PDJE_PARALLEL::TO_RGB(values, 10);

    REQUIRE(rgb.size() == 3u);
    CheckUnitRange(rgb);
    const auto triplet = RgbTriplet(rgb, 0u);
    CheckDominantChannel(triplet, 0u);
    CHECK(triplet[1] > 0.05f);
    CHECK(triplet[2] > 0.05f);
}

TEST_CASE("backendless rgb conversion maps mid-only energy to green")
{
    const std::vector<float> values{ 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
                                     0.0f, 0.0f, 0.0f, 0.0f, 0.0f };

    const auto rgb = PDJE_PARALLEL::TO_RGB(values, 10);

    REQUIRE(rgb.size() == 3u);
    CheckUnitRange(rgb);
    CheckDominantChannel(RgbTriplet(rgb, 0u), 1u);
}

TEST_CASE("backendless rgb conversion maps high-only energy to blue")
{
    const std::vector<float> values{ 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
                                     1.0f, 1.0f, 1.0f, 1.0f, 1.0f };

    const auto rgb = PDJE_PARALLEL::TO_RGB(values, 10);

    REQUIRE(rgb.size() == 3u);
    CheckUnitRange(rgb);
    const auto triplet = RgbTriplet(rgb, 0u);
    CheckDominantChannel(triplet, 2u);
    CHECK(triplet[0] > 0.05f);
    CHECK(triplet[1] > 0.05f);
}

TEST_CASE(
    "backendless rgb conversion keeps equal-energy frames within bright "
    "blue-leaning pastel range")
{
    const std::vector<float> values(10, 1.0f);

    const auto rgb = PDJE_PARALLEL::TO_RGB(values, 10);

    REQUIRE(rgb.size() == 3u);
    CheckUnitRange(rgb);
    const auto triplet = RgbTriplet(rgb, 0u);
    CHECK(triplet[0] > 0.35f);
    CHECK(triplet[1] > 0.35f);
    CHECK(triplet[2] > 0.35f);
    CHECK((std::max({ triplet[0], triplet[1], triplet[2] }) -
           std::min({ triplet[0], triplet[1], triplet[2] })) < 0.22f);
    CHECK(triplet[2] >= triplet[1]);
    CHECK(triplet[1] >= triplet[0]);
}

TEST_CASE("backendless rgb conversion keeps blue visible in low-high mixes")
{
    const std::vector<float> values{ 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
                                     1.0f, 1.0f, 1.0f, 1.0f, 1.0f };

    const auto rgb = PDJE_PARALLEL::TO_RGB(values, 10);

    REQUIRE(rgb.size() == 3u);
    CheckUnitRange(rgb);
    const auto triplet = RgbTriplet(rgb, 0u);
    CHECK(triplet[2] > 0.45f);
    CHECK(triplet[2] > triplet[1]);
}

TEST_CASE("backendless rgb conversion handles negative and non-finite mel bins")
{
    const std::vector<float> values{ -1.0f,
                                     -1.0f,
                                     std::numeric_limits<float>::quiet_NaN(),
                                     std::numeric_limits<float>::infinity(),
                                     0.0f,
                                     0.0f,
                                     0.0f,
                                     0.0f,
                                     0.25f,
                                     0.5f };

    const auto rgb = PDJE_PARALLEL::TO_RGB(values, 10);

    REQUIRE(rgb.size() == 3u);
    CheckUnitRange(rgb);
    CheckDominantChannel(RgbTriplet(rgb, 0u), 2u);
}

TEST_CASE("backendless rgb conversion flattens brightness for same-spectrum frames")
{
    const std::vector<float> values{
        0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f,
        1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    };

    const auto rgb = PDJE_PARALLEL::TO_RGB(values, 10);

    REQUIRE(rgb.size() == 6u);
    CheckUnitRange(rgb);

    const float lowLuminance  = ComputeLuminance(RgbTriplet(rgb, 0u));
    const float highLuminance = ComputeLuminance(RgbTriplet(rgb, 1u));

    CHECK(lowLuminance > 0.0f);
    CHECK(highLuminance >= lowLuminance);
    CHECK((highLuminance / lowLuminance) <= 2.5f);
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

    PDJE_TEST::util::SerialStftHarness stft;
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

    PDJE_TEST::util::SerialStftHarness stft;

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

    PDJE_TEST::util::SerialStftHarness stft;

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

    PDJE_TEST::util::SerialStftHarness stft;

    auto [melOut, imagOut] = stft.calculate(
        pcm, PDJE_PARALLEL::WINDOW_LIST::HANNING, 6, 0.5f, postProcess);

    CHECK(imagOut.empty());
    CheckVectorsClose(melOut, referenceMel, 8.0e-2f);
}

TEST_CASE("stft request exact-windowed returns beat-this linear mel output")
{
    auto pcm = BuildSignal(4096);

    PDJE_PARALLEL::POST_PROCESS postProcess;
    postProcess.to_bin    = true;
    postProcess.toPower   = true;
    postProcess.mel_scale = true;

    auto request = PDJE_TEST::util::BuildBeatThisRequest(postProcess);

    auto [referenceReal, referenceImag] =
        ReferenceDftStftExact(pcm, request.n_fft, request.hop_length);
    const auto referenceBinPower =
        ReferenceBinPower(referenceReal,
                          referenceImag,
                          static_cast<int>(std::log2(request.n_fft)));
    const auto referenceMel = ReferenceMelOutput(referenceBinPower,
                                                 request.mel_filter_bank.value(),
                                                 static_cast<unsigned int>(
                                                     ((pcm.size() -
                                                       static_cast<std::size_t>(
                                                           request.n_fft)) /
                                                      request.hop_length) +
                                                     1u),
                                                 false);

    PDJE_TEST::util::SerialStftHarness stft;
    const auto [melOut, imagOut] = stft.calculate(pcm, request);

    CHECK(imagOut.empty());
    CheckVectorsClose(melOut, referenceMel, 8.0e-2f);
}

TEST_CASE("stft request uses custom mel filter bank bin counts")
{
    auto pcm = BuildSignal(2048);

    PDJE_PARALLEL::POST_PROCESS postProcess;
    postProcess.to_bin    = true;
    postProcess.toPower   = true;
    postProcess.mel_scale = true;

    const PDJE_PARALLEL::MelFilterBankSpec melSpec{
        .sample_rate = 32000,
        .n_fft = 64,
        .n_mels = 24,
        .f_min = 60.0f,
        .f_max = 12000.0f,
        .mel_formula = PDJE_PARALLEL::MelFormula::HTK,
        .norm = PDJE_PARALLEL::MelNorm::Peak,
    };
    const PDJE_PARALLEL::STFTRequest request{
        .sample_rate = melSpec.sample_rate,
        .n_fft = melSpec.n_fft,
        .hop_length = 16u,
        .target_window = PDJE_PARALLEL::WINDOW_LIST::HANNING,
        .post_process = postProcess,
        .frame_policy = PDJE_PARALLEL::FRAME_POLICY::EXACT_WINDOWED,
        .mel_filter_bank = melSpec,
    };

    auto [referenceReal, referenceImag] =
        ReferenceDftStftExact(pcm, request.n_fft, request.hop_length);
    const auto referenceBinPower =
        ReferenceBinPower(referenceReal,
                          referenceImag,
                          static_cast<int>(std::log2(request.n_fft)));
    const auto frameCount = static_cast<unsigned int>(
        ((pcm.size() - static_cast<std::size_t>(request.n_fft)) / request.hop_length) +
        1u);
    const auto referenceMel =
        ReferenceMelOutput(referenceBinPower, melSpec, frameCount, false);

    PDJE_TEST::util::SerialStftHarness stft;
    const auto [melOut, imagOut] = stft.calculate(pcm, request);

    CHECK(imagOut.empty());
    REQUIRE(melOut.size() ==
            static_cast<std::size_t>(frameCount) *
                static_cast<std::size_t>(melSpec.n_mels));
    CheckVectorsClose(melOut, referenceMel, 8.0e-2f);
}

TEST_CASE("legacy stft mel overload still defaults to 80 mel bins")
{
    auto pcm = BuildSignal(256);

    PDJE_PARALLEL::POST_PROCESS postProcess;
    postProcess.mel_scale = true;

    PDJE_TEST::util::SerialStftHarness stft;
    const auto [melOut, imagOut] = stft.calculate(
        pcm, PDJE_PARALLEL::WINDOW_LIST::HANNING, 6, 0.5f, postProcess);

    CHECK(imagOut.empty());
    REQUIRE(melOut.size() ==
            static_cast<std::size_t>(ReferenceToQuot(pcm.size(), 0.5f, 1 << 6)) *
                static_cast<std::size_t>(kLegacyMelBins));
}

TEST_CASE("stft request dc_remove defaults on and can be disabled")
{
    auto pcm = BuildSignal(256);
    for (float &sample : pcm) {
        sample += 0.75f;
    }

    PDJE_PARALLEL::STFTRequest request;
    request.sample_rate  = 16000;
    request.n_fft        = 64;
    request.hop_length   = 16u;
    request.target_window = PDJE_PARALLEL::WINDOW_LIST::HANNING;
    request.frame_policy = PDJE_PARALLEL::FRAME_POLICY::EXACT_WINDOWED;

    const auto [referenceDefaultReal, referenceDefaultImag] =
        ReferenceDftStftExact(pcm, request.n_fft, request.hop_length, true);

    PDJE_TEST::util::SerialStftHarness stft;
    const auto [defaultReal, defaultImag] = stft.calculate(pcm, request);

    CheckVectorsClose(defaultReal, referenceDefaultReal);
    CheckVectorsClose(defaultImag, referenceDefaultImag);

    request.dc_remove = false;
    const auto [referenceRawReal, referenceRawImag] =
        ReferenceDftStftExact(pcm, request.n_fft, request.hop_length, false);
    const auto [rawReal, rawImag] = stft.calculate(pcm, request);

    CheckVectorsClose(rawReal, referenceRawReal);
    CheckVectorsClose(rawImag, referenceRawImag);

    bool sawDifference = false;
    for (std::size_t idx = 0; idx < defaultReal.size(); ++idx) {
        if (std::fabs(defaultReal[idx] - rawReal[idx]) > 1.0e-4f) {
            sawDifference = true;
            break;
        }
    }
    CHECK(sawDifference);
}

TEST_CASE("stft parallel calculate rejects unsupported window sizes")
{
    auto pcm = BuildSignal(128);

    PDJE_PARALLEL::POST_PROCESS postProcess;
    postProcess.toPower = true;

    PDJE_TEST::util::SerialStftHarness stft;

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

    PDJE_TEST::util::SerialStftHarness stft;

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

    PDJE_TEST::util::SerialStftHarness cachedStft;

    const auto [initialLargeReal, initialLargeImag] = cachedStft.calculate(
        pcm, PDJE_PARALLEL::WINDOW_LIST::HANNING, 12, 0.5f);
    const auto [smallReal, smallImag] =
        cachedStft.calculate(pcm, PDJE_PARALLEL::WINDOW_LIST::HANNING, 6, 0.5f);
    const auto [afterSwitchReal, afterSwitchImag] = cachedStft.calculate(
        pcm, PDJE_PARALLEL::WINDOW_LIST::HANNING, 12, 0.5f);

    PDJE_TEST::util::SerialStftHarness freshStft;
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

    PDJE_TEST::util::SerialStftHarness cachedStft;

    const auto [firstMel, firstImag] = cachedStft.calculate(
        pcm, PDJE_PARALLEL::WINDOW_LIST::HANNING, 10, 0.5f, postProcess);
    const auto [secondMel, secondImag] = cachedStft.calculate(
        pcm, PDJE_PARALLEL::WINDOW_LIST::HANNING, 8, 0.5f, postProcess);
    const auto [thirdMel, thirdImag] = cachedStft.calculate(
        pcm, PDJE_PARALLEL::WINDOW_LIST::HANNING, 10, 0.5f, postProcess);

    PDJE_TEST::util::SerialStftHarness freshStft;
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

    PDJE_TEST::util::SerialStftHarness expectedStft;
    const auto [expectedReal, expectedImag] = expectedStft.calculate(
        pcm, PDJE_PARALLEL::WINDOW_LIST::HANNING, 6, 0.5f);

    SUBCASE("exception fallback")
    {
        PDJE_TEST::util::FallbackStftHarness stft;
        stft.set_opencl_backend(std::make_unique<ThrowingBackend>());

        const auto [realOut, imagOut] =
            stft.calculate(pcm, PDJE_PARALLEL::WINDOW_LIST::HANNING, 6, 0.5f);

        CHECK(stft.active_backend() == PDJE_PARALLEL::BACKEND_T::SERIAL);
        CheckVectorsClose(realOut, expectedReal);
        CheckVectorsClose(imagOut, expectedImag);
    }

    SUBCASE("empty result fallback")
    {
        PDJE_TEST::util::FallbackStftHarness stft;
        stft.set_opencl_backend(std::make_unique<EmptyBackend>());

        const auto [realOut, imagOut] =
            stft.calculate(pcm, PDJE_PARALLEL::WINDOW_LIST::HANNING, 6, 0.5f);

        CHECK(stft.active_backend() == PDJE_PARALLEL::BACKEND_T::SERIAL);
        CheckVectorsClose(realOut, expectedReal);
        CheckVectorsClose(imagOut, expectedImag);
    }
}

TEST_CASE("stft request opencl backend matches serial output when available")
{
    if (PDJE_PARALLEL::STFT::detect_available_backend() !=
        PDJE_PARALLEL::BACKEND_T::OPENCL) {
        INFO("OpenCL backend is not available on this host");
        return;
    }

    PDJE_PARALLEL::POST_PROCESS postProcess;
    postProcess.to_bin    = true;
    postProcess.toPower   = true;
    postProcess.mel_scale = true;

    CheckOpenclMatchesSerial(
        BuildSignal(4096),
        PDJE_TEST::util::BuildBeatThisRequest(postProcess),
        1.0e-2f);

    auto noDcRequest = PDJE_TEST::util::BuildBeatThisRequest(postProcess);
    noDcRequest.dc_remove = false;
    CheckOpenclMatchesSerial(BuildSignal(4096), noDcRequest, 1.0e-2f);

    const PDJE_PARALLEL::MelFilterBankSpec customMelSpec{
        .sample_rate = 32000,
        .n_fft = 64,
        .n_mels = 24,
        .f_min = 60.0f,
        .f_max = 12000.0f,
        .mel_formula = PDJE_PARALLEL::MelFormula::HTK,
        .norm = PDJE_PARALLEL::MelNorm::Peak,
    };
    const PDJE_PARALLEL::STFTRequest customRequest{
        .sample_rate = customMelSpec.sample_rate,
        .n_fft = customMelSpec.n_fft,
        .hop_length = 16u,
        .target_window = PDJE_PARALLEL::WINDOW_LIST::HANNING,
        .post_process = postProcess,
        .frame_policy = PDJE_PARALLEL::FRAME_POLICY::EXACT_WINDOWED,
        .mel_filter_bank = customMelSpec,
    };
    CheckOpenclMatchesSerial(BuildSignal(2048), customRequest, 1.0e-2f);
}

TEST_CASE("stft public api exposes backend query without internal seams")
{
    PDJE_PARALLEL::STFT stft;
    const bool hasSupportedBackend =
        stft.active_backend() == PDJE_PARALLEL::BACKEND_T::SERIAL ||
        stft.active_backend() == PDJE_PARALLEL::BACKEND_T::OPENCL;

    CHECK(hasSupportedBackend);
    CHECK(PDJE_PARALLEL::STFT::detect_available_backend() ==
          PDJE_TEST::util::DetectAvailableBackend());
}

TEST_CASE("stft serial rgb output reduces mel frames into triplets")
{
    auto pcm = BuildSignal(128);

    PDJE_PARALLEL::POST_PROCESS rgbPostProcess;
    rgbPostProcess.to_rgb = true;

    PDJE_PARALLEL::POST_PROCESS melPostProcess;
    melPostProcess.to_bin    = true;
    melPostProcess.toPower   = true;
    melPostProcess.mel_scale = true;

    PDJE_TEST::util::SerialStftHarness rgbStft;
    const auto [firstRgb, firstImag] = rgbStft.calculate(
        pcm, PDJE_PARALLEL::WINDOW_LIST::HANNING, 6, 0.5f, rgbPostProcess);
    const auto [secondRgb, secondImag] = rgbStft.calculate(
        pcm, PDJE_PARALLEL::WINDOW_LIST::HANNING, 6, 0.5f, rgbPostProcess);

    PDJE_TEST::util::SerialStftHarness melStft;
    const auto [melOut, melImag] = melStft.calculate(
        pcm, PDJE_PARALLEL::WINDOW_LIST::HANNING, 6, 0.5f, melPostProcess);
    const auto expectedRgb = PDJE_PARALLEL::TO_RGB(melOut, kLegacyMelBins);

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

TEST_CASE("stft serial rgb output ignores normalize_min_max flag")
{
    auto pcm = BuildSignal(128);

    PDJE_PARALLEL::POST_PROCESS directRgbPostProcess;
    directRgbPostProcess.to_rgb = true;

    PDJE_PARALLEL::POST_PROCESS normalizedRgbPostProcess;
    normalizedRgbPostProcess.to_rgb            = true;
    normalizedRgbPostProcess.normalize_min_max = true;

    PDJE_TEST::util::SerialStftHarness stft;

    const auto [directRgb, directImag] = stft.calculate(
        pcm,
        PDJE_PARALLEL::WINDOW_LIST::HANNING,
        6,
        0.5f,
        directRgbPostProcess);
    const auto [normalizedRgb, normalizedImag] = stft.calculate(
        pcm,
        PDJE_PARALLEL::WINDOW_LIST::HANNING,
        6,
        0.5f,
        normalizedRgbPostProcess);

    REQUIRE(directImag.empty());
    REQUIRE(normalizedImag.empty());
    CheckVectorsClose(directRgb, normalizedRgb, 1.0e-6f);
}

TEST_CASE(
    "stft opencl backend matches serial postprocess outputs when available")
{
    if (PDJE_PARALLEL::STFT::detect_available_backend() !=
        PDJE_PARALLEL::BACKEND_T::OPENCL) {
        return;
    }

    PDJE_TEST::util::OpenclStftHarness openclProbe;
    if (!openclProbe.available()) {
        return;
    }

    CheckOpenclMatchesSerial(BuildSignal(5000), 12, 0.5f, {}, 1.2e-2f);

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

TEST_CASE("mel filter bank spec overload defaults to beat-this frontend geometry")
{
    const PDJE_PARALLEL::MelFilterBankSpec spec;
    const auto filterBank = PDJE_PARALLEL::GenMelFilterBank(spec);
    const std::size_t expectedSize =
        static_cast<std::size_t>(spec.n_mels) *
        static_cast<std::size_t>((spec.n_fft / 2) + 1);

    REQUIRE(filterBank.size() == expectedSize);
    CHECK(spec.sample_rate == 22050);
    CHECK(spec.n_fft == 1024);
    CHECK(spec.n_mels == 128);
    CHECK(spec.f_min == doctest::Approx(30.0f));
    CHECK(spec.f_max == doctest::Approx(11000.0f));
}
