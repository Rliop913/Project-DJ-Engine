
.. _program_listing_file_include_core_audioRender_ManualMix_MusicControlPanel.hpp:

Program Listing for File MusicControlPanel.hpp
==============================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_core_audioRender_ManualMix_MusicControlPanel.hpp>` (``include\core\audioRender\ManualMix\MusicControlPanel.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include "FrameCalc.hpp"
   #include "PreLoadedMusic.hpp"
   
   #include <chrono>
   #include <map>
   
   #include "ManualMix.hpp"
   #include "PDJE_EXPORT_SETTER.hpp"
   #include "PredictMusic.hpp"
   #include "SoundTouch.h"
   #include "dbRoot.hpp"
   #include "fileNameSanitizer.hpp"
   #include <array>
   #include <filesystem>
   namespace fs = std::filesystem;
   // #undef HWY_TARGET_INCLUDE
   // #define HWY_TARGET_INCLUDE "MusicControlPanel-inl.h"
   // #include "hwy/foreach_target.h"
   // #include <hwy/highway.h>
   
   using LOADED_LIST          = std::vector<std::string>;
   constexpr int PREDICT_SIZE = 10;
   struct MusicOnDeck {
       bool                                       play = false;
       PreLoadedMusic                             loaded;
       std::optional<PredictBuffer<PREDICT_SIZE>> pb;
       FXControlPanel                            *fxP;
       uint64_t                                   frameSZ = 0;
       std::optional<soundtouch::SoundTouch>      st;
       std::optional<std::thread>                 worker;
       std::atomic<bool>                          flag = true;
       MusicOnDeck(const MusicOnDeck &)                = delete;
       MusicOnDeck &
       operator=(const MusicOnDeck &) = delete;
       MusicOnDeck() : fxP(new FXControlPanel(48000))
       {
   
           st.emplace();
           st->setChannels(CHANNEL);
           st->setSampleRate(SAMPLERATE);
           st->setSetting(0, 1);
           st->setSetting(2, 0);
           st->setTempo(1.0);
       };
       void
       Init(const uint64_t frameSize)
       {
           frameSZ = frameSize;
           pb.emplace();
           worker.emplace([this]() { predict_loop(); });
       }
       void
       predict_loop()
       {
           SIMD_FLOAT pcmBuffer(frameSZ);
           PREDICT    pd;
           pd.predict_fragment.resize(frameSZ * CHANNEL);
           std::chrono::milliseconds sleepTime(((frameSZ * 1000) / SAMPLERATE) *
                                               (PREDICT_SIZE / 2));
           while (flag) {
               if (!pb->IsFull()) {
                   pd.io_ratio     = st->getInputOutputSampleRatio();
                   pd.start_cursor = loaded.cursor;
                   pd.used_frames  = static_cast<uint64_t>(
                       std::ceil(static_cast<double>(frameSZ) / pd.io_ratio));
                   loaded.getRange(pd.used_frames, pcmBuffer);
                   st->putSamples(pcmBuffer.data(), pd.used_frames);
                   st->receiveSamples(pd.predict_fragment.data(), frameSZ);
                   pb->Fill(pd);
               } else {
                   std::this_thread::sleep_for(sleepTime);
               }
           }
       }
       bool
       join()
       {
           if (worker) {
               if (worker->joinable()) {
                   flag = false;
                   worker->join();
                   return true;
               }
           }
           return false;
       }
       ~MusicOnDeck()
       {
           join();
           delete fxP;
       }
   };
   
   using LOADS = std::map<std::string, MusicOnDeck>;
   
   class PDJE_API MusicControlPanel {
     private:
       LOADS              deck;
       unsigned long      fsize;
       uint64_t           bufferSZ;
       std::vector<float> L;
       std::vector<float> R;
       float             *FaustStyle[2];
       SIMD_FLOAT         tempFrames;
   
     public:
       MusicControlPanel(const MusicControlPanel &) = delete;
       MusicControlPanel &
       operator=(const MusicControlPanel &) = delete;
       bool
       LoadMusic(litedb &ROOTDB, const musdata &Mus);
   
       bool
       CueMusic(const UNSANITIZED &title, const unsigned long long newPos);
   
       bool
       SetMusic(const UNSANITIZED &title, const bool onOff);
   
       LOADED_LIST
       GetLoadedMusicList();
   
       bool
       UnloadMusic(const UNSANITIZED &title);
   
       bool
       GetPCMFrames(float *array, const unsigned long FrameSize);
   
       FXControlPanel *
       getFXHandle(const UNSANITIZED &title);
   
       bool
       ChangeBpm(const UNSANITIZED &title,
                 const double       targetBpm,
                 const double       originBpm);
   
       MusicControlPanel(const unsigned long FrameSize, const uint64_t BufferSize)
           : fsize(FrameSize), bufferSZ(BufferSize)
       {
       }
       ~MusicControlPanel();
   };
