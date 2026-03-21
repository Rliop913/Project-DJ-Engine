
.. _program_listing_file_include_util_function_image_WaveformWebp.hpp:

Program Listing for File WaveformWebp.hpp
=========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_util_function_image_WaveformWebp.hpp>` (``include\util\function\image\WaveformWebp.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include "util/common/Result.hpp"
   #include "util/function/FunctionContext.hpp"
   
   #include <cstddef>
   #include <cstdint>
   #include <span>
   #include <vector>
   
   namespace PDJE_UTIL::function::image {
   
   using EncodedWebpBytes = std::vector<std::uint8_t>;
   using ChannelWaveformWebps = std::vector<EncodedWebpBytes>;
   using WaveformWebpBatch = std::vector<ChannelWaveformWebps>;
   
   struct EncodeWaveformWebpArgs {
       std::span<const float> pcm;
       std::size_t            channel_count = 0;
       std::size_t            y_pixels = 0;
       std::size_t            pcm_per_pixel = 0;
       std::size_t            x_pixels_per_image = 0;
       int                    compression_level = -1;
       std::size_t            worker_thread_count = 0;
   };
   
   common::Result<WaveformWebpBatch>
   encode_waveform_webps(const EncodeWaveformWebpArgs &args,
                         function::EvalOptions         options = {});
   
   } // namespace PDJE_UTIL::function::image
