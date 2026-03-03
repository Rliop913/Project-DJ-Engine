
.. _program_listing_file_include_util_function_scalar_Clamp.hpp:

Program Listing for File Clamp.hpp
==================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_util_function_scalar_Clamp.hpp>` (``include/util/function/scalar/Clamp.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include "util/common/Result.hpp"
   
   #include <algorithm>
   
   namespace PDJE_UTIL::function {
   
   struct ClampArgs {
       double value     = 0.0;
       double min_value = 0.0;
       double max_value = 1.0;
   };
   
   inline common::Result<double>
   clamp(ClampArgs args)
   {
       if (args.min_value > args.max_value) {
           return common::Result<double>::failure(
               { common::StatusCode::invalid_argument,
                 "ClampArgs.min_value must be less than or equal to ClampArgs.max_value." });
       }
   
       return common::Result<double>::success(
           std::clamp(args.value, args.min_value, args.max_value));
   }
   
   } // namespace PDJE_UTIL::function
