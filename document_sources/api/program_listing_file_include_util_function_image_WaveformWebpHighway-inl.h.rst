
.. _program_listing_file_include_util_function_image_WaveformWebpHighway-inl.h:

Program Listing for File WaveformWebpHighway-inl.h
==================================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_util_function_image_WaveformWebpHighway-inl.h>` (``include/util/function/image/WaveformWebpHighway-inl.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include <algorithm>
   #include <cstddef>
   
   #undef HWY_TARGET_INCLUDE
   #define HWY_TARGET_INCLUDE "WaveformWebpHighway-inl.h"
   #include "hwy/foreach_target.h"
   
   #include <hwy/highway.h>
   
   namespace HWY_NAMESPACE {
   
   HWY_ATTR
   void
   ComputeWaveformColumnExtremaSIMD(const float *samples,
                                    std::size_t  pcm_per_pixel,
                                    std::size_t  x_pixels,
                                    float       *mins_out,
                                    float       *maxs_out)
   {
       const hwy::HWY_NAMESPACE::ScalableTag<float> tag;
       const std::size_t                            lane_count =
           hwy::HWY_NAMESPACE::Lanes(tag);
       const auto neg_one = hwy::HWY_NAMESPACE::Set(tag, -1.0f);
       const auto pos_one = hwy::HWY_NAMESPACE::Set(tag, 1.0f);
   
       for (std::size_t x = 0; x < x_pixels; ++x) {
           const float *column = samples + (x * pcm_per_pixel);
           auto         min_v  = pos_one;
           auto         max_v  = neg_one;
           std::size_t  i      = 0;
   
           for (; i + lane_count <= pcm_per_pixel; i += lane_count) {
               auto values = hwy::HWY_NAMESPACE::LoadU(tag, column + i);
               values      = hwy::HWY_NAMESPACE::Min(
                   hwy::HWY_NAMESPACE::Max(values, neg_one),
                   pos_one);
               min_v = hwy::HWY_NAMESPACE::Min(min_v, values);
               max_v = hwy::HWY_NAMESPACE::Max(max_v, values);
           }
   
           float min_sample = hwy::HWY_NAMESPACE::ReduceMin(tag, min_v);
           float max_sample = hwy::HWY_NAMESPACE::ReduceMax(tag, max_v);
   
           for (; i < pcm_per_pixel; ++i) {
               const float sample = std::clamp(column[i], -1.0f, 1.0f);
               min_sample         = std::min(min_sample, sample);
               max_sample         = std::max(max_sample, sample);
           }
   
           mins_out[x] = min_sample;
           maxs_out[x] = max_sample;
       }
   }
   
   } // namespace HWY_NAMESPACE
