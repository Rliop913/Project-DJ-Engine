
.. _program_listing_file_include_core_audioRender_ManualMix_PreLoadedMusic_PreLoadedMusic.hpp:

Program Listing for File PreLoadedMusic.hpp
===========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_core_audioRender_ManualMix_PreLoadedMusic_PreLoadedMusic.hpp>` (``include\core\audioRender\ManualMix\PreLoadedMusic\PreLoadedMusic.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   #include "Decoder.hpp"
   #include "FrameCalc.hpp"
   
   struct PDJE_API PreLoadedMusic {
   
       SIMD_FLOAT           music;
       ma_uint64            fullSize;
       uint64_t             cursor;
       SIMD_FLOAT::iterator p;
       PreLoadedMusic()  = default;
       ~PreLoadedMusic() = default;
   
       bool
       init(litedb &db, const SANITIZED_ORNOT &KeyOrPath);
       bool
       changePos(FRAME_POS Pos);
   
       bool
       getPos(FRAME_POS &pos);
   
       bool
       getRange(FRAME_POS numFrames, SIMD_FLOAT &buffer);
   };
