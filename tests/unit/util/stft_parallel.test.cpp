#include "STFT_Parallel.hpp"
#include "MelFilterBank.hpp"

#include <cmath>
#include <cstddef>
#include <limits>
#include <vector>

#include <doctest/doctest.h>

namespace {

constexpr float kPi                 = 3.14159265358979323846f;
constexpr float kReferenceTolerance = 1.0e-3f;

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

std::pair<std::vector<float>, std::vector<float>>
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

    return std::pair(std::move(realOut), std::move(imagOut));
}

std::vector<float>
ReferenceMagnitude(const std::vector<float> &realOut,
                   const std::vector<float> &imagOut)
{
    std::vector<float> magnitude(realOut.size(), 0.0f);

    for (std::size_t idx = 0; idx < realOut.size(); ++idx) {
        magnitude[idx] = std::sqrt(realOut[idx] * realOut[idx] +
                                   imagOut[idx] * imagOut[idx]);
    }

    return magnitude;
}

} // namespace

TEST_CASE("stft parallel calculate uses generated Serial path")
{
    std::vector<float>    pcm(128, 0.0f);
    constexpr std::size_t expectedOutputSize = 320;

    for (std::size_t idx = 0; idx < pcm.size(); ++idx) {
        pcm[idx] = std::sin(static_cast<float>(idx) * 0.1f);
    }

    PDJE_PARALLEL::STFT stft;
    stft.backend_now = PDJE_PARALLEL::BACKEND_T::SERIAL;
    auto [realOut, imagOut] =
        stft.calculate(pcm, PDJE_PARALLEL::WINDOW_LIST::HANNING, 6, 0.5f);
    auto [referenceReal, referenceImag] = ReferenceDftStft(pcm, 6, 0.5f);

    REQUIRE(realOut.size() == expectedOutputSize);
    REQUIRE(imagOut.size() == expectedOutputSize);
    REQUIRE(referenceReal.size() == expectedOutputSize);
    REQUIRE(referenceImag.size() == expectedOutputSize);

    for (std::size_t idx = 0; idx < realOut.size(); ++idx) {
        CHECK(std::isfinite(realOut[idx]));
        CHECK(std::isfinite(imagOut[idx]));
        CHECK(std::fabs(realOut[idx] - referenceReal[idx]) <
              kReferenceTolerance);
        CHECK(std::fabs(imagOut[idx] - referenceImag[idx]) <
              kReferenceTolerance);
    }
}

TEST_CASE("stft parallel calculate returns magnitude when POST_PROCESS::POWER is enabled")
{
    std::vector<float>    pcm(128, 0.0f);
    constexpr std::size_t expectedOutputSize = 320;

    for (std::size_t idx = 0; idx < pcm.size(); ++idx) {
        pcm[idx] = std::sin(static_cast<float>(idx) * 0.1f);
    }

    PDJE_PARALLEL::STFT stft;
    stft.backend_now = PDJE_PARALLEL::BACKEND_T::SERIAL;

    auto [magnitudeOut, imagOut] =
        stft.calculate(pcm,
                       PDJE_PARALLEL::WINDOW_LIST::HANNING,
                       6,
                       0.5f,
                       PDJE_PARALLEL::POST_PROCESS::POWER);
    auto [referenceReal, referenceImag] = ReferenceDftStft(pcm, 6, 0.5f);
    auto referenceMagnitude = ReferenceMagnitude(referenceReal, referenceImag);

    REQUIRE(magnitudeOut.size() == expectedOutputSize);
    REQUIRE(referenceMagnitude.size() == expectedOutputSize);
    CHECK(imagOut.empty());

    for (std::size_t idx = 0; idx < magnitudeOut.size(); ++idx) {
        CHECK(std::isfinite(magnitudeOut[idx]));
        CHECK(std::fabs(magnitudeOut[idx] - referenceMagnitude[idx]) <
              kReferenceTolerance);
    }
}

