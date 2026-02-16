
.. _program_listing_file_include_core_audioRender_MixMachine_MUSIC_CTR_BattleDj.hpp:

Program Listing for File BattleDj.hpp
=====================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_core_audioRender_MixMachine_MUSIC_CTR_BattleDj.hpp>` (``include/core/audioRender/MixMachine/MUSIC_CTR/BattleDj.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include "SoundTouch.h"
   
   #include <optional>
   #include <vector>
   
   #include "MUSIC_CTR.hpp"
   #include "MixTranslator.hpp"
   enum SoundTouchType { MASTER, RAW, PITCH };
   
   struct PDJE_API DJ_JOBS {
       unsigned long  sourcePoint;
       unsigned long  attachIn;
       unsigned long  attachOut;
       float          SPEED = 1.0;
       SoundTouchType STT;
       bool           getFromOrigin = true;
   };
   
   class BattleDj {
     private:
       soundtouch::SoundTouch *st;
       Decoder                *D;
       std::vector<DJ_JOBS>    jobs;
   
     public:
       BattleDj();
       ~BattleDj();
       bool
       GetDataFrom(MUSIC_CTR &mc);
       bool
       Spin(MixStruct &ms);
   
       bool
       Rev(MixStruct &ms);
   
       bool
       Scratch(MixStruct &ms);
   
       bool
                                    Pitch(MixStruct &ms);
       std::optional<unsigned long> StartPos;
   
       std::optional<SIMD_FLOAT *>
       operator<<(std::optional<SIMD_FLOAT *> Array);
   };
