
.. _program_listing_file_include_judge_PDJE_Judge.cpp:

Program Listing for File PDJE_Judge.cpp
=======================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_judge_PDJE_Judge.cpp>` (``include/judge/PDJE_Judge.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "PDJE_Judge.hpp"
   
   PDJE_JUDGE::PDJE_JUDGE(const PDJE_CORE_DATA_LINE& core_line)
   {
       if(core_line.maxCursor != nullptr &&
           core_line.nowCursor != nullptr &&
           core_line.preRenderedData != nullptr &&
           core_line.used_frame != nullptr){
               core = core_line;
           }
   }
