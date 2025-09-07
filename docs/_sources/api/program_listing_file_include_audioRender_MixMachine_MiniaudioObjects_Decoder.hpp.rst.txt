
.. _program_listing_file_include_audioRender_MixMachine_MiniaudioObjects_Decoder.hpp:

Program Listing for File Decoder.hpp
====================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_audioRender_MixMachine_MiniaudioObjects_Decoder.hpp>` (``include/audioRender/MixMachine/MiniaudioObjects/Decoder.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include <optional>
   #include <string>
   #include <vector>
   
   #include "FrameCalc.hpp"
   #include "dbRoot.hpp"
   #include "fileNameSanitizer.hpp"
   #include <filesystem>
   #include <miniaudio.h>
   
   namespace fs = std::filesystem;
   // using MAYBE_FRAME = std::optional<std::vector<float>>;
   
   using FRAME_POS = unsigned long long;
   struct PDJE_API Decoder {
       ma_decoder           dec;
       std::vector<uint8_t> musicBinary;
       Decoder();
       ~Decoder();
       bool
       init(litedb &db, const SANITIZED_ORNOT &KeyOrPath);
       bool
       changePos(FRAME_POS Pos);
   
       bool
       getPos(FRAME_POS &pos);
   
       bool
       getRange(FRAME_POS numFrames, std::vector<float> &buffer);
   };
