
.. _program_listing_file_include_core_MainObjects_audioPlayer_audioCallbacks.hpp:

Program Listing for File audioCallbacks.hpp
===========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_core_MainObjects_audioPlayer_audioCallbacks.hpp>` (``include/core/MainObjects/audioPlayer/audioCallbacks.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include "MusicControlPanel.hpp"
   #include "PDJE_EXPORT_SETTER.hpp"
   #include "PDJE_Highres_Clock.hpp"
   #include "PDJE_SYNC_CORE.hpp"
   #include <atomic>
   #include <cstdint>
   #include <miniaudio.h>
   #include <optional>
   
   struct PDJE_API audioEngineDataStruct {
       float                           *faustPcmPP[2];
       std::optional<FXControlPanel>    FXManualPanel;
       std::optional<MusicControlPanel> MusCtrPanel;
       std::vector<float>              *pcmDataPoint;
       unsigned long long               nowCursor = 0;
       unsigned long long               maxCursor = 0;
       std::atomic<audioSyncData>       syncData =
           audioSyncData{ .consumed_frames = 0, .microsecond = 0 };
       audioSyncData             cacheSync;
       PDJE_HIGHRES_CLOCK::CLOCK highres_clock;
       inline std::optional<float *>
       getNowfPointer(const unsigned long frameCount);
   
       inline void
       CountUp(const unsigned long frameCount);
   
       void
       GetAfterManFX(float *pOutput, unsigned long frameCount);
   
       void
       Get(float *pOutput, unsigned long frameCount);
   };
