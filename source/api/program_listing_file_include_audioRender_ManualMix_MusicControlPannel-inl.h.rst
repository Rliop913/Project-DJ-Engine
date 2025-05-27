
.. _program_listing_file_include_audioRender_ManualMix_MusicControlPannel-inl.h:

Program Listing for File MusicControlPannel-inl.h
=================================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_audioRender_ManualMix_MusicControlPannel-inl.h>` (``include/audioRender/ManualMix/MusicControlPannel-inl.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   
   #include "MusicControlPannel.hpp"
   
   #undef HWY_TARGET_INCLUDE
   #define HWY_TARGET_INCLUDE "MusicControlPannel-inl.h"
   #include "hwy/foreach_target.h"
   #include <hwy/highway.h>
   
   namespace HWY_NAMESPACE {
   
   HWY_ATTR
   bool
   GetPCMFramesSIMD(
       SIMD_FLOAT& tempFrames,
       std::vector<float>& L,
       std::vector<float>& R,
       float** FaustStyle,
       LOADS& deck,
       float* array,
       const unsigned long FrameSize)
   {
       const unsigned long long RAWFrameSize = FrameSize * CHANNEL;
   
       tempFrames.resize(RAWFrameSize);
       L.resize(FrameSize);
       R.resize(FrameSize);
       FaustStyle[0] = L.data();
       FaustStyle[1] = R.data();
       const hwy::HWY_NAMESPACE::ScalableTag<float> hwyFTag;
       auto laneSize = hwy::HWY_NAMESPACE::Lanes(hwyFTag);
       auto times = RAWFrameSize / laneSize;
       auto remained = RAWFrameSize % laneSize;
   
       for(auto& i : deck){
           if(i.second.play){
   
               if(ma_decoder_read_pcm_frames(&i.second.dec, tempFrames.data(), FrameSize, NULL) != MA_SUCCESS){
                   return false;
               }
               toFaustStylePCM(FaustStyle, tempFrames.data(), FrameSize);
               i.second.fxP->addFX(FaustStyle, FrameSize);
               toLRStylePCM(FaustStyle, tempFrames.data(), FrameSize);
   
               float* opoint = array;
               float* tpoint = tempFrames.data();
   
               for(size_t j = 0; j < times; ++j){
                   auto simdtemp = hwy::HWY_NAMESPACE::Load(hwyFTag, tpoint);
                   auto simdorigin = hwy::HWY_NAMESPACE::LoadU(hwyFTag, opoint);
                   auto res = simdtemp + simdorigin;
                   hwy::HWY_NAMESPACE::StoreU(res, hwyFTag, opoint);
                   opoint += laneSize;
                   tpoint += laneSize;
               }
   
               for(size_t j=0; j<remained; ++j){
                   (*(opoint++)) += (*(tpoint++));
               }
           }
       }
       return true;
   }
   }
