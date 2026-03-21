
.. _program_listing_file_include_core_db_Capnp_Translators_FrameCalcCore.hpp:

Program Listing for File FrameCalcCore.hpp
==========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_core_db_Capnp_Translators_FrameCalcCore.hpp>` (``include\core\db\Capnp\Translators\FrameCalcCore.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include "PDJE_EXPORT_SETTER.hpp"
   #include "PDJE_LOG_SETTER.hpp"
   #include <cmath>
   #include <cstdint>
   #include <vector>
   #define CHANNEL 2
   #define SAMPLERATE 48000
   #define DSAMPLERATE 48000.0
   #define DMINUTE 60.0
   
   #define APPRX(TYPE, BEAT, SUBBEAT, SEP)                                        \
       static_cast<TYPE>(BEAT) +                                                  \
           (static_cast<TYPE>(SUBBEAT) / static_cast<TYPE>(SEP))
   
   namespace FrameCalc {
   
   static inline uint64_t
   CountFrame(uint64_t Sbeat,
              uint64_t SsubBeat,
              uint64_t Sseparate,
              uint64_t Ebeat,
              uint64_t EsubBeat,
              uint64_t Eseparate,
              double   bpm)
   {
       Sseparate   = Sseparate > 0 ? Sseparate : 1;
       Eseparate   = Eseparate > 0 ? Eseparate : 1;
       bpm         = bpm > 0 ? bpm : 1;
       auto Sapprx = APPRX(double, Sbeat, SsubBeat, Sseparate);
       auto Eapprx = APPRX(double, Ebeat, EsubBeat, Eseparate);
       if (Sapprx > Eapprx) {
           critlog("Failed to Count Frame. Start apprx position is bigger than "
                   "End apprx position. Start apprx: ");
           critlog(Sapprx);
           critlog("End apprx: ");
           critlog(Eapprx);
       }
       return static_cast<unsigned long>(
           std::round((Eapprx - Sapprx) * (DMINUTE / bpm) * DSAMPLERATE));
   }
   }; // namespace FrameCalc
   
   struct BpmFragment {
       uint64_t beat          = 0;
       uint64_t subBeat       = 0;
       uint64_t separate      = 0;
       uint64_t frame_to_here = 0;
       double   bpm           = 0;
   };
   
   struct BpmStruct {
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
