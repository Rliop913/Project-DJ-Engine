
.. _program_listing_file_core_include_db_Capnp_Translators_FrameCalc.hpp:

Program Listing for File FrameCalc.hpp
======================================

|exhale_lsh| :ref:`Return to documentation for file <file_core_include_db_Capnp_Translators_FrameCalc.hpp>` (``core_include\db\Capnp\Translators\FrameCalc.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include <algorithm>
   #include <cmath>
   
   #include "CapnpBinary.hpp"
   
   #include "PDJE_EXPORT_SETTER.hpp"
   
   // #include <hwy/highway.h>
   #include <hwy/aligned_allocator.h>
   
   // namespace hn = hwy::HWY_NAMESPACE;
   
   using SIMD_FLOAT = std::vector<float, hwy::AlignedAllocator<float>>;
   
   #define CHANNEL 2
   #define SAMPLERATE 48000
   #define DSAMPLERATE 48000.0
   #define DMINUTE 60.0
   
   #define APPRX(TYPE, BEAT, SUBBEAT, SEP)                                        \
       static_cast<TYPE>(BEAT) +                                                  \
           (static_cast<TYPE>(SUBBEAT) / static_cast<TYPE>(SEP))
   
   namespace FrameCalc {
   extern unsigned long
   CountFrame(unsigned long Sbeat,
              unsigned long SsubBeat,
              unsigned long Sseparate,
              unsigned long Ebeat,
              unsigned long EsubBeat,
              unsigned long Eseparate,
              double        bpm);
   }; // namespace FrameCalc
   
   struct PDJE_API BpmFragment {
       unsigned long      beat;
       unsigned long      subBeat;
       unsigned long      separate;
       unsigned long long frame_to_here = 0;
       double             bpm;
   };
   
   struct PDJE_API BpmStruct {
       std::vector<BpmFragment> fragments;
   
       void
       sortFragment();
       bool
       calcFrame(unsigned long long StartPos = 0);
       const BpmFragment &
       getAffected(const BpmFragment &searchFrag) const;
       const BpmFragment &
       getAffected(const unsigned long long searchFrame) const;
   
       const std::vector<const BpmFragment *>
       getAffectedList(const unsigned long long searchStartFrame,
                       const unsigned long long searchEndFrame) const;
   };
   
   struct PDJE_API MixStruct {
       unsigned long long frame_in;
       unsigned long long frame_out;
       MBData::Reader RP;
   };
