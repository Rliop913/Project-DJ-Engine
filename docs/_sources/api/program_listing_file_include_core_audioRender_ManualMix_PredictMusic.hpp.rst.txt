
.. _program_listing_file_include_core_audioRender_ManualMix_PredictMusic.hpp:

Program Listing for File PredictMusic.hpp
=========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_core_audioRender_ManualMix_PredictMusic.hpp>` (``include\core\audioRender\ManualMix\PredictMusic.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include "FrameCalc.hpp"
   #include "PDJE_EXPORT_SETTER.hpp"
   #include <SoundTouch.h>
   #include <array>
   #include <atomic>
   #include <cstdint>
   #include <optional>
   #include <thread>
   #include <vector>
   
   struct PREDICT {
       uint64_t           start_cursor;
       uint64_t           used_frames;
       double             io_ratio = 1.0;
       std::vector<float> predict_fragment;
   };
   
   template <int SZ> class PDJE_API PredictBuffer {
     private:
       using iterator = typename std::array<PREDICT, SZ>::iterator;
       std::array<PREDICT, SZ> buffer;
       iterator                popp;
       iterator                pushp;
       std::atomic<uint32_t>   fill_counter{ 0 };
       void
       Next(iterator &itr)
       {
           ++itr;
           if (itr == buffer.end()) {
               itr = buffer.begin();
           }
       }
   
     public:
       bool
       IsFull()
       {
           return fill_counter.load(std::memory_order_acquire) == SZ;
       }
       bool
       Pop(PREDICT &out)
       {
           if (fill_counter.load(std::memory_order_acquire) == 0) {
               return false;
           }
           out = *popp;
           Next(popp);
           --fill_counter;
           return true;
       }
       void
       Reset()
       {
           popp         = buffer.begin();
           pushp        = buffer.begin();
           fill_counter = 0;
       }
       void
       Fill(const PREDICT &data)
       {
           (*pushp) = data;
           ++fill_counter;
           Next(pushp);
       }
       PredictBuffer()
       {
           popp  = buffer.begin();
           pushp = buffer.begin();
       }
   };
