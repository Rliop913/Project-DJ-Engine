
.. _program_listing_file_include_audioRender_MixMachine_MixTypes_Type_FX_PANNER.cpp:

Program Listing for File Type_FX_PANNER.cpp
===========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_audioRender_MixMachine_MixTypes_Type_FX_PANNER.cpp>` (``include/audioRender/MixMachine/MixTypes/Type_FX_PANNER.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "MixMachine.hpp"
   
   template <>
   bool
   MixMachine::TypeWorks<TypeEnum::PANNER, FaustEffects>(MixStruct    &ms,
                                                         FaustEffects &data,
                                                         SIMD_FLOAT   *Vec)
   {
       if (!InterpolateInit(data.pannerData, Vec, ms)) {
           return false;
       }
   
       EightPointValues thirds(ms.RP.getThird().cStr());
       data.pannerData.back().bps   = thirds.vals[0] / 60.0;
       data.pannerData.back().PGain = thirds.vals[1];
   
       return true;
   }
