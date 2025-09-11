
.. _program_listing_file_core_include_audioRender_MixMachine_MixTypes_Type_FX_TRANCE.cpp:

Program Listing for File Type_FX_TRANCE.cpp
===========================================

|exhale_lsh| :ref:`Return to documentation for file <file_core_include_audioRender_MixMachine_MixTypes_Type_FX_TRANCE.cpp>` (``core_include\audioRender\MixMachine\MixTypes\Type_FX_TRANCE.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "MixMachine.hpp"
   
   template <>
   bool
   MixMachine::TypeWorks<TypeEnum::TRANCE, FaustEffects>(MixStruct    &ms,
                                                         FaustEffects &data,
                                                         SIMD_FLOAT   *Vec)
   {
       if (!InterpolateInit(data.tranceData, Vec, ms)) {
           return false;
       }
   
       EightPointValues thirds(ms.RP.getThird().cStr());
       data.tranceData.back().bps  = thirds.vals[0] / 60.0;
       data.tranceData.back().gain = thirds.vals[1];
   
       return true;
   }
