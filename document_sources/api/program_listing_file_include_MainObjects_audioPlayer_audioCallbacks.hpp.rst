
.. _program_listing_file_include_MainObjects_audioPlayer_audioCallbacks.hpp:

Program Listing for File audioCallbacks.hpp
===========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_MainObjects_audioPlayer_audioCallbacks.hpp>` (``include/MainObjects/audioPlayer/audioCallbacks.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include <optional>
   
   #include "MusicControlPannel.hpp"
   #include "PDJE_EXPORT_SETTER.hpp"
   #include <miniaudio.h>
   
   struct PDJE_API audioEngineDataStruct {
       float                            *faustPcmPP[2];
       std::optional<FXControlPannel>    FXManualPannel;
       std::optional<MusicControlPannel> MusCtrPannel;
       std::vector<float>               *pcmDataPoint;
       unsigned long long                nowCursor      = 0;
       unsigned long long                maxCursor      = 0;
       unsigned long long                consumedFrames = 0;
   
       inline std::optional<float *>
       getNowfPointer(const unsigned long frameCount);
   
       inline void
       CountUp(const unsigned long frameCount);
   
       void
       GetAfterManFX(float *pOutput, unsigned long frameCount);
   
       void
       Get(float *pOutput, unsigned long frameCount);
   };
