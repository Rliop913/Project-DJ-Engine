
.. _program_listing_file_include_audioRender_audioRender.hpp:

Program Listing for File audioRender.hpp
========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_audioRender_audioRender.hpp>` (``include/audioRender/audioRender.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include "MixMachine.hpp"
   #include "ManualMix.hpp"
   class audioRender{
   public:
       std::optional<std::vector<float>> rendered_frames;
   
       bool LoadTrack(litedb& db, trackdata& td);
       bool LoadTrackFromMixData(litedb& db, BIN& mixData);
       audioRender() = default;
       ~audioRender() = default;
   
   };
