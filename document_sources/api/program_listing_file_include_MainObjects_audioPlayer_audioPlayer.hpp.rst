
.. _program_listing_file_include_MainObjects_audioPlayer_audioPlayer.hpp:

Program Listing for File audioPlayer.hpp
========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_MainObjects_audioPlayer_audioPlayer.hpp>` (``include\MainObjects\audioPlayer\audioPlayer.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include "ManualMix.hpp"
   #include "MixMachine.hpp"
   #include "PDJE_Core_DataLine.hpp"
   #include "PDJE_EXPORT_SETTER.hpp"
   #include "audioCallbacks.hpp"
   #include "audioRender.hpp"
   #include <miniaudio.h>
   class PDJE_API audioPlayer {
     private:
       ma_device  player;
       ma_context ctxt;
   
       audioRender renderer;
   
       std::vector<float> LFaust;
   
       std::vector<float> RFaust;
   
       audioEngineDataStruct engineDatas;
   
       ma_device_config
       DefaultInit(const unsigned int frameBufferSize);
   
       void
       ContextInit();
   
     public:
       std::string STATUS = "OK";
   
       const std::string
       GetStatus()
       {
           return STATUS;
       }
   
       bool
       Activate();
   
       bool
       Deactivate();
   
       void
       ChangeCursorPos(unsigned long long pos);
   
       unsigned long long
       GetConsumedFrames();
       FXControlPannel *
       GetFXControlPannel(const UNSANITIZED &title = "__PDJE__MAIN__");
   
       MusicControlPannel *
       GetMusicControlPannel();
       audioPlayer(litedb            &db,
                   trackdata         &td,
                   const unsigned int frameBufferSize,
                   const bool         hasManual = false);
       audioPlayer(const unsigned int frameBufferSize);
   
       PDJE_CORE_DATA_LINE
       PullOutDataLine();
   
       ~audioPlayer();
   };
