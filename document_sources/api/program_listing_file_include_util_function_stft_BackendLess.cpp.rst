
.. _program_listing_file_include_util_function_stft_BackendLess.cpp:

Program Listing for File BackendLess.cpp
========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_util_function_stft_BackendLess.cpp>` (``include\util\function\stft\BackendLess.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "Parallel_Args.hpp"
   #include "STFT_Parallel.hpp"
   namespace PDJE_PARALLEL {
   
   StftArgs
   STFT::GenArgs(const std::vector<float> &inputVec,
                 const int                 windowSizeEXP,
                 const float               overlapRatio)
   {
       StftArgs arglist;
       arglist.FullSize   = static_cast<unsigned int>(inputVec.size());
       arglist.windowSize = 1 << windowSizeEXP;
       arglist.qtConst =
           toQuot(arglist.FullSize, overlapRatio, arglist.windowSize);
       arglist.OFullSize = arglist.qtConst * arglist.windowSize;
       arglist.OHalfSize = arglist.OFullSize / 2;
       arglist.OMove =
           static_cast<unsigned int>(arglist.windowSize * (1.0f - overlapRatio));
       return arglist;
   }
   
   
   
   } // namespace PDJE_PARALLEL
