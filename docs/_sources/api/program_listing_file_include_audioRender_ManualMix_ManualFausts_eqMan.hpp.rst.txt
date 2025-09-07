
.. _program_listing_file_include_audioRender_ManualMix_ManualFausts_eqMan.hpp:

Program Listing for File eqMan.hpp
==================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_audioRender_ManualMix_ManualFausts_eqMan.hpp>` (``include/audioRender/ManualMix/ManualFausts/eqMan.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   #include "DeckData.hpp"
   
   class EQMan {
     public:
       int EQLow  = 0;
       int EQMid  = 0;
       int EQHigh = 0;
       ARGSETTER
       makeArgSetter()
       {
           return {
               { "EQLow",
                 [this](double value) { this->EQLow = static_cast<int>(value); } },
               { "EQMid",
                 [this](double value) { this->EQMid = static_cast<int>(value); } },
               { "EQHigh",
                 [this](double value) { this->EQHigh = static_cast<int>(value); } }
           };
       }
   };
