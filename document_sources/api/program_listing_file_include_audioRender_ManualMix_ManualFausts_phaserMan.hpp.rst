
.. _program_listing_file_include_audioRender_ManualMix_ManualFausts_phaserMan.hpp:

Program Listing for File phaserMan.hpp
======================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_audioRender_ManualMix_ManualFausts_phaserMan.hpp>` (``include/audioRender/ManualMix/ManualFausts/phaserMan.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   #include "DeckData.hpp"
   
   class PhaserMan {
     public:
       float bps;
       float PhaserDryWet;
       ARGSETTER
       makeArgSetter()
       {
           return { { "Bps",
                      [this](double value) {
                          this->bps = static_cast<float>(value);
                      } },
                    { "PhaserDryWet", [this](double value) {
                         this->PhaserDryWet = static_cast<float>(value);
                     } } };
       }
   };
