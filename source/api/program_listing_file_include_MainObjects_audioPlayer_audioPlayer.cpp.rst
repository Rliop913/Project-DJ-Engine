
.. _program_listing_file_include_MainObjects_audioPlayer_audioPlayer.cpp:

Program Listing for File audioPlayer.cpp
========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_MainObjects_audioPlayer_audioPlayer.cpp>` (``include/MainObjects/audioPlayer/audioPlayer.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "audioPlayer.hpp"
   
   
   extern void FullPreRender_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);
   
   extern void HybridRender_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);
   extern void FullManualRender_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);
   
   
   #include "MusicControlPannel.hpp"
   
   void
   audioPlayer::ContextInit()
   {
       auto conf = ma_context_config_init();
       ma_context_init(NULL, 0, &conf, &ctxt);
       ctxt.threadPriority = ma_thread_priority_high;
   }
   
   
   ma_device_config
   audioPlayer::DefaultInit(const unsigned int frameBufferSize)
   {
       ma_device_config conf = ma_device_config_init(ma_device_type_playback);
       conf.playback.format = ma_format_f32;
       conf.playback.channels = 2;
       conf.sampleRate = 48000;
       conf.periodSizeInFrames = frameBufferSize;
       conf.performanceProfile = ma_performance_profile_low_latency;
       LFaust.resize(frameBufferSize);
       RFaust.resize(frameBufferSize);
       engineDatas.faustPcmPP[0] = LFaust.data();
       engineDatas.faustPcmPP[1] = RFaust.data();
       conf.pUserData = reinterpret_cast<void*>(&engineDatas);
       ContextInit();
       return conf;
   }
   
   audioPlayer::audioPlayer(litedb& db, trackdata& td, const unsigned int frameBufferSize, const bool hasManual)
   {
       auto conf = DefaultInit(frameBufferSize);
       if(hasManual){
           conf.dataCallback = HybridRender_callback;
           engineDatas.FXManualPannel.emplace(SAMPLERATE);
           engineDatas.MusCtrPannel.emplace(SAMPLERATE);
       }
       else{
           conf.dataCallback = FullPreRender_callback;
       }
       
       if(!renderer.LoadTrack(db, td)){
           STATUS = "Failed to load track";
           return;
       }
       engineDatas.pcmDataPoint = &renderer.rendered_frames.value();
       engineDatas.maxCursor = renderer.rendered_frames->size() / CHANNEL;
       
       if(ma_device_init(&ctxt, &conf, &player) != MA_SUCCESS){
           STATUS = "Failed to init device";
           return;
       }
       
   }
   
   audioPlayer::audioPlayer(const unsigned int frameBufferSize)
   {
       ma_device_config conf = DefaultInit(frameBufferSize);
       
       conf.dataCallback = FullManualRender_callback;
       engineDatas.FXManualPannel.emplace(SAMPLERATE);
       engineDatas.MusCtrPannel.emplace(SAMPLERATE);
       
   
       if(ma_device_init(&ctxt, &conf, &player) != MA_SUCCESS){
           STATUS = "Failed to init device";
       }
       
   }
   
   bool
   audioPlayer::Activate()
   {
       return ma_device_start(&player) == MA_SUCCESS;
   }
   
   bool
   audioPlayer::Deactivate()
   {
       return ma_device_stop(&player) == MA_SUCCESS;
   }
   
   
   audioPlayer::~audioPlayer()
   {
       ma_device_uninit(&player);
       ma_context_uninit(&ctxt);
       
   }
   
   void
   audioPlayer::ChangeCursorPos(unsigned long long pos)
   {
       engineDatas.nowCursor = pos;
   }
   
   unsigned long long
   audioPlayer::GetConsumedFrames()
   {
       return  engineDatas.consumedFrames;
   }
   
   FXControlPannel*
   audioPlayer::GetFXControlPannel(const std::string& title)
   {
       if(title == "__PDJE__MAIN__"){
           if(!engineDatas.FXManualPannel.has_value()){
               engineDatas.FXManualPannel.emplace(48000);
           }
           return &engineDatas.FXManualPannel.value();
       }
       else{
           if(engineDatas.MusCtrPannel.has_value()){
               return engineDatas.MusCtrPannel->getFXHandle(title);
           }
           else{
               return nullptr;
           }
       }
   }
   
   
   MusicControlPannel* 
   audioPlayer::GetMusicControlPannel()
   {
       if(engineDatas.MusCtrPannel.has_value()){
           return &(engineDatas.MusCtrPannel.value());
       }
       else{
           return nullptr;
       }
   }
