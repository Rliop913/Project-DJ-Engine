#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <vector>

namespace PDJE_PARALLEL
{

namespace detail {

static inline float
ClampUnitFloat(const float value)
{
    if (!std::isfinite(value)) {
        return 0.0f;
    }

    return std::clamp(value, 0.0f, 1.0f);
}

struct RgbBandBoundaries {
    std::size_t low_begin  = 0;
    std::size_t low_end    = 0;
    std::size_t mid_begin  = 0;
    std::size_t mid_end    = 0;
    std::size_t high_begin = 0;
    std::size_t high_end   = 0;
};

static inline float
SanitizeShiftedValue(const float value, const float shift) noexcept
{
    if (!std::isfinite(value)) {
        return 0.0f;
    }

    return std::max(value + shift, 0.0f);
}

static inline RgbBandBoundaries
ComputeRgbBandBoundaries(const std::size_t melSize) noexcept
{
    if (melSize < 3u) {
        return {};
    }

    std::array<std::size_t, 3> counts{
        (melSize * 180u) / 1000u,
        (melSize * 350u) / 1000u,
        0u,
    };
    counts[2] = melSize - counts[0] - counts[1];

    for (std::size_t idx = 0; idx < counts.size(); ++idx) {
        if (counts[idx] != 0u) {
            continue;
        }

        std::size_t donor = idx;
        for (std::size_t candidate = 0; candidate < counts.size(); ++candidate) {
            if (counts[candidate] > counts[donor]) {
                donor = candidate;
            }
        }

        if (counts[donor] > 1u) {
            --counts[donor];
            counts[idx] = 1u;
        }
    }

    return {
        .low_begin = 0u,
        .low_end = counts[0],
        .mid_begin = counts[0],
        .mid_end = counts[0] + counts[1],
        .high_begin = counts[0] + counts[1],
        .high_end = melSize,
    };
}

static inline float
SanitizeFrameRange(const std::vector<float> &vec,
                   const std::size_t         begin,
                   const std::size_t         end) noexcept
{
    float minValue = std::numeric_limits<float>::infinity();
    bool  hasFiniteValue = false;

    const std::size_t clampedEnd = std::min(end, vec.size());
    for (std::size_t idx = begin; idx < clampedEnd; ++idx) {
        const float value = vec[idx];
        if (!std::isfinite(value)) {
            continue;
        }

        minValue       = std::min(minValue, value);
        hasFiniteValue = true;
    }

    if (!hasFiniteValue || minValue >= 0.0f) {
        return 0.0f;
    }

    return -minValue;
}

static inline float
BandRms(const std::vector<float> &vec,
        const std::size_t         begin,
        const std::size_t         end,
        const float               shift) noexcept
{
    if (begin >= end || begin >= vec.size()) {
        return 0.0f;
    }

    const std::size_t clampedEnd = std::min(end, vec.size());
    const std::size_t count      = clampedEnd - begin;
    if (count == 0u) {
        return 0.0f;
    }

    double sumSquares = 0.0;
    for (std::size_t idx = begin; idx < clampedEnd; ++idx) {
        const double value = static_cast<double>(
            SanitizeShiftedValue(vec[idx], shift));
        sumSquares += value * value;
    }

    return static_cast<float>(
        std::sqrt(sumSquares / static_cast<double>(count)));
}

static inline float
FrameMean(const std::vector<float> &vec,
          const std::size_t         begin,
          const std::size_t         end,
          const float               shift) noexcept
{
    if (begin >= end || begin >= vec.size()) {
        return 0.0f;
    }

    const std::size_t clampedEnd = std::min(end, vec.size());
    const std::size_t count      = clampedEnd - begin;
    if (count == 0u) {
        return 0.0f;
    }

    double sum = 0.0;
    for (std::size_t idx = begin; idx < clampedEnd; ++idx) {
        sum += static_cast<double>(SanitizeShiftedValue(vec[idx], shift));
    }

    return static_cast<float>(sum / static_cast<double>(count));
}

static inline float
Percentile(std::vector<float> values, const float fraction)
{
    if (values.empty()) {
        return 0.0f;
    }

    const float clampedFraction = std::clamp(fraction, 0.0f, 1.0f);
    const auto percentileIndex = static_cast<std::size_t>(
        std::ceil(static_cast<double>(values.size()) *
                  static_cast<double>(clampedFraction))) -
                                 1u;
    std::nth_element(values.begin(),
                     values.begin() +
                         static_cast<std::ptrdiff_t>(percentileIndex),
                     values.end());
    return values[percentileIndex];
}

} // namespace detail

static inline void
Normalize_minmax(std::vector<float> &vec, const uint32_t chunkSZ)
{
    if (vec.empty() || chunkSZ == 0) {
        return;
    }

    const std::size_t chunkSize = static_cast<std::size_t>(chunkSZ);

    for (std::size_t chunkBegin = 0; chunkBegin < vec.size();
         chunkBegin += chunkSize) {
        const std::size_t chunkEnd = std::min(vec.size(), chunkBegin + chunkSize);

        float minValue = std::numeric_limits<float>::infinity();
        float maxValue = -std::numeric_limits<float>::infinity();
        bool  hasFiniteValue = false;

        for (std::size_t idx = chunkBegin; idx < chunkEnd; ++idx) {
            const float value = vec[idx];
            if (!std::isfinite(value)) {
                continue;
            }

            minValue = std::min(minValue, value);
            maxValue = std::max(maxValue, value);
            hasFiniteValue = true;
        }

        if (!hasFiniteValue || maxValue <= minValue) {
            std::fill(vec.begin() + static_cast<std::ptrdiff_t>(chunkBegin),
                      vec.begin() + static_cast<std::ptrdiff_t>(chunkEnd),
                      0.0f);
            continue;
        }

        const float invRange = 1.0f / (maxValue - minValue);

        for (std::size_t idx = chunkBegin; idx < chunkEnd; ++idx) {
            const float value = vec[idx];
            if (!std::isfinite(value)) {
                vec[idx] = 0.0f;
                continue;
            }

            vec[idx] =
                detail::ClampUnitFloat((value - minValue) * invRange);
        }
    }
}

static inline std::vector<float>
TO_RGB(const std::vector<float> &vec, const uint32_t melSZ)
{
    if (vec.empty() || melSZ < 3) {
        return {};
    }

    const std::size_t melSize = static_cast<std::size_t>(melSZ);
    if ((vec.size() % melSize) != 0) {
        return {};
    }

    const std::size_t frameCount = vec.size() / melSize;
    if (frameCount > (std::numeric_limits<std::size_t>::max() / 3u)) {
        return {};
    }

    std::vector<float> rgb(frameCount * 3u, 0.0f);
    const auto         boundaries = detail::ComputeRgbBandBoundaries(melSize);
    constexpr float    kEpsilon   = 1.0e-6f;
    constexpr float    kRedGain   = 1.00f;
    constexpr float    kGreenGain = 1.18f;
    constexpr float    kBlueGain  = 1.55f;
    constexpr float    kPastelMin = 0.18f;
    constexpr float    kRedChromaGamma   = 1.08f;
    constexpr float    kGreenChromaGamma = 0.92f;
    constexpr float    kBlueChromaGamma  = 0.84f;

    std::vector<float> rawBrightness(frameCount, 0.0f);
    std::vector<float> positiveBrightness;
    positiveBrightness.reserve(frameCount);

    for (std::size_t frameIdx = 0; frameIdx < frameCount; ++frameIdx) {
        const std::size_t frameBase = frameIdx * melSize;
        const float shift =
            detail::SanitizeFrameRange(vec, frameBase, frameBase + melSize);
        const float brightness =
            detail::FrameMean(vec, frameBase, frameBase + melSize, shift);
        rawBrightness[frameIdx] = brightness;

        if (brightness > kEpsilon) {
            positiveBrightness.push_back(brightness);
        }
    }

    if (positiveBrightness.empty()) {
        return rgb;
    }

    const float brightnessP15 =
        detail::Percentile(positiveBrightness, 0.15f);
    const float brightnessP85 =
        detail::Percentile(std::move(positiveBrightness), 0.85f);
    const bool  hasBrightnessRange =
        brightnessP85 > (brightnessP15 + kEpsilon);
    const float brightnessRange =
        std::max(brightnessP85 - brightnessP15, kEpsilon);
    const float silenceThreshold =
        std::max(kEpsilon, brightnessP15 * 0.25f);

    for (std::size_t frameIdx = 0; frameIdx < frameCount; ++frameIdx) {
        const std::size_t frameBase = frameIdx * melSize;
        const std::size_t rgbBase   = frameIdx * 3u;
        const float shift =
            detail::SanitizeFrameRange(vec, frameBase, frameBase + melSize);

        const float lowRaw = detail::BandRms(
            vec,
            frameBase + boundaries.low_begin,
            frameBase + boundaries.low_end,
            shift);
        const float midRaw = detail::BandRms(
            vec,
            frameBase + boundaries.mid_begin,
            frameBase + boundaries.mid_end,
            shift);
        const float highRaw = detail::BandRms(
            vec,
            frameBase + boundaries.high_begin,
            frameBase + boundaries.high_end,
            shift);

        const float red   = std::log1p(std::max(lowRaw, 0.0f)) * kRedGain;
        const float green = std::log1p(std::max(midRaw, 0.0f)) * kGreenGain;
        const float blue  = std::log1p(std::max(highRaw, 0.0f)) * kBlueGain;
        const float sum   = red + green + blue;
        if (sum <= kEpsilon) {
            continue;
        }

        std::array<float, 3> chroma{
            red / sum,
            green / sum,
            blue / sum,
        };
        chroma[0] =
            kPastelMin +
            ((1.0f - kPastelMin) *
             std::pow(detail::ClampUnitFloat(chroma[0]), kRedChromaGamma));
        chroma[1] =
            kPastelMin +
            ((1.0f - kPastelMin) *
             std::pow(detail::ClampUnitFloat(chroma[1]), kGreenChromaGamma));
        chroma[2] =
            kPastelMin +
            ((1.0f - kPastelMin) *
             std::pow(detail::ClampUnitFloat(chroma[2]), kBlueChromaGamma));

        if (rawBrightness[frameIdx] <= silenceThreshold) {
            continue;
        }

        const float brightnessNorm =
            hasBrightnessRange
                ? detail::ClampUnitFloat(
                      (rawBrightness[frameIdx] - brightnessP15) / brightnessRange)
                : 1.0f;
        const float brightness =
            0.58f + (0.42f * std::pow(brightnessNorm, 0.80f));

        rgb[rgbBase + 0u] =
            detail::ClampUnitFloat(chroma[0] * brightness);
        rgb[rgbBase + 1u] =
            detail::ClampUnitFloat(chroma[1] * brightness);
        rgb[rgbBase + 2u] =
            detail::ClampUnitFloat(chroma[2] * brightness);
    }

    return rgb;
}

} // namespace PDJE_PARALLEL
