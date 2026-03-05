
.. _program_listing_file_include_core_audioRender_ManualMix_MusicControlPanel.cpp:

Program Listing for File MusicControlPanel.cpp
==============================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_core_audioRender_ManualMix_MusicControlPanel.cpp>` (``include\core\audioRender\ManualMix\MusicControlPanel.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "MusicControlPanel.hpp"
   
   // #undef HWY_TARGET_INCLUDE
   // #define HWY_TARGET_INCLUDE "MusicControlPanel-inl.h"
   // #include "hwy/foreach_target.h"
   // #include <hwy/highway.h>
   #include "Decoder.hpp"
   #include "MusicControlPanel-inl.h"
   #include "PDJE_LOG_SETTER.hpp"
   
   MusicControlPanel::~MusicControlPanel()
   {
   }
   
   bool
   MusicControlPanel::LoadMusic(litedb &ROOTDB, const musdata &Mus)
   {
   
       if (!deck.try_emplace(Mus.title).second) {
           critlog("failed to load music from MusicControlPanel LoadMusic. "
                   "ErrTitle: ");
           critlog(Mus.title);
           return false;
       }
       if (!deck[Mus.title].loaded.init(ROOTDB, Mus.musicPath)) {
           critlog("failed to load music.");
           return false;
       }
       deck[Mus.title].Init(bufferSZ);
       return true;
   }
   
   bool
   MusicControlPanel::CueMusic(const UNSANITIZED       &title,
                               const unsigned long long newPos)
   {
       auto safeTitle = PDJE_Name_Sanitizer::sanitizeFileName(title);
       if (!safeTitle) {
           critlog("failed to sanitize title from MusicControlPanel CueMusic. "
                   "ErrTitle: ");
           critlog(title);
           return false;
       }
       if (deck.find(safeTitle.value()) == deck.end()) {
           warnlog("failed to find music from deck from MusicControlPanel "
                   "CueMusic. ErrTitle: ");
           warnlog(title);
           return false;
       }
       deck[safeTitle.value()].loaded.changePos(newPos);
   
       deck[safeTitle.value()].pb->Reset();
       return true;
   }
   
   bool
   MusicControlPanel::SetMusic(const UNSANITIZED &title, const bool onOff)
   {
       auto safeTitle = PDJE_Name_Sanitizer::sanitizeFileName(title);
       if (!safeTitle) {
           critlog("failed to sanitize title from MusicControlPanel SetMusic. "
                   "ErrTitle: ");
           critlog(title);
           return false;
       }
       if (deck.find(safeTitle.value()) == deck.end()) {
           warnlog("failed to find music from deck from MusicControlPanel "
                   "SetMusic. ErrTitle: ");
           warnlog(title);
           return false;
       }
       deck[safeTitle.value()].play = onOff;
       return true;
   }
   
   LOADED_LIST
   MusicControlPanel::GetLoadedMusicList()
   {
       LOADED_LIST list;
       for (auto &i : deck) {
           UNSANITIZED originTitle = PDJE_Name_Sanitizer::getFileName(i.first);
           list.push_back(originTitle);
       }
       return std::move(list);
   }
   
   bool
   MusicControlPanel::UnloadMusic(const UNSANITIZED &title)
   {
       auto safeTitle = PDJE_Name_Sanitizer::sanitizeFileName(title);
       if (!safeTitle) {
           critlog("failed to sanitize title from MusicControlPanel UnloadMusic. "
                   "ErrTitle: ");
           critlog(title);
           return false;
       }
       return deck.erase(safeTitle.value()) != 0;
   }
   
   HWY_EXPORT(GetPCMFramesSIMD);
   
   bool
   MusicControlPanel::GetPCMFrames(float *array, const unsigned long FrameSize)
   {
       return HWY_DYNAMIC_DISPATCH(GetPCMFramesSIMD)(
           tempFrames, L, R, FaustStyle, deck, array, FrameSize);
   }
   
   FXControlPanel *
   MusicControlPanel::getFXHandle(const UNSANITIZED &title)
   {
       auto safeTitle = PDJE_Name_Sanitizer::sanitizeFileName(title);
       if (!safeTitle) {
           critlog("failed to sanitize title from MusicControlPanel getFXHandle. "
                   "ErrTitle: ");
           critlog(title);
           return nullptr;
       }
       if (deck.find(safeTitle.value()) == deck.end()) {
           warnlog("failed to find music from deck. Err from MusicControlPanel "
                   "getFXHandle. ErrTitle: ");
           warnlog(title);
           return nullptr;
       } else {
           return deck[safeTitle.value()].fxP;
       }
   }
   
   bool
   MusicControlPanel::ChangeBpm(const UNSANITIZED &title,
                                const double       targetBpm,
                                const double       originBpm)
   {
       auto safeTitle = PDJE_Name_Sanitizer::sanitizeFileName(title);
       if (!safeTitle) {
           critlog("failed to sanitize title from MusicControlPanel SetMusic. "
                   "ErrTitle: ");
           critlog(title);
           return false;
       }
       if (deck.find(safeTitle.value()) == deck.end()) {
           warnlog("failed to find music from deck from MusicControlPanel "
                   "SetMusic. ErrTitle: ");
           warnlog(title);
           return false;
       } else {
           deck[safeTitle.value()].st->setTempo(targetBpm / originBpm);
           PREDICT temp;
           if (deck[safeTitle.value()].pb->Pop(temp)) {
               deck[safeTitle.value()].loaded.changePos(temp.start_cursor);
           }
           deck[safeTitle.value()].pb->Reset();
           return true;
       }
   }
