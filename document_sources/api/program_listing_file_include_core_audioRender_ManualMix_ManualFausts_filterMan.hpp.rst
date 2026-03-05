
.. _program_listing_file_include_core_audioRender_ManualMix_ManualFausts_filterMan.hpp:

Program Listing for File filterMan.hpp
======================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_core_audioRender_ManualMix_ManualFausts_filterMan.hpp>` (``include\core\audioRender\ManualMix\ManualFausts\filterMan.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   #include "DeckData.hpp"
   
   class FilterMan {
     public:
       int HLswitch;
       int Filterfreq;
       ARGSETTER
       makeArgSetter()
       {
           return { { "HLswitch",
                      [this](double value) {
                          this->HLswitch = static_cast<int>(value);
                      } },
                    { "Filterfreq", [this](double value) {
                         this->Filterfreq = static_cast<int>(value);
                     } } };
       }
   };
