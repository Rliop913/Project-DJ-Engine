
.. _program_listing_file_include_audioRender_ManualMix_ManualFausts_distortionMan.hpp:

Program Listing for File distortionMan.hpp
==========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_audioRender_ManualMix_ManualFausts_distortionMan.hpp>` (``include/audioRender/ManualMix/ManualFausts/distortionMan.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   #include "DeckData.hpp"
   
   class DistortionMan{
   public:
       float distortionValue;
       ARGSETTER
       makeArgSetter(){
           return{
               {"distortionValue", [this](double value){
                   this->distortionValue = static_cast<float>(value);
               }}
           };
       }
   };
