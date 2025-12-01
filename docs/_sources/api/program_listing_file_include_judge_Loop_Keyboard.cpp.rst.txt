
.. _program_listing_file_include_judge_Loop_Keyboard.cpp:

Program Listing for File Keyboard.cpp
=====================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_judge_Loop_Keyboard.cpp>` (``include/judge/Loop/Keyboard.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "PDJE_Judge_Loop.hpp"
   #include "PDJE_Note_OBJ.hpp"
   #include <cstdint>
   
   namespace PDJE_JUDGE {
   
   template <>
   void
   Judge_Loop::UseEvent<PDJE_Dev_Type::KEYBOARD>(const PDJE_Input_Log &ilog)
   {
       if (ilog.microSecond < Cached.global_local_diff) {
           std::cout << "cutted on first" << std::endl;
           return;
       }
       INPUT_RULE rule;
   
       rule.Device_ID  = ilog.name;
       rule.DeviceType = ilog.type;
       rule.DeviceKey  = ilog.event.keyboard.k;
   
       if (!FindDevSetting(rule, Cached.setting)) {
           std::cout << "cutted on second. "
                     << ilog.microSecond - Cached.synced_data.microsecond
                     << std::endl;
           return;
       }
       uint64_t offset_applied =
           ilog.microSecond + Cached.setting.offset_microsecond;
       if (ilog.event.keyboard.pressed) {
           init_datas->note_objects->Get<BUFFER_MAIN>(
               Cached.use_range, Cached.setting.MatchRail, Cached.found_list);
           Match(offset_applied - Cached.global_local_diff,
                 Cached.found_list,
                 Cached.setting.MatchRail,
                 true);
       } else {
           init_datas->note_objects->Get<BUFFER_SUB>(
               Cached.use_range, Cached.setting.MatchRail, Cached.found_list);
           Match(offset_applied - Cached.global_local_diff,
                 Cached.found_list,
                 Cached.setting.MatchRail,
                 false);
       }
   }
   }; // namespace PDJE_JUDGE
