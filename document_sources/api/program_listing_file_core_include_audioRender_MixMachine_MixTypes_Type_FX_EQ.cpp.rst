
.. _program_listing_file_core_include_audioRender_MixMachine_MixTypes_Type_FX_EQ.cpp:

Program Listing for File Type_FX_EQ.cpp
=======================================

|exhale_lsh| :ref:`Return to documentation for file <file_core_include_audioRender_MixMachine_MixTypes_Type_FX_EQ.cpp>` (``core_include\audioRender\MixMachine\MixTypes\Type_FX_EQ.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "MixMachine.hpp"
   
   template <>
   bool
   MixMachine::TypeWorks<TypeEnum::EQ, FaustEffects>(MixStruct    &ms,
                                                     FaustEffects &data,
                                                     SIMD_FLOAT   *Vec)
   {
       if (!InterpolateInit(data.eqData, Vec, ms)) {
           return false;
       }
       switch (ms.RP.getDetails()) {
       case DetailEnum::HIGH:
           data.eqData.back().EQSelect = 0;
           break;
       case DetailEnum::MID:
           data.eqData.back().EQSelect = 1;
           break;
       case DetailEnum::LOW:
           data.eqData.back().EQSelect = 2;
           break;
       default:
           break;
       }
       return true;
   }
