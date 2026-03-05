
.. _program_listing_file_include_core_audioRender_MixMachine_MUSIC_CTR_MUSIC_CTR.cpp:

Program Listing for File MUSIC_CTR.cpp
======================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_core_audioRender_MixMachine_MUSIC_CTR_MUSIC_CTR.cpp>` (``include\core\audioRender\MixMachine\MUSIC_CTR\MUSIC_CTR.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "MUSIC_CTR.hpp"
   #include "PDJE_LOG_SETTER.hpp"
   
   MUSIC_CTR::MUSIC_CTR()
   {
       st.emplace();
       st->setChannels(CHANNEL);
       st->setSampleRate(SAMPLERATE);
       st->setSetting(0, 1);
       st->setSetting(2, 0);
       st->setTempo(1.0);
       D.emplace();
   }
   
   bool
   MUSIC_CTR::SendData(soundtouch::SoundTouch *&stp, Decoder *&dp)
   {
       if (st.has_value() && D.has_value()) {
           stp = &(st.value());
           dp  = &(D.value());
           return true;
       } else {
           critlog("failed to give soundtouch & decoder object from MUSIC_CTR "
                   "SendData.");
           return false;
       }
   }
   
   void
   MUSIC_CTR::ChangeBpm(double targetbpm, double originBpm)
   {
       st->setTempo(targetbpm / originBpm);
   }
   
   bool
   MUSIC_CTR::checkUsable()
   {
       // if(!PausePos.has_value() && FullPos.has_value()){
       //     PausePos = FullPos.value();
       // }
       bool Usable = D.has_value() && songPath.has_value();
   
       if (!Usable) {
           critlog("usable check failed. Decoder or songpath not valid from "
                   "MUSIC_CTR checkUsable.");
           if (D.has_value()) {
               critlog("songPath is not valid");
           } else {
               critlog("Decoder is not valid");
           }
       }
       return Usable;
   }
   
   bool
   MUSIC_CTR::TimeStretch(const FRAME_POS Frame, float *&masterPTR)
   {
   
       const FRAME_POS Sola = Frame / st->getInputOutputSampleRatio();
       if (!D->getRange(Sola, timeStretchBuffer)) {
           critlog("failed to get musicdata from decoder. From MUSIC_CTR "
                   "timeStretch.");
           return false;
       }
       st->putSamples(timeStretchBuffer.data(), Sola);
       st->receiveSamples(masterPTR, Frame);
       masterPTR += (Frame * CHANNEL);
       return true;
   }
   
   bool
   MUSIC_CTR::Render(const double     targetBpm,
                     const double     originBpm,
                     const LOCAL_POS  LocalIDX,
                     const GLOBAL_POS RenderAmount,
                     float          *&masterPTR)
   {
       D->changePos(LocalIDX);
       ChangeBpm(targetBpm, originBpm);
       FRAME_POS ItrTimes   = RenderAmount / BPM_WINDOWS_SIZE;
       FRAME_POS remainLast = RenderAmount % BPM_WINDOWS_SIZE;
       for (auto j = 0; j < ItrTimes - 1; ++j) {
           if (!TimeStretch(BPM_WINDOWS_SIZE, masterPTR)) {
               critlog("failed to Timestretch. From MUSIC_CTR Render");
               return false;
           }
       }
       if (!TimeStretch(BPM_WINDOWS_SIZE + remainLast, masterPTR)) {
           critlog("failed to Timestretch. From MUSIC_CTR Render");
           return false;
       }
       return true;
   }
   
   std::optional<SIMD_FLOAT *>
   MUSIC_CTR::Execute(const BPM &bpms, SIMD_FLOAT *PCMS, litedb &db)
   {
       if (!checkUsable()) {
           critlog("failed to execute because usable check failed. From MUSIC_CTR "
                   "Execute");
           return std::nullopt;
       }
       if (!D->init(db, songPath.value())) {
           critlog("failed to execute because Decoder init failed. From MUSIC_CTR "
                   "Execute");
           return std::nullopt;
       }
       QDatas.Ready(bpms.bpmVec, Mus.bpms);
   
       GLOBAL_POS RfullFrameSize =
           QDatas.pos.back().Gidx - QDatas.pos.front().Gidx;
   
       PCMS->resize(RfullFrameSize * CHANNEL);
       auto masterPTR = PCMS->data();
       for (unsigned int i = 0; i < QDatas.pos.size() - 1; ++i) {
           if (QDatas.pos[i].status == PLAY) {
               GLOBAL_POS range = QDatas.pos[i + 1].Gidx - QDatas.pos[i].Gidx;
               Render(QDatas.pos[i].TargetBPM,
                      QDatas.pos[i].OriginBPM,
                      QDatas.pos[i].Lidx,
                      range,
                      masterPTR);
           }
       }
   
       return PCMS;
   }
   bool
   MUSIC_CTR::setLOAD(MBData::Reader &RP, litedb &db, FRAME_POS FrameIn)
   {
       musdata md;
       md.title    = RP.getFirst();
       md.composer = RP.getSecond();
       md.bpm      = std::stod(RP.getThird().cStr());
   
       auto searchRes = db << md;
       if (!searchRes.has_value()) {
           critlog("search music failed. From MUSIC_CTR setLOAD. ErrTitle: ");
           critlog(md.title);
           return false;
       }
       if (searchRes->empty()) {
           critlog(
               "cannot find music from DB. From MUSIC_CTR setLOAD. ErrTitle: ");
           critlog(md.title);
           return false;
       }
       songPath = searchRes.value()[0].musicPath;
       PlayPosition startpos;
       startpos.Gidx = FrameIn;
   
       try {
           startpos.Lidx = std::stoull(searchRes.value()[0].firstBeat);
       } catch (std::exception &e) {
           critlog("failed to convert string to unsigned longlong. From MUSIC_CTR "
                   "setLOAD. ErrTitle: ");
           critlog(md.title);
           return false;
       }
       startpos.status = MIXSTATE::PLAY;
       QDatas.pos.push_back(startpos);
   
       if (!capnpMus.open(searchRes.value()[0].bpmBinary)) {
           critlog(
               "failed to open capnpBinary. From MUSIC_CTR setLOAD. ErrTitle: ");
           critlog(md.title);
           return false;
       }
       if (!Mus.Read(capnpMus, startpos.Lidx)) {
           critlog(
               "failed to Read CapnpReader. From MUSIC_CTR setLOAD. ErrTitle: ");
           critlog(md.title);
           return false;
       }
       return true;
   }
