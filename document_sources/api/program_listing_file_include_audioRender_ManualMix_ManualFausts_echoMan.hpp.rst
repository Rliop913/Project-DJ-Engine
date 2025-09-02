
.. _program_listing_file_include_audioRender_ManualMix_ManualFausts_echoMan.hpp:

Program Listing for File echoMan.hpp
====================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_audioRender_ManualMix_ManualFausts_echoMan.hpp>` (``include/audioRender/ManualMix/ManualFausts/echoMan.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   #include "DeckData.hpp"
   class EchoMan {
     public:
       float EchoBps;
       float EchoFeedback;
       float EchoDryWet;
       ARGSETTER
       makeArgSetter()
       {
           return {
               { "EchoBps",
                 [this](double value) {
                     this->EchoBps = static_cast<float>(value);
                 } },
               { "EchoFeedback",
                 [this](double value) {
                     this->EchoFeedback = static_cast<float>(value);
                 } },
               { "EchoDryWet",
                 [this](double value) {
                     this->EchoDryWet = static_cast<float>(value);
                 } },
           };
       }
   };
