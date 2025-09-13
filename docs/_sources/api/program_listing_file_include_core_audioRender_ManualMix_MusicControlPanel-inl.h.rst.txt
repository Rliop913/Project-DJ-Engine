
.. _program_listing_file_include_core_audioRender_ManualMix_MusicControlPanel-inl.h:

Program Listing for File MusicControlPanel-inl.h
================================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_core_audioRender_ManualMix_MusicControlPanel-inl.h>` (``include/core/audioRender/ManualMix/MusicControlPanel-inl.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   
   #include "MusicControlPanel.hpp"
   
   #undef HWY_TARGET_INCLUDE
   #define HWY_TARGET_INCLUDE "MusicControlPanel-inl.h"
   #include "hwy/foreach_target.h"
   #include <hwy/highway.h>
   
   namespace HWY_NAMESPACE {
   
   HWY_ATTR
   bool
   GetPCMFramesSIMD(SIMD_FLOAT         &tempFrames,
                    std::vector<float> &L,
                    std::vector<float> &R,
                    float             **FaustStyle,
                    LOADS              &deck,
                    float              *array,
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
       auto times    = RAWFrameSize / laneSize;
       auto remained = RAWFrameSize % laneSize;
   
       SIMD_FLOAT solaVector;
       for (auto &i : deck) {
           if (i.second.play) {
               const FRAME_POS Sola = static_cast<FRAME_POS>(
                   std::ceil(static_cast<double>(FrameSize) /
                             i.second.st->getInputOutputSampleRatio()));
               solaVector.resize(Sola * CHANNEL);
               if (ma_decoder_read_pcm_frames(
                       &i.second.dec.dec, solaVector.data(), Sola, NULL) !=
                   MA_SUCCESS) {
                   return false;
               }
   
               i.second.st->putSamples(solaVector.data(), Sola);
               i.second.st->receiveSamples(tempFrames.data(), FrameSize);
   
               toFaustStylePCM(FaustStyle, tempFrames.data(), FrameSize);
               i.second.fxP->addFX(FaustStyle, FrameSize);
               toLRStylePCM(FaustStyle, tempFrames.data(), FrameSize);
   
               float *opoint = array;
               float *tpoint = tempFrames.data();
   
               for (size_t j = 0; j < times; ++j) {
                   auto simdtemp   = hwy::HWY_NAMESPACE::Load(hwyFTag, tpoint);
                   auto simdorigin = hwy::HWY_NAMESPACE::LoadU(hwyFTag, opoint);
                   auto res        = simdtemp + simdorigin;
                   hwy::HWY_NAMESPACE::StoreU(res, hwyFTag, opoint);
                   opoint += laneSize;
                   tpoint += laneSize;
               }
   
               for (size_t j = 0; j < remained; ++j) {
                   (*(opoint++)) += (*(tpoint++));
               }
           }
       }
       return true;
   }
   } // namespace HWY_NAMESPACE
