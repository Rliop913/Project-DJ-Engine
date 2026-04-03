
.. _program_listing_file_include_util_function_stft_BackendLess.hpp:

Program Listing for File BackendLess.hpp
========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_util_function_stft_BackendLess.hpp>` (``include\util\function\stft\BackendLess.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include <algorithm>
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
   
   static inline float
   MeanBandValue(const std::vector<float> &vec,
                 const std::size_t         begin,
                 const std::size_t         end)
   {
       if (begin >= end || begin >= vec.size()) {
           return 0.0f;
       }
   
       float       sum   = 0.0f;
       std::size_t count = 0;
   
       for (std::size_t idx = begin; idx < end; ++idx) {
           const float value = vec[idx];
           if (!std::isfinite(value)) {
               continue;
           }
   
           sum += ClampUnitFloat(value);
           ++count;
       }
   
       if (count == 0) {
           return 0.0f;
       }
   
       return ClampUnitFloat(sum / static_cast<float>(count));
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
   
       const std::size_t lowEnd = melSize / 3u;
       const std::size_t midEnd = (melSize * 2u) / 3u;
   
       std::vector<float> rgb(frameCount * 3u, 0.0f);
   
       for (std::size_t frameIdx = 0; frameIdx < frameCount; ++frameIdx) {
           const std::size_t frameBase = frameIdx * melSize;
           const std::size_t rgbBase   = frameIdx * 3u;
   
           rgb[rgbBase + 0u] = detail::MeanBandValue(
               vec, frameBase, frameBase + lowEnd);
           rgb[rgbBase + 1u] = detail::MeanBandValue(
               vec, frameBase + lowEnd, frameBase + midEnd);
           rgb[rgbBase + 2u] = detail::MeanBandValue(
               vec, frameBase + midEnd, frameBase + melSize);
       }
   
       return rgb;
   }
   
   } // namespace PDJE_PARALLEL
