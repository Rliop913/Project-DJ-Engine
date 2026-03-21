
.. _program_listing_file_include_judge_NoteOBJ_PDJE_Note_OBJ.cpp:

Program Listing for File PDJE_Note_OBJ.cpp
==========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_judge_NoteOBJ_PDJE_Note_OBJ.cpp>` (``include/judge/NoteOBJ/PDJE_Note_OBJ.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "PDJE_Note_OBJ.hpp"
   #include <algorithm>
   #include <cstdint>
   #include <memory>
   namespace PDJE_JUDGE {
   
   void
   OBJ::Sort()
   {
       auto compare = [](const NOTE &a, const NOTE &b) {
           return a.microsecond < b.microsecond;
       };
   
       for (auto &o : Buffer_Main) {
           std::sort(o.second.vec.begin(), o.second.vec.end(), compare);
           o.second.itr = o.second.vec.begin();
       }
       for (auto &o : Buffer_Sub) {
           std::sort(o.second.vec.begin(), o.second.vec.end(), compare);
           o.second.itr = o.second.vec.begin();
       }
   }
   
   }; // namespace PDJE_JUDGE
