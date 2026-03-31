#include "STFT_Parallel.hpp"

#include <cmath>
#include <cstddef>
#include <vector>

#include <doctest/doctest.h>

namespace {

constexpr float kPi = 3.14159265358979323846f;
constexpr float kReferenceTolerance = 1.0e-3f;

unsigned int
ReferenceToQuot(const std::size_t fullSize,
                const float       overlapRatio,
                const int         windowSize)
{
    if (overlapRatio == 0.0f) {
        return static_cast<unsigned int>(fullSize / windowSize) + 1;
    }

    return static_cast<unsigned int>(
               fullSize / (windowSize * (1.0f - overlapRatio))) +
           1;
}

std::vector<float>
BuildReferenceOverlap(const std::vector<float> &pcm,
                      const int                 windowSizeExp,
                      const float               overlapRatio)
{
    const unsigned int windowSize = 1u << windowSizeExp;
    const unsigned int outputSize =
        ReferenceToQuot(pcm.size(), overlapRatio, static_cast<int>(windowSize)) *
        windowSize;
    const unsigned int moveSize =
        static_cast<unsigned int>(windowSize * (1.0f - overlapRatio));

    std::vector<float> overlapped(outputSize, 0.0f);

    for (unsigned int idx = 0; idx < outputSize; ++idx) {
        const unsigned int windowIdx = idx >> windowSizeExp;
        const unsigned int localIdx = idx & (windowSize - 1u);
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
    const float angle =
        2.0f * kPi * static_cast<float>(index) /
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
                const float angle = -2.0f * kPi * static_cast<float>(k * n) /
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
        magnitude[idx] =
            std::sqrt(realOut[idx] * realOut[idx] + imagOut[idx] * imagOut[idx]);
    }

    return magnitude;
}

} // namespace

TEST_CASE("stft parallel calculate uses generated OpenMP path")
{
    std::vector<float> pcm(128, 0.0f);
    constexpr std::size_t expectedOutputSize = 320;

    for (std::size_t idx = 0; idx < pcm.size(); ++idx) {
        pcm[idx] = std::sin(static_cast<float>(idx) * 0.1f);
    }

    PDJE_PARALLEL::STFT stft;
    stft.backend_now = PDJE_PARALLEL::BACKEND_T::OPENMP;
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

TEST_CASE("stft parallel calculate returns magnitude when toPower is enabled")
{
    std::vector<float> pcm(128, 0.0f);
    constexpr std::size_t expectedOutputSize = 320;

    for (std::size_t idx = 0; idx < pcm.size(); ++idx) {
        pcm[idx] = std::sin(static_cast<float>(idx) * 0.1f);
    }

    PDJE_PARALLEL::STFT stft;
    stft.backend_now = PDJE_PARALLEL::BACKEND_T::OPENMP;

    auto [magnitudeOut, imagOut] =
        stft.calculate(pcm, PDJE_PARALLEL::WINDOW_LIST::HANNING, 6, 0.5f, true);
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

TEST_CASE("stft parallel calculate rejects unsupported window sizes")
{
    std::vector<float> pcm(128, 1.0f);
    PDJE_PARALLEL::STFT stft;
    stft.backend_now = PDJE_PARALLEL::BACKEND_T::OPENMP;

    auto [realOut, imagOut] =
        stft.calculate(pcm, PDJE_PARALLEL::WINDOW_LIST::HANNING, 5, 0.5f);
    auto [powerOut, powerImagOut] =
        stft.calculate(pcm, PDJE_PARALLEL::WINDOW_LIST::HANNING, 5, 0.5f, true);

    CHECK(realOut.empty());
    CHECK(imagOut.empty());
    CHECK(powerOut.empty());
    CHECK(powerImagOut.empty());
}