TEST_CASE("stft parallel calculate maps placeholder post process modes to NONE")
{
    std::vector<float> pcm(128, 0.0f);

    for (std::size_t idx = 0; idx < pcm.size(); ++idx) {
        pcm[idx] = std::sin(static_cast<float>(idx) * 0.1f);
    }

    PDJE_PARALLEL::STFT stft;
    stft.backend_now = PDJE_PARALLEL::BACKEND_T::SERIAL;

    auto [noneReal, noneImag] =
        stft.calculate(pcm,
                       PDJE_PARALLEL::WINDOW_LIST::HANNING,
                       6,
                       0.5f,
                       PDJE_PARALLEL::POST_PROCESS::NONE);
    auto [dbReal, dbImag] =
        stft.calculate(pcm,
                       PDJE_PARALLEL::WINDOW_LIST::HANNING,
                       6,
                       0.5f,
                       PDJE_PARALLEL::POST_PROCESS::TO_DB);
    auto [melReal, melImag] =
        stft.calculate(pcm,
                       PDJE_PARALLEL::WINDOW_LIST::HANNING,
                       6,
                       0.5f,
                       PDJE_PARALLEL::POST_PROCESS::MEL_SCALE);
    auto [chainReal, chainImag] =
        stft.calculate(pcm,
                       PDJE_PARALLEL::WINDOW_LIST::HANNING,
                       6,
                       0.5f,
                       PDJE_PARALLEL::POST_PROCESS::KERNEL_CHAIN__POWER_MEL_DB);

    CHECK(dbReal == noneReal);
    CHECK(dbImag == noneImag);
    CHECK(melReal == noneReal);
    CHECK(melImag == noneImag);
    CHECK(chainReal == noneReal);
    CHECK(chainImag == noneImag);
}

TEST_CASE("stft parallel calculate rejects unsupported window sizes")
{
    std::vector<float>  pcm(128, 1.0f);
    PDJE_PARALLEL::STFT stft;
    stft.backend_now = PDJE_PARALLEL::BACKEND_T::SERIAL;

    auto [realOut, imagOut] =
        stft.calculate(pcm, PDJE_PARALLEL::WINDOW_LIST::HANNING, 5, 0.5f);
    auto [powerOut, powerImagOut] =
        stft.calculate(pcm,
                       PDJE_PARALLEL::WINDOW_LIST::HANNING,
                       5,
                       0.5f,
                       PDJE_PARALLEL::POST_PROCESS::POWER);

    CHECK(realOut.empty());
    CHECK(imagOut.empty());
    CHECK(powerOut.empty());
    CHECK(powerImagOut.empty());
}

TEST_CASE("mel filter bank generates contiguous mel-major triangular rows")
{
    constexpr int sampleRate = 16000;
    constexpr int nFft = 16;
    constexpr int nMels = 4;
    constexpr int freqBins = (nFft / 2) + 1;

    const auto implicitNyquist =
        PDJE_PARALLEL::GenMelFilterBank(sampleRate, nFft, nMels);
    const auto explicitNyquist = PDJE_PARALLEL::GenMelFilterBank(
        sampleRate, nFft, nMels, 0.0f, static_cast<float>(sampleRate) * 0.5f);

    REQUIRE(implicitNyquist.size() ==
            static_cast<std::size_t>(nMels * freqBins));
    REQUIRE(implicitNyquist == explicitNyquist);

    for (int melIdx = 0; melIdx < nMels; ++melIdx) {
        bool sawPositive = false;
        bool sawDescent = false;
        int  firstPositiveBin = -1;
        int  lastPositiveBin = -1;
        float previousWeight = 0.0f;

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
                    sawPositive = true;
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
    CHECK(PDJE_PARALLEL::GenMelFilterBank(16000, 16, 4, -1.0f, 8000.0f)
              .empty());
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
    CHECK(PDJE_PARALLEL::GenMelFilterBank(
              16000,
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
    constexpr int sampleRate = 22050;
    constexpr int nFft = 512;
    constexpr int nMels = 16;
    constexpr int freqBins = (nFft / 2) + 1;
    constexpr float epsilon = 1.0e-5f;

    const auto htkNone = PDJE_PARALLEL::GenMelFilterBank(
        sampleRate,
        nFft,
        nMels,
        0.0f,
        -1.0f,
        PDJE_PARALLEL::MelFormula::HTK,
        PDJE_PARALLEL::MelNorm::None);
    const auto htkPeak = PDJE_PARALLEL::GenMelFilterBank(
        sampleRate,
        nFft,
        nMels,
        0.0f,
        -1.0f,
        PDJE_PARALLEL::MelFormula::HTK,
        PDJE_PARALLEL::MelNorm::Peak);
    const auto htkSlaneyNorm = PDJE_PARALLEL::GenMelFilterBank(
        sampleRate,
        nFft,
        nMels,
        0.0f,
        -1.0f,
        PDJE_PARALLEL::MelFormula::HTK,
        PDJE_PARALLEL::MelNorm::Slaney);
    const auto slaneyPeak = PDJE_PARALLEL::GenMelFilterBank(
        sampleRate,
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
    bool sawNormDifference = false;

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
