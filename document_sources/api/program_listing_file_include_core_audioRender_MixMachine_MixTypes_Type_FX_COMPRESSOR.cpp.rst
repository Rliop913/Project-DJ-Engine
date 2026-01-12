
.. _program_listing_file_include_core_audioRender_MixMachine_MixTypes_Type_FX_COMPRESSOR.cpp:

Program Listing for File Type_FX_COMPRESSOR.cpp
===============================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_core_audioRender_MixMachine_MixTypes_Type_FX_COMPRESSOR.cpp>` (``include\core\audioRender\MixMachine\MixTypes\Type_FX_COMPRESSOR.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "MixMachine.hpp"
   
   template <>
   bool
   MixMachine::TypeWorks<TypeEnum::COMPRESSOR, FaustEffects>(MixStruct    &ms,
                                                             FaustEffects &data,
                                                             SIMD_FLOAT   *Vec)
   {
       data.compressorData.emplace_back(Vec, ms.frame_in, ms.frame_out);
       TRY(data.compressorData.back().strength =
               std::stof(ms.RP.getFirst().cStr());)
   
       EightPointValues tk(ms.RP.getSecond().cStr());
       EightPointValues ar(ms.RP.getThird().cStr());
       data.compressorData.back().threshDB  = tk.vals[0];
       data.compressorData.back().kneeDB    = tk.vals[1];
       data.compressorData.back().attackMS  = ar.vals[0];
       data.compressorData.back().releaseMS = ar.vals[1];
       return true;
   }
