
.. _program_listing_file_include_audioRender_MixMachine_MUSIC_CTR_MUSIC_CTR.hpp:

Program Listing for File MUSIC_CTR.hpp
======================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_audioRender_MixMachine_MUSIC_CTR_MUSIC_CTR.hpp>` (``include/audioRender/MixMachine/MUSIC_CTR/MUSIC_CTR.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include <optional>
   
   #include <SoundTouch.h>
   
   #include "DeckData.hpp"
   #include "Decoder.hpp"
   #include "MixTranslator.hpp"
   #include "MusicTranslator.hpp"
   #include "dbRoot.hpp"
   
   constexpr FRAME_POS BPM_WINDOWS_SIZE = SAMPLERATE / 1000;
   
   using GLOBAL_POS = FRAME_POS;
   using LOCAL_POS  = FRAME_POS;
   
   #define GET_BIGGER(A, B) (A < B ? B : A)
   
   enum MIXSTATE {
       PLAY,
       PAUSE,
       END,
       BPMCHANGE,
   };
   
   struct PDJE_API PlayPosition {
       GLOBAL_POS Gidx;
       LOCAL_POS  Lidx;
       double     OriginBPM = -1;
       double     TargetBPM = -1;
       MIXSTATE   status    = MIXSTATE::PLAY;
   };
   
   struct PDJE_API Ingredients {
     private:
       void
       SORT();
   
       void
       FillGlobal(std::vector<PlayPosition> &Gbpm, const BpmStruct &Global);
       void
       FillLocal(std::vector<PlayPosition> &Lbpm, const BpmStruct &Local);
       std::vector<PlayPosition>::iterator
       GetSameGidx(GLOBAL_POS gidx);
   
     public:
       void
       Ready(const BpmStruct &Global, const BpmStruct &Local);
   
       std::vector<PlayPosition> pos;
   };
   class PDJE_API MUSIC_CTR {
     private:
       std::optional<soundtouch::SoundTouch> st;
       std::optional<Decoder>                D;
       CapReader<MusicBinaryCapnpData>       capnpMus;
       MusicTranslator                       Mus;
       std::vector<float>                    timeStretchBuffer;
       bool
       checkUsable();
   
       bool
       TimeStretch(const FRAME_POS Frame, float *&masterPTR);
       bool
       Render(const double     targetBpm,
              const double     originBpm,
              const LOCAL_POS  LocalIDX,
              const GLOBAL_POS RenderAmount,
              float          *&masterPTR);
       void
       ChangeBpm(double targetbpm, double originbpm);
   
     public:
       Ingredients QDatas;
       MUSIC_CTR();
       ~MUSIC_CTR() = default;
       bool
       setLOAD(MBData::Reader &RP, litedb &db, FRAME_POS FrameIn);
       // bool AddFrameCut(FRAME_POS cutIn, FRAME_POS cutOut);
   
       // std::optional<double> originBpm;
       std::optional<std::string> songPath;
       std::optional<SIMD_FLOAT *>
       Execute(const BPM &bpms, SIMD_FLOAT *PCMS, litedb &db);
   
       bool
       SendData(soundtouch::SoundTouch *&stp, Decoder *&dp);
   };
