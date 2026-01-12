
.. _program_listing_file_include_core_audioRender_MixMachine_MixTypes_Type_FX_PHASER.cpp:

Program Listing for File Type_FX_PHASER.cpp
===========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_core_audioRender_MixMachine_MixTypes_Type_FX_PHASER.cpp>` (``include\core\audioRender\MixMachine\MixTypes\Type_FX_PHASER.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "MixMachine.hpp"
   
   template <>
   bool
   MixMachine::TypeWorks<TypeEnum::PHASER, FaustEffects>(MixStruct    &ms,
                                                         FaustEffects &data,
                                                         SIMD_FLOAT   *Vec)
   {
       if (!InterpolateInit(data.phaserData, Vec, ms)) {
           return false;
       }
       TRY(data.phaserData.back().bps = std::stof(ms.RP.getThird().cStr()) / 60.0;)
       return true;
   }
