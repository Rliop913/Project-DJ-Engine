
.. _program_listing_file_include_audioRender_MixMachine_MixTypes_Type_FX_FLANGER.cpp:

Program Listing for File Type_FX_FLANGER.cpp
============================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_audioRender_MixMachine_MixTypes_Type_FX_FLANGER.cpp>` (``include\audioRender\MixMachine\MixTypes\Type_FX_FLANGER.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "MixMachine.hpp"
   
   template <>
   bool
   MixMachine::TypeWorks<TypeEnum::FLANGER, FaustEffects>(MixStruct    &ms,
                                                          FaustEffects &data,
                                                          SIMD_FLOAT   *Vec)
   {
       if (!InterpolateInit(data.flangerData, Vec, ms)) {
           return false;
       }
   
       TRY(data.flangerData.back().bps =
               std::stof(ms.RP.getThird().cStr()) / 60.0;)
   
       return true;
   }
