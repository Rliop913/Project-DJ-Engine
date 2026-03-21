
.. _program_listing_file_include_util_function_halide_GainBias.hpp:

Program Listing for File GainBias.hpp
=====================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_util_function_halide_GainBias.hpp>` (``include/util/function/halide/GainBias.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include "util/common/Result.hpp"
   #include "util/function/FunctionContext.hpp"
   
   #include <cstddef>
   #include <cstdint>
   #include <vector>
   
   namespace PDJE_UTIL::function::halide {
   
   struct GrayImageView {
       const std::uint8_t *data   = nullptr;
       std::size_t         width  = 0;
       std::size_t         height = 0;
       std::size_t         stride = 0;
   };
   
   struct GrayImage {
       std::vector<std::uint8_t> pixels;
       std::size_t               width  = 0;
       std::size_t               height = 0;
       std::size_t               stride = 0;
   };
   
   struct GainBiasArgs {
       int  gain  = 1;
       int  bias  = 0;
       bool clamp = true;
   };
   
   inline common::Result<GrayImage>
   apply_gain_bias(GrayImageView input,
                   const GainBiasArgs &args,
                   function::EvalOptions options = {})
   {
       (void)options;
   
       if (input.data == nullptr) {
           return common::Result<GrayImage>::failure(
               { common::StatusCode::invalid_argument, "GrayImageView.data must not be null." });
       }
       if (input.width == 0 || input.height == 0) {
           return common::Result<GrayImage>::failure(
               { common::StatusCode::invalid_argument,
                 "GrayImageView width and height must be greater than zero." });
       }
       if (input.stride < input.width) {
           return common::Result<GrayImage>::failure(
               { common::StatusCode::invalid_argument,
                 "GrayImageView.stride must be greater than or equal to width." });
       }
   
       GrayImage output;
       output.width  = input.width;
       output.height = input.height;
       output.stride = input.width;
       output.pixels.resize(output.width * output.height);
   
       for (std::size_t y = 0; y < input.height; ++y) {
           for (std::size_t x = 0; x < input.width; ++x) {
               const auto src_index = y * input.stride + x;
               const auto dst_index = y * output.stride + x;
               int value = static_cast<int>(input.data[src_index]) * args.gain + args.bias;
               if (args.clamp) {
                   if (value < 0) {
                       value = 0;
                   } else if (value > 255) {
                       value = 255;
                   }
               }
               output.pixels[dst_index] = static_cast<std::uint8_t>(value);
           }
       }
   
       return common::Result<GrayImage>::success(std::move(output));
   }
   
   } // namespace PDJE_UTIL::function::halide
