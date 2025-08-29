
.. _program_listing_file_include_audioRender_ManualMix_ManualFausts_pannerMan.hpp:

Program Listing for File pannerMan.hpp
======================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_audioRender_ManualMix_ManualFausts_pannerMan.hpp>` (``include\audioRender\ManualMix\ManualFausts\pannerMan.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   #include "DeckData.hpp"
   
   class PannerMan {
     public:
       float bps;
       float PGain;
       float PannerDryWet;
       ARGSETTER
       makeArgSetter()
       {
           return { { "Bps",
                      [this](double value) {
                          this->bps = static_cast<float>(value);
                      } },
                    { "PGain",
                      [this](double value) {
                          this->PGain = static_cast<float>(value);
                      } },
                    { "PannerDryWet", [this](double value) {
                         this->PannerDryWet = static_cast<float>(value);
                     } } };
       }
   };
