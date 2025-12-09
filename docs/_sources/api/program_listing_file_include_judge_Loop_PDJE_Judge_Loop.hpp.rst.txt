
.. _program_listing_file_include_judge_Loop_PDJE_Judge_Loop.hpp:

Program Listing for File PDJE_Judge_Loop.hpp
============================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_judge_Loop_PDJE_Judge_Loop.hpp>` (``include/judge/Loop/PDJE_Judge_Loop.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   #include "InputParser.hpp"
   #include "Input_State.hpp"
   
   #include "PDJE_Highres_Clock.hpp"
   #include "PDJE_Judge_Init.hpp"
   #include "PDJE_Judge_Loop_Structs.hpp"
   #include "PDJE_Note_OBJ.hpp"
   #include "PDJE_Rule.hpp"
   #include <cstdint>
   #include <optional>
   #include <vector>
   
   namespace PDJE_JUDGE {
   
   class Judge_Loop {
   
     private:
       EV_Thread Event_Controls;
   
       Queues Event_Datas;
   
     private: // cached values
       std::pair<PDJE_Input_Log *, uint64_t> input_log;
   
       LoopCached Cached;
   
       Judge_Init               *init_datas;
       PDJE_HIGHRES_CLOCK::CLOCK clock_root;
       inline void
       Cut(const uint64_t cut_range);
       PARSE_OUT *
       PreProcess();
   
       std::optional<uint64_t>
       QueryRailid(const RAIL_META &meta)
       {
           auto itr = init_datas->devparser.railData.find(meta);
           if (itr != init_datas->devparser.railData.end()) {
               if (itr->first == meta) {
                   return itr->second.MatchRail;
               }
           }
           return std::nullopt;
       }
       void
       ParseMouse(const BITMASK ev);
   
       template <PDJE_Dev_Type D>
       void
       UseEvent(const PDJE_Input_Log &ilog);
       // bool
       // FindDevSetting(const INPUT_RULE &rule, INPUT_SETTING &setting);
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
