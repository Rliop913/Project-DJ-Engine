
.. _program_listing_file_include_audioRender_MixMachine_MixMachine-inl.h:

Program Listing for File MixMachine-inl.h
=========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_audioRender_MixMachine_MixMachine-inl.h>` (``include/audioRender/MixMachine/MixMachine-inl.h``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   
   #include "MixMachine.hpp"
   
   #undef HWY_TARGET_INCLUDE
   #define HWY_TARGET_INCLUDE "MixMachine-inl.h"
   #include "hwy/foreach_target.h"
   
   #include "hwy/base.h"
   #include <hwy/highway.h>
   
   namespace HWY_NAMESPACE {
   
   HWY_ATTR
   void
   INTEGRATE_PCM_SIMD(SIMD_FLOAT         &tempVec,
                      std::mutex         &renderLock,
                      std::vector<float> &rendered_out,
                      MUSIC_CTR         *&MC)
   {
       const hwy::HWY_NAMESPACE::ScalableTag<float> hwyFTag;
       auto laneSize = hwy::HWY_NAMESPACE::Lanes(hwyFTag);
       auto times    = tempVec.size() / laneSize;
       auto remained = tempVec.size() % laneSize;
   
       auto Tptr = tempVec.data();
       {
           std::lock_guard<std::mutex> locks(renderLock);
           if (rendered_out.size() < (MC->QDatas.pos.back().Gidx * CHANNEL)) {
               rendered_out.resize((MC->QDatas.pos.back().Gidx * CHANNEL));
           }
           auto Rptr =
               rendered_out.data() + (MC->QDatas.pos.front().Gidx * CHANNEL);
   
           for (size_t L = 0; L < times; ++L) {
               auto Tsimd = hwy::HWY_NAMESPACE::Load(hwyFTag, Tptr);
               auto Rsimd = hwy::HWY_NAMESPACE::LoadU(hwyFTag, Rptr);
               hwy::HWY_NAMESPACE::StoreU(Rsimd + Tsimd, hwyFTag, Rptr);
               Tptr += laneSize;
               Rptr += laneSize;
           }
           for (size_t REM = 0; REM < remained; ++REM) {
               (*(Rptr++)) += (*(Tptr++));
           }
       }
   }
   
   } // namespace HWY_NAMESPACE
