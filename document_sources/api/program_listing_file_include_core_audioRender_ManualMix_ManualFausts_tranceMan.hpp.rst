
.. _program_listing_file_include_core_audioRender_ManualMix_ManualFausts_tranceMan.hpp:

Program Listing for File tranceMan.hpp
======================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_core_audioRender_ManualMix_ManualFausts_tranceMan.hpp>` (``include/core/audioRender/ManualMix/ManualFausts/tranceMan.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   #include "DeckData.hpp"
   
   class TranceMan {
     public:
       float bps;
       float gain;
       float TranceDryWet;
       ARGSETTER
       makeArgSetter()
       {
           return { { "Bps",
                      [this](double value) {
                          this->bps = static_cast<float>(value);
                      } },
                    { "Gain",
                      [this](double value) {
                          this->gain = static_cast<float>(value);
                      } },
                    { "TranceDryWet", [this](double value) {
                         this->TranceDryWet = static_cast<float>(value);
                     } } };
       }
   };
