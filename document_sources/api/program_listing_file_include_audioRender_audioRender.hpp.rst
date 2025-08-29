
.. _program_listing_file_include_audioRender_audioRender.hpp:

Program Listing for File audioRender.hpp
========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_audioRender_audioRender.hpp>` (``include\audioRender\audioRender.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include "ManualMix.hpp"
   #include "MixMachine.hpp"
   #include "PDJE_EXPORT_SETTER.hpp"
   class PDJE_API audioRender {
     public:
       std::optional<std::vector<float>> rendered_frames;
   
       bool
       LoadTrack(litedb &db, trackdata &td);
       bool
       LoadTrackFromMixData(litedb &db, BIN &mixData);
       audioRender()  = default;
       ~audioRender() = default;
   };
   
   enum ITPL_ENUM {
     ITPL_LINEAR =0,
     ITPL_COSINE,
     ITPL_CUBIC,
     ITPL_FLAT
   };
