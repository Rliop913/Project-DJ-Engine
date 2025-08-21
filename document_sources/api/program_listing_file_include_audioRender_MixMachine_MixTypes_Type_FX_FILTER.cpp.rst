
.. _program_listing_file_include_audioRender_MixMachine_MixTypes_Type_FX_FILTER.cpp:

Program Listing for File Type_FX_FILTER.cpp
===========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_audioRender_MixMachine_MixTypes_Type_FX_FILTER.cpp>` (``include\audioRender\MixMachine\MixTypes\Type_FX_FILTER.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "MixMachine.hpp"
   
   template<>
   bool
   MixMachine::TypeWorks<TypeEnum::FILTER, FaustEffects>
   (MixStruct& ms, FaustEffects& data, SIMD_FLOAT* Vec)
   {
       if(!InterpolateInit(data.filterData, Vec, ms)){
           return false;
       }
       
       switch (ms.RP.getDetails())
       {
       case DetailEnum::HIGH:
           data.filterData.back().HLswitch = 0;
           break;
       case DetailEnum::LOW:
           data.filterData.back().HLswitch = 1;
           break;
       default:
           break;
       }
       return true;
   }
