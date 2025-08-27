
.. _program_listing_file_include_audioRender_MixMachine_MixTypes_Type_FX_OCS_FILTER.cpp:

Program Listing for File Type_FX_OCS_FILTER.cpp
===============================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_audioRender_MixMachine_MixTypes_Type_FX_OCS_FILTER.cpp>` (``include\audioRender\MixMachine\MixTypes\Type_FX_OCS_FILTER.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "MixMachine.hpp"
   
   template<>
   bool
   MixMachine::TypeWorks<TypeEnum::OSC_FILTER, FaustEffects>
   (MixStruct& ms, FaustEffects& data, SIMD_FLOAT* Vec)
   {
       if(!InterpolateInit(data.ocsFilterData, Vec, ms)){
           return false;
       }
       
       switch (ms.RP.getDetails())
       {
       case DetailEnum::HIGH:
           data.ocsFilterData.back().ocsFilterHighLowSW = 0;
           break;
       
       case DetailEnum::LOW:
           data.ocsFilterData.back().ocsFilterHighLowSW = 1;
           break;
       default:
           break;
       }
   
       EightPointValues thirds(ms.RP.getThird().cStr());
       data.ocsFilterData.back().bps = thirds.vals[0] / 60.0;
       data.ocsFilterData.back().middleFreq = thirds.vals[1];
       data.ocsFilterData.back().rangeFreqHalf = thirds.vals[2];
   
       return true;
   }
