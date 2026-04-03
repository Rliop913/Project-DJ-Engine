
.. _program_listing_file_include_util_function_stft_STFT_args.hpp:

Program Listing for File STFT_args.hpp
======================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_util_function_stft_STFT_args.hpp>` (``include\util\function\stft\STFT_args.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   namespace PDJE_PARALLEL {
   struct StftArgs {
       unsigned int FullSize;
       int          windowSize;
       int          qtConst;
       unsigned int OFullSize;
       unsigned int OHalfSize;
       unsigned int OMove;
   };
   } // namespace PDJE_PARALLEL
