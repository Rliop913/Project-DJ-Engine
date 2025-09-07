
.. _program_listing_file_include_audioRender_ManualMix_ManualFausts_volMan.hpp:

Program Listing for File volMan.hpp
===================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_audioRender_ManualMix_ManualFausts_volMan.hpp>` (``include/audioRender/ManualMix/ManualFausts/volMan.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   #include "DeckData.hpp"
   
   class VolMan {
     public:
       float VolPower;
       ARGSETTER
       makeArgSetter()
       {
           return { { "VolPower", [this](double value) {
                         this->VolPower = static_cast<float>(value);
                     } } };
       }
   };
