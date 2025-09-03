
.. _program_listing_file_include_audioRender_ManualMix_ManualFausts_ocsFilterMan.hpp:

Program Listing for File ocsFilterMan.hpp
=========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_audioRender_ManualMix_ManualFausts_ocsFilterMan.hpp>` (``include/audioRender/ManualMix/ManualFausts/ocsFilterMan.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   #include "DeckData.hpp"
   
   class OcsFilterMan {
     public:
       int   ocsFilterHighLowSW;
       int   middleFreq;
       int   rangeFreqHalf;
       float bps;
       float OCSFilterDryWet;
       ARGSETTER
       makeArgSetter()
       {
           return { { "OcsFilterHighLowSW",
                      [this](double value) {
                          this->ocsFilterHighLowSW = static_cast<int>(value);
                      } },
                    { "MiddleFreq",
                      [this](double value) {
                          this->middleFreq = static_cast<int>(value);
                      } },
                    { "RangeFreqHalf",
                      [this](double value) {
                          this->rangeFreqHalf = static_cast<int>(value);
                      } },
                    { "Bps",
                      [this](double value) {
                          this->bps = static_cast<float>(value);
                      } },
                    { "OCSFilterDryWet", [this](double value) {
                         this->OCSFilterDryWet = static_cast<float>(value);
                     } } };
       }
   };
