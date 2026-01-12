
.. _program_listing_file_include_judge_Loop_PreProcess_PDJE_PreProcess.cpp:

Program Listing for File PDJE_PreProcess.cpp
============================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_judge_Loop_PreProcess_PDJE_PreProcess.cpp>` (``include\judge\Loop\PreProcess\PDJE_PreProcess.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "PDJE_PreProcess.hpp"
   
   namespace PDJE_JUDGE {
   
   bool
   PreProcess::GetDatas()
   {
       if (init->inputline->input_arena) {
           init->inputline->input_arena->Receive();
           Parse(parsed_res, init->raildb, init->inputline->input_arena->datas);
       }
       if (init->inputline->midi_datas) {
           auto got = init->inputline->midi_datas->Get();
           Parse(parsed_res, init->raildb, *got);
       }
       if ((parsed_res.logs.empty() || init->inputline->input_arena == nullptr) &&
           (parsed_res.midi_logs.empty() ||
            init->inputline->midi_datas == nullptr)) {
           return false;
       } else {
           return true;
       }
   }
   
   void
   PreProcess::Cut(const uint64_t cut_range)
   {
       missed_buffers.clear();
       init->note_objects->Cut<BUFFER_MAIN>(cut_range, missed_buffers);
   
       if (!missed_buffers.empty()) {
           Event_Datas.miss_queue.Write(missed_buffers);
       }
       missed_buffers.clear();
       init->note_objects->Cut<BUFFER_SUB>(cut_range, missed_buffers);
       if (!missed_buffers.empty()) {
   
           Event_Datas.miss_queue.Write(missed_buffers);
       }
   }
   
   }; // namespace PDJE_JUDGE
