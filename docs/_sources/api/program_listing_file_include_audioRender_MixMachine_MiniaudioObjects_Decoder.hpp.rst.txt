
.. _program_listing_file_include_audioRender_MixMachine_MiniaudioObjects_Decoder.hpp:

Program Listing for File Decoder.hpp
====================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_audioRender_MixMachine_MiniaudioObjects_Decoder.hpp>` (``include/audioRender/MixMachine/MiniaudioObjects/Decoder.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include <string>
   #include <optional>
   #include <vector>
   
   #include <miniaudio.h>
   
   #include "FrameCalc.hpp"
   
   // using MAYBE_FRAME = std::optional<std::vector<float>>;
   
   using FRAME_POS = unsigned long long;
   struct Decoder{
       ma_decoder dec;
   
       Decoder();
       ~Decoder();
       bool init(const std::string& song_path, const std::string& root_path);
       bool changePos(FRAME_POS Pos);
   
       bool getPos(FRAME_POS& pos);
   
       bool getRange(FRAME_POS numFrames, std::vector<float>& buffer);
   
   };
