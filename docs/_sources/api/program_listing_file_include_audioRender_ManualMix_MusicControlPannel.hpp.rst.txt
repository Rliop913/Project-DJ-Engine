
.. _program_listing_file_include_audioRender_ManualMix_MusicControlPannel.hpp:

Program Listing for File MusicControlPannel.hpp
===============================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_audioRender_ManualMix_MusicControlPannel.hpp>` (``include/audioRender/ManualMix/MusicControlPannel.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include <miniaudio.h>
   
   #include <map>
   
   #include "ManualMix.hpp"
   
   // #undef HWY_TARGET_INCLUDE
   // #define HWY_TARGET_INCLUDE "MusicControlPannel-inl.h"
   // #include "hwy/foreach_target.h"
   // #include <hwy/highway.h>
   
   using TITLE         = std::string;
   
   using LOADED_LIST   = std::vector<TITLE>;
   
   
   
   
   
   struct MusicOnDeck{
       bool play = false;
       ma_decoder dec;
       FXControlPannel* fxP;
       MusicOnDeck() : fxP(new FXControlPannel(48000)) {};
       ~MusicOnDeck(){
           ma_decoder_uninit(&dec);
           delete fxP;
       }
   
   };
   
   
   
   using LOADS         = std::map<TITLE, MusicOnDeck>;
   
   class MusicControlPannel{
   private:
   
       LOADS deck; 
       unsigned long fsize;
       std::vector<float> L;
       std::vector<float> R;
       float* FaustStyle[2];
       SIMD_FLOAT tempFrames;
   
   public:
       int LoadMusic(const musdata& Mus);
   
       bool CueMusic(const TITLE& title, const unsigned long long newPos);
   
       bool SetMusic(const TITLE& title, const bool onOff);
   
       LOADED_LIST GetLoadedMusicList();
   
       bool UnloadMusic(const TITLE& title);
   
       bool GetPCMFrames(float* array, const unsigned long FrameSize);
       
       FXControlPannel* getFXHandle(const TITLE& title);
       MusicControlPannel(const unsigned long FrameSize): fsize(FrameSize){}
       ~MusicControlPannel();
   
   };
   
   
   // HWY_BEFORE_NAMESPACE();
   
   // namespace hwy{
   // namespace HWY_NAMESPACE {
   
   // bool
   // GetPCMFramesSIMD(
   //     SIMD_FLOAT& tempFrames,
   //     std::vector<float>& L,
   //     std::vector<float>& R,
   //     float** FaustStyle,
   //     LOADS& deck,
   //     float* array, 
   //     const unsigned long FrameSize)
   // {
   //     const unsigned long long RAWFrameSize = FrameSize * CHANNEL;
       
   //     tempFrames.resize(RAWFrameSize);
   //     L.resize(FrameSize);
   //     R.resize(FrameSize);
   //     FaustStyle[0] = L.data();
   //     FaustStyle[1] = R.data();
   //     const hn::ScalableTag<float> hwyFTag;
   //     auto laneSize = hn::Lanes(hwyFTag);
   //     auto times = RAWFrameSize / laneSize;
   //     auto remained = RAWFrameSize % laneSize;
   
   //     for(auto& i : deck){
   //         if(i.second.play){
               
   //             if(ma_decoder_read_pcm_frames(&i.second.dec, tempFrames.data(), FrameSize, NULL) != MA_SUCCESS){
   //                 return false;
   //             }
   //             toFaustStylePCM(FaustStyle, tempFrames.data(), FrameSize);
   //             i.second.fxP->addFX(FaustStyle, FrameSize);
   //             toLRStylePCM(FaustStyle, tempFrames.data(), FrameSize);
               
   //             float* opoint = array;
   //             float* tpoint = tempFrames.data();
               
   //             for(size_t j = 0; j < times; ++j){
   //                 auto simdtemp = hn::Load(hwyFTag, tpoint);
   //                 auto simdorigin = hn::LoadU(hwyFTag, opoint);
   //                 auto res = simdtemp + simdorigin;
   //                 hn::StoreU(res, hwyFTag, opoint);
   //                 opoint += laneSize;
   //                 tpoint += laneSize;
   //             }
               
   //             for(size_t j=0; j<remained; ++j){
   //                 (*(opoint++)) += (*(tpoint++));
   //             }
   //         }
   //     }
   //     return true;
   // }
   // }
   // }
   
   
   // HWY_AFTER_NAMESPACE();
