
.. _program_listing_file_include_audioRender_ManualMix_MusicControlPannel.cpp:

Program Listing for File MusicControlPannel.cpp
===============================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_audioRender_ManualMix_MusicControlPannel.cpp>` (``include\audioRender\ManualMix\MusicControlPannel.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "MusicControlPannel.hpp"
   
   // #undef HWY_TARGET_INCLUDE
   // #define HWY_TARGET_INCLUDE "MusicControlPannel-inl.h"
   // #include "hwy/foreach_target.h"
   // #include <hwy/highway.h>
   #include "Decoder.hpp"
   #include "MusicControlPannel-inl.h"
   #include "PDJE_LOG_SETTER.hpp"
   
   MusicControlPannel::~MusicControlPannel()
   {
   }
   
   bool
   MusicControlPannel::LoadMusic(litedb &ROOTDB, const musdata &Mus)
   {
       if (!deck.try_emplace(Mus.title).second) {
           critlog("failed to load music from MusicControlPannel LoadMusic. "
                   "ErrTitle: ");
           critlog(Mus.title);
           return false;
       }
       return deck[Mus.title].dec.init(ROOTDB, Mus.musicPath);
   }
   
   bool
   MusicControlPannel::CueMusic(const UNSANITIZED       &title,
                                const unsigned long long newPos)
   {
       auto safeTitle = PDJE_Name_Sanitizer::sanitizeFileName(title);
       if (!safeTitle) {
           critlog("failed to sanitize title from MusicControlPannel CueMusic. "
                   "ErrTitle: ");
           critlog(title);
           return false;
       }
       if (deck.find(safeTitle.value()) == deck.end()) {
           warnlog("failed to find music from deck from MusicControlPannel "
                   "CueMusic. ErrTitle: ");
           warnlog(title);
           return false;
       }
       deck[safeTitle.value()].dec.changePos(newPos * CHANNEL);
       return true;
   }
   
   bool
   MusicControlPannel::SetMusic(const UNSANITIZED &title, const bool onOff)
   {
       auto safeTitle = PDJE_Name_Sanitizer::sanitizeFileName(title);
       if (!safeTitle) {
           critlog("failed to sanitize title from MusicControlPannel SetMusic. "
                   "ErrTitle: ");
           critlog(title);
           return false;
       }
       if (deck.find(safeTitle.value()) == deck.end()) {
           warnlog("failed to find music from deck from MusicControlPannel "
                   "SetMusic. ErrTitle: ");
           warnlog(title);
           return false;
       }
       deck[safeTitle.value()].play = onOff;
       return true;
   }
   
   LOADED_LIST
   MusicControlPannel::GetLoadedMusicList()
   {
       LOADED_LIST list;
       for (auto &i : deck) {
           UNSANITIZED originTitle = PDJE_Name_Sanitizer::getFileName(i.first);
           list.push_back(originTitle);
       }
       return std::move(list);
   }
   
   bool
   MusicControlPannel::UnloadMusic(const UNSANITIZED &title)
   {
       auto safeTitle = PDJE_Name_Sanitizer::sanitizeFileName(title);
       if (!safeTitle) {
           critlog("failed to sanitize title from MusicControlPannel UnloadMusic. "
                   "ErrTitle: ");
           critlog(title);
           return false;
       }
       return deck.erase(safeTitle.value()) != 0;
   }
   
   HWY_EXPORT(GetPCMFramesSIMD);
   
   bool
   MusicControlPannel::GetPCMFrames(float *array, const unsigned long FrameSize)
   {
       return HWY_DYNAMIC_DISPATCH(GetPCMFramesSIMD)(
           tempFrames, L, R, FaustStyle, deck, array, FrameSize);
   }
   
   FXControlPannel *
   MusicControlPannel::getFXHandle(const UNSANITIZED &title)
   {
       auto safeTitle = PDJE_Name_Sanitizer::sanitizeFileName(title);
       if (!safeTitle) {
           critlog("failed to sanitize title from MusicControlPannel getFXHandle. "
                   "ErrTitle: ");
           critlog(title);
           return nullptr;
       }
       if (deck.find(safeTitle.value()) == deck.end()) {
           warnlog("failed to find music from deck. Err from MusicControlPannel "
                   "getFXHandle. ErrTitle: ");
           warnlog(title);
           return nullptr;
       } else {
           return deck[safeTitle.value()].fxP;
       }
   }
   
   bool
   MusicControlPannel::ChangeBpm(const UNSANITIZED &title,
                                 const double       targetBpm,
                                 const double       originBpm)
   {
       auto safeTitle = PDJE_Name_Sanitizer::sanitizeFileName(title);
       if (!safeTitle) {
           critlog("failed to sanitize title from MusicControlPannel SetMusic. "
                   "ErrTitle: ");
           critlog(title);
           return false;
       }
       if (deck.find(safeTitle.value()) == deck.end()) {
           warnlog("failed to find music from deck from MusicControlPannel "
                   "SetMusic. ErrTitle: ");
           warnlog(title);
           return false;
       } else {
           deck[safeTitle.value()].st->setTempo(targetBpm / originBpm);
           return true;
       }
   }
