
.. _program_listing_file_include_audioRender_MixMachine_MixTypes_Type_FX_ECHO.cpp:

Program Listing for File Type_FX_ECHO.cpp
=========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_audioRender_MixMachine_MixTypes_Type_FX_ECHO.cpp>` (``include/audioRender/MixMachine/MixTypes/Type_FX_ECHO.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "MixMachine.hpp"
   
   template<>
   bool
   MixMachine::TypeWorks<TypeEnum::ECHO, FaustEffects>
   (MixStruct& ms, FaustEffects& data, SIMD_FLOAT* Vec)
   {
       if(!InterpolateInit(data.echoData, Vec, ms)){
           return false;
       }
   
       EightPointValues thirds(ms.RP.getThird().cStr());
       data.echoData.back().EchoBps = thirds.vals[0] / 60.0;
       data.echoData.back().EchoFeedback = thirds.vals[1];
       
       return true;
   }
