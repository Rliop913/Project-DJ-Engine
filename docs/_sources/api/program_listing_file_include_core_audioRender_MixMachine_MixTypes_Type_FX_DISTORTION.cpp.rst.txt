
.. _program_listing_file_include_core_audioRender_MixMachine_MixTypes_Type_FX_DISTORTION.cpp:

Program Listing for File Type_FX_DISTORTION.cpp
===============================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_core_audioRender_MixMachine_MixTypes_Type_FX_DISTORTION.cpp>` (``include\core\audioRender\MixMachine\MixTypes\Type_FX_DISTORTION.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "MixMachine.hpp"
   
   template <>
   bool
   MixMachine::TypeWorks<TypeEnum::DISTORTION, FaustEffects>(MixStruct    &ms,
                                                             FaustEffects &data,
                                                             SIMD_FLOAT   *Vec)
   {
       if (!InterpolateInit(data.distortionData, Vec, ms)) {
           return false;
       }
       return true;
   }
