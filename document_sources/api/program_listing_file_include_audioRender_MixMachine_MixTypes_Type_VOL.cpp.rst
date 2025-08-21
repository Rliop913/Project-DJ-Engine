
.. _program_listing_file_include_audioRender_MixMachine_MixTypes_Type_VOL.cpp:

Program Listing for File Type_VOL.cpp
=====================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_audioRender_MixMachine_MixTypes_Type_VOL.cpp>` (``include/audioRender/MixMachine/MixTypes/Type_VOL.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "MixMachine.hpp"
   
   
   template<>
   bool
   MixMachine::TypeWorks<TypeEnum::VOL, FaustEffects>
   (MixStruct& ms, FaustEffects& data, SIMD_FLOAT* Vec)
   {
       if(!InterpolateInit(data.volData, Vec, ms)){
           return false;
       }
       
       return true;
   }
