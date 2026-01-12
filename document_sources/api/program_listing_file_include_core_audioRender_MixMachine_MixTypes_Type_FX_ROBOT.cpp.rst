
.. _program_listing_file_include_core_audioRender_MixMachine_MixTypes_Type_FX_ROBOT.cpp:

Program Listing for File Type_FX_ROBOT.cpp
==========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_core_audioRender_MixMachine_MixTypes_Type_FX_ROBOT.cpp>` (``include\core\audioRender\MixMachine\MixTypes\Type_FX_ROBOT.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "MixMachine.hpp"
   
   template <>
   bool
   MixMachine::TypeWorks<TypeEnum::ROBOT, FaustEffects>(MixStruct    &ms,
                                                        FaustEffects &data,
                                                        SIMD_FLOAT   *Vec)
   {
       if (!InterpolateInit(data.robotData, Vec, ms)) {
           return false;
       }
       TRY(data.robotData.back().robotFreq = std::stof(ms.RP.getThird().cStr());)
   
       return true;
   }
