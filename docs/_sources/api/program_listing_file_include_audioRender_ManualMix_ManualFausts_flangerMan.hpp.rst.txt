
.. _program_listing_file_include_audioRender_ManualMix_ManualFausts_flangerMan.hpp:

Program Listing for File flangerMan.hpp
=======================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_audioRender_ManualMix_ManualFausts_flangerMan.hpp>` (``include\audioRender\ManualMix\ManualFausts\flangerMan.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   #include "DeckData.hpp"
   
   class FlangerMan{
   public:
       float bps;
       float FlangerDryWet;
       ARGSETTER
       makeArgSetter(){
           return {
               {"Bps", [this](double value) {
                   this->bps = static_cast<float>(value);
               }},
               {"FlangerDryWet", [this](double value) {
                   this->FlangerDryWet = static_cast<float>(value);
               }}
           };
       }
   };
