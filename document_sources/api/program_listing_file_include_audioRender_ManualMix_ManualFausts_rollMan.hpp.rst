
.. _program_listing_file_include_audioRender_ManualMix_ManualFausts_rollMan.hpp:

Program Listing for File rollMan.hpp
====================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_audioRender_ManualMix_ManualFausts_rollMan.hpp>` (``include\audioRender\ManualMix\ManualFausts\rollMan.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   #include "DeckData.hpp"
   
   class RollMan {
     public:
       float RollBpm;
       float RollPower;
       ARGSETTER
       makeArgSetter()
       {
           return { { "RollBpm",
                      [this](double value) {
                          this->RollBpm = static_cast<float>(value);
                      } },
                    { "RollPower", [this](double value) {
                         this->RollPower = static_cast<float>(value);
                     } } };
       }
   };
