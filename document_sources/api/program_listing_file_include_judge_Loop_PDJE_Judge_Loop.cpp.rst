
.. _program_listing_file_include_judge_Loop_PDJE_Judge_Loop.cpp:

Program Listing for File PDJE_Judge_Loop.cpp
============================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_judge_Loop_PDJE_Judge_Loop.cpp>` (``include\judge\Loop\PDJE_Judge_Loop.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "PDJE_Judge_Loop.hpp"
   
   #include "InputParser.hpp"
   #include "PDJE_Buffer.hpp"
   #include "PDJE_Input_DataLine.hpp"
   #include "PDJE_Judge_Init.hpp"
   #include "PDJE_LOG_SETTER.hpp"
   #include "PDJE_Note_OBJ.hpp"
   #include "PDJE_Rule.hpp"
   #include <chrono>
   #include <cstddef>
   #include <exception>
   
   #include "PDJE_Benchmark.hpp"
   #include <ratio>
   #include <thread>
   namespace PDJE_JUDGE {
   Judge_Loop::Judge_Loop(Judge_Init &inits) : pre(&inits), match(&pre, &inits)
   {
       init_datas = &inits;
   }
   
   void
   Judge_Loop::loop()
   {
       WBCH("judge loop started")
       while (loop_switch) {
           WBCH("judge loop head")
   
           if (!pre.Work()) {
               continue;
           }
           if (init_datas->inputline->input_arena) {
               for (const PDJE_Input_Log &input_ev : pre.parsed_res.logs) {
                   switch (input_ev.type) {
                   case PDJE_Dev_Type::KEYBOARD:
                       match.UseEvent<PDJE_Dev_Type::KEYBOARD>(input_ev);
                       break;
                   case PDJE_Dev_Type::MOUSE:
                       match.UseEvent<PDJE_Dev_Type::MOUSE>(input_ev);
                       break;
                   default:
                       break;
                   }
               }
           }
           if (init_datas->inputline->midi_datas) {
               for (const auto &midi_ev : pre.parsed_res.midi_logs) {
                   match.UseEvent(midi_ev);
               }
           }
           WBCH("judge loop tail")
       }
   }
   void
   Judge_Loop::StartEventLoop()
   {
       Event_Controls.use_event_switch = true;
       Event_Controls.use_event_thread.emplace([this]() {
           auto use_clock = std::chrono::steady_clock::now();
           WBCH("use event loop init")
           while (Event_Controls.use_event_switch.value()) {
               try {
                   WBCH("use event line head")
                   use_clock += init_datas->lambdas.use_event_sleep_time;
                   std::this_thread::sleep_until(use_clock);
   
                   auto queue = pre.Event_Datas.use_queue.Get();
                   for (const auto &used : (*queue)) {
                       init_datas->lambdas.used_event(
                           used.railid, used.Pressed, used.IsLate, used.diff);
                   }
                   WBCH("use event line tail")
               } catch (const std::exception &e) {
                   critlog("caught error on use event loop. Why:");
                   critlog(e.what());
               }
           }
       });
       Event_Controls.miss_event_switch = true;
       Event_Controls.miss_event_thread.emplace([this]() {
           auto miss_clock = std::chrono::steady_clock::now();
           WBCH("miss event init")
           while (Event_Controls.miss_event_switch.value()) {
               try {
                   WBCH("miss event line head")
                   miss_clock += init_datas->lambdas.miss_event_sleep_time;
                   std::this_thread::sleep_for(
                       init_datas->lambdas.miss_event_sleep_time);
                   auto queue = pre.Event_Datas.miss_queue.Get();
                   for (const auto &missed : (*queue)) {
                       init_datas->lambdas.missed_event(missed);
                   }
                   WBCH("miss event line tail")
               } catch (const std::exception &e) {
                   critlog("caught error on miss event loop. Why:");
                   critlog(e.what());
               }
           }
       });
   }
   
   void
   Judge_Loop::EndEventLoop()
   {
       Event_Controls.use_event_switch  = false;
       Event_Controls.miss_event_switch = false;
       if (Event_Controls.use_event_thread->joinable()) {
           Event_Controls.use_event_thread->join();
       }
       if (Event_Controls.miss_event_thread->joinable()) {
           Event_Controls.miss_event_thread->join();
       }
   }
   
   }; // namespace PDJE_JUDGE
