
.. _program_listing_file_include_judge_Loop_Match_Keyboard.cpp:

Program Listing for File Keyboard.cpp
=====================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_judge_Loop_Match_Keyboard.cpp>` (``include\judge\Loop\Match\Keyboard.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "PDJE_Judge_Init_Structs.hpp"
   #include "PDJE_Match.hpp"
   #include "PDJE_Note_OBJ.hpp"
   #include <cstdint>
   
   namespace PDJE_JUDGE {
   
   template <>
   void
   Match::UseEvent<PDJE_Dev_Type::KEYBOARD>(const PDJE_Input_Log &ilog)
   {
       RAIL_KEY::KB_MOUSE key;
       key.Device_Name.assign(ilog.name, ilog.name_len);
       key.DeviceKey = ilog.event.keyboard.k;
       auto res      = init->raildb.GetID(key);
       if (!res) {
           return;
       }
   
       if (ilog.event.keyboard.pressed) {
           init->note_objects->Get<BUFFER_MAIN>(
               pre->use_range, res.value(), found_list);
           Work(ilog.microSecond, found_list, res.value(), true);
       } else {
           init->note_objects->Get<BUFFER_SUB>(
               pre->use_range, res.value(), found_list);
           Work(ilog.microSecond, found_list, res.value(), false);
       }
   }
   }; // namespace PDJE_JUDGE
