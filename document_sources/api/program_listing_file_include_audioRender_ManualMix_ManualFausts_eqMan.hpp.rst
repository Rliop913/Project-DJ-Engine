
.. _program_listing_file_include_audioRender_ManualMix_ManualFausts_eqMan.hpp:

Program Listing for File eqMan.hpp
==================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_audioRender_ManualMix_ManualFausts_eqMan.hpp>` (``include\audioRender\ManualMix\ManualFausts\eqMan.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   #include "DeckData.hpp"
   
   class EQMan{
   public:
       int EQSelect;
       int EQPower;
       ARGSETTER
       makeArgSetter(){
           return {
               {"EQSelect", [this](double value) {
                   this->EQSelect = static_cast<int>(value);
               }},
               {"EQPower", [this](double value) {
                   this->EQPower = static_cast<int>(value);
               }}
           };
       }
   };
