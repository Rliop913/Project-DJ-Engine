
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
   #include "PDJE_Match.hpp"
   #include "PDJE_Note_OBJ.hpp"
   #include "PDJE_PreProcess.hpp"
   #include "PDJE_Rule.hpp"
   #include <cstdint>
   #include <optional>
   #include <vector>
   namespace PDJE_JUDGE {
   
   class Judge_Loop {
   
     private:
       EV_Thread Event_Controls;
   
     private:
       Judge_Init               *init_datas;
       PDJE_HIGHRES_CLOCK::CLOCK clock_root;
       PreProcess                pre;
   
       Match match;
   
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
