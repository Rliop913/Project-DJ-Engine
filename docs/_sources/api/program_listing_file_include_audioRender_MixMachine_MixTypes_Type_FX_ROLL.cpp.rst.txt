
.. _program_listing_file_include_audioRender_MixMachine_MixTypes_Type_FX_ROLL.cpp:

Program Listing for File Type_FX_ROLL.cpp
=========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_audioRender_MixMachine_MixTypes_Type_FX_ROLL.cpp>` (``include/audioRender/MixMachine/MixTypes/Type_FX_ROLL.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "MixMachine.hpp"
   
   template <>
   bool
   MixMachine::TypeWorks<TypeEnum::ROLL, FaustEffects>(MixStruct    &ms,
                                                       FaustEffects &data,
                                                       SIMD_FLOAT   *Vec)
   {
       if (!InterpolateInit(data.rollData, Vec, ms)) {
           return false;
       }
   
       TRY(data.rollData.back().RollBpm = std::stof(ms.RP.getThird().cStr());)
   
       return true;
   }
