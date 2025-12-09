
.. _program_listing_file_include_judge_Loop_Keyboard.cpp:

Program Listing for File Keyboard.cpp
=====================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_judge_Loop_Keyboard.cpp>` (``include/judge/Loop/Keyboard.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "PDJE_Judge_Init_Structs.hpp"
   #include "PDJE_Judge_Loop.hpp"
   #include "PDJE_Note_OBJ.hpp"
   #include <cstdint>
   
   namespace PDJE_JUDGE {
   
   template <>
   void
   Judge_Loop::UseEvent<PDJE_Dev_Type::KEYBOARD>(const PDJE_Input_Log &ilog)
   {
   
       Cached.meta.Device_Name.assign(ilog.name, ilog.name_len);
       Cached.meta.DeviceKey = ilog.event.keyboard.k;
       auto id               = QueryRailid(Cached.meta);
   
       if (!id) {
           return;
       }
       Cached.railid = id.value();
       // std::cout << "got valid key. id:" << Cached.railid << std::endl;
       if (ilog.event.keyboard.pressed) {
           init_datas->note_objects->Get<BUFFER_MAIN>(
               Cached.use_range, Cached.railid, Cached.found_list);
           // std::cout << "got range:" << Cached.found_list.size() << std::endl;
           Match(ilog.microSecond, Cached.found_list, Cached.railid, true);
       } else {
           init_datas->note_objects->Get<BUFFER_SUB>(
               Cached.use_range, Cached.railid, Cached.found_list);
           Match(ilog.microSecond, Cached.found_list, Cached.railid, false);
       }
   }
   }; // namespace PDJE_JUDGE
