
.. _program_listing_file_include_judge_Loop_PDJE_Judge_Loop.hpp:

Program Listing for File PDJE_Judge_Loop.hpp
============================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_judge_Loop_PDJE_Judge_Loop.hpp>` (``include/judge/Loop/PDJE_Judge_Loop.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   #include "Input_State.hpp"
   #include "PDJE_Buffer.hpp"
   #include "PDJE_Highres_Clock.hpp"
   #include "PDJE_Judge_Init.hpp"
   #include "PDJE_Note_OBJ.hpp"
   #include "PDJE_Rule.hpp"
   #include "PDJE_SYNC_CORE.hpp"
   #include <cstdint>
   #include <optional>
   #include <thread>
   #include <unordered_map>
   #include <vector>
   
   namespace PDJE_JUDGE {
   class Judge_Loop {
   
     private:
       struct {
           std::optional<bool> use_event_switch;
           std::optional<bool> miss_event_switch;
   
           std::optional<std::thread> use_event_thread;
           std::optional<std::thread> miss_event_thread;
       } Event_Controls;
       struct useDatas {
           uint64_t railid;
           bool     Pressed;
           bool     IsLate;
           uint64_t diff;
       };
       struct {
           PDJE_Buffer_Arena<std::unordered_map<uint64_t, NOTE_VEC>> miss_queue;
           PDJE_Buffer_Arena<useDatas>                               use_queue;
       } Event_Datas;
   
     private: // cached values
       std::pmr::vector<PDJE_Input_Log> *input_log;
       struct mouse_btn_event {
           uint64_t rail_id = 0;
           int      status  = -1;
       };
       struct {
   
           std::unordered_map<uint64_t, NOTE_VEC> missed_buffers;
   
           P_NOTE_VEC found_list;
           P_NOTE_VEC related_list_out;
   
           // time values
           LOCAL_TIME local_microsecond_position;
           uint64_t   global_local_diff;
   
           LOCAL_TIME log_begin;
           LOCAL_TIME log_end;
   
           LOCAL_TIME use_range;
           LOCAL_TIME cut_range;
   
           audioSyncData synced_data;
   
           bool isLate;
   
           uint64_t railID;
   
           uint64_t                     diff;
           std::vector<mouse_btn_event> mouse_btn_event_queue;
       } Cached;
   
       Judge_Init               *init_datas;
       PDJE_HIGHRES_CLOCK::CLOCK clock_root;
       inline void
       Cut();
       bool
       PreProcess();
       void
       ParseMouse(INPUT_RULE &rule, const BITMASK ev);
       template <PDJE_Dev_Type D>
       void
       UseEvent(const PDJE_Input_Log &ilog);
       bool
       FindRailID(const INPUT_RULE &rule, uint64_t &id);
       void
       Match(const LOCAL_TIME  input_time,
             const P_NOTE_VEC &note_list,
             const uint64_t    railid,
             const bool        isPressed);
   
     public:
       void
       EndEventLoop();
       void
       StartEventLoop();
       void
       loop();
   
       std::atomic<bool> loop_switch;
       Judge_Loop(Judge_Init &inits);
       ~Judge_Loop() = default;
   };
   
   }; // namespace PDJE_JUDGE
