
.. _program_listing_file_include_judge_Init_PDJE_Judge_Init_Structs.hpp:

Program Listing for File PDJE_Judge_Init_Structs.hpp
====================================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_judge_Init_PDJE_Judge_Init_Structs.hpp>` (``include/judge/Init/PDJE_Judge_Init_Structs.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   #include "PDJE_Note_OBJ.hpp"
   #include <cstdint>
   #include <functional>
   namespace PDJE_JUDGE {
   
   // 48 kHz PCM frame -> microsecond conversion is exactly frames * 125 / 6.
   constexpr uint64_t kPcm48kFrameToMicro_Num = 125;
   constexpr uint64_t kPcm48kFrameToMicro_Den = 6;
   constexpr inline uint64_t
   Convert_Frame_Into_MicroSecond(const uint64_t pcm_frame)
   {
       const uint64_t q = pcm_frame / kPcm48kFrameToMicro_Den;
       const uint64_t r = pcm_frame % kPcm48kFrameToMicro_Den;
       return q * kPcm48kFrameToMicro_Num +
              (r * kPcm48kFrameToMicro_Num) / kPcm48kFrameToMicro_Den;
   }
   static_assert(Convert_Frame_Into_MicroSecond(48) == 1000,
                 "48 frames at 48 kHz must be 1000 us");
   static_assert(Convert_Frame_Into_MicroSecond(96) == 2000,
                 "96 frames at 48 kHz must be 2000 us");
   
   using RAIL_ID = uint64_t;
   using MISS_CALLBACK =
       std::function<void(std::unordered_map<uint64_t, NOTE_VEC>)>;
   using USE_CALLBACK                = std::function<void(
       uint64_t railid, bool Pressed, bool IsLate, uint64_t diff)>;
   using MOUSE_CUSTOM_PARSE_CALLBACK = // todo - make simpler
       std::function<void(const LOCAL_TIME     microSecond,
                          const P_NOTE_VEC    &found_events,
                          uint64_t             railID,
                          int                  x,
                          int                  y,
                          PDJE_Mouse_Axis_Type axis_type)>;
   struct PDJE_API Custom_Events {
       MISS_CALLBACK               missed_event;
       USE_CALLBACK                used_event;
       MOUSE_CUSTOM_PARSE_CALLBACK custom_mouse_parse;
       std::chrono::milliseconds   use_event_sleep_time =
           std::chrono::milliseconds(100);
       std::chrono::milliseconds miss_event_sleep_time =
           std::chrono::milliseconds(200);
   };
   
   } // namespace PDJE_JUDGE
