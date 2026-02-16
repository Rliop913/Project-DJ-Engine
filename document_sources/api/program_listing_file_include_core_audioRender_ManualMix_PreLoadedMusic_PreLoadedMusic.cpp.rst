
.. _program_listing_file_include_core_audioRender_ManualMix_PreLoadedMusic_PreLoadedMusic.cpp:

Program Listing for File PreLoadedMusic.cpp
===========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_core_audioRender_ManualMix_PreLoadedMusic_PreLoadedMusic.cpp>` (``include/core/audioRender/ManualMix/PreLoadedMusic/PreLoadedMusic.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "PreLoadedMusic.hpp"
   #include <algorithm>
   bool
   PreLoadedMusic::init(litedb &db, const SANITIZED_ORNOT &KeyOrPath)
   {
       Decoder dec;
       if (!dec.init(db, KeyOrPath)) {
           warnlog("failed to load music");
           return false;
       }
   
       if (ma_decoder_get_available_frames(&dec.dec, &fullSize) != MA_SUCCESS) {
           critlog("failed to get maximum frames.");
           return false;
       }
       if (!dec.getRange(fullSize, music)) {
           critlog("failed to get range");
           return false;
       }
       cursor = 0;
       p      = music.begin();
       return true;
   }
   bool
   PreLoadedMusic::changePos(FRAME_POS Pos)
   {
       if (Pos <= fullSize) {
           cursor = Pos;
           p      = music.begin() + (cursor * CHANNEL);
           return true;
       }
       return false;
   }
   
   bool
   PreLoadedMusic::getPos(FRAME_POS &pos)
   {
       pos = cursor;
       return true;
   }
   
   bool
   PreLoadedMusic::getRange(FRAME_POS numFrames, SIMD_FLOAT &buffer)
   {
       try {
   
           uint64_t copies;
           uint64_t remained   = fullSize > cursor ? fullSize - cursor : 0;
           uint64_t BufferSize = numFrames * CHANNEL;
           uint64_t zeroes     = 0;
   
           if (buffer.size() < BufferSize) {
               buffer.resize(BufferSize);
           }
   
           if (numFrames > remained) {
               copies    = remained * CHANNEL;
               zeroes    = (numFrames - remained) * CHANNEL;
               numFrames = remained;
           } else {
               copies = BufferSize;
           }
   
           std::copy(p, p + copies, buffer.data());
           p += copies;
           cursor += numFrames;
           if (zeroes != 0) {
               std::fill_n(buffer.data() + copies, zeroes, 0.0f);
           }
           return true;
       } catch (const std::exception &e) {
           critlog("failed to get pcm data. What: ");
           critlog(e.what());
           return false;
       }
   }
