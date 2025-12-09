
.. _program_listing_file_include_judge_Init_PDJE_Judge_Init.cpp:

Program Listing for File PDJE_Judge_Init.cpp
============================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_judge_Init_PDJE_Judge_Init.cpp>` (``include/judge/Init/PDJE_Judge_Init.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "PDJE_Judge_Init.hpp"
   #include "PDJE_Input_Device_Data.hpp"
   #include "PDJE_Note_OBJ.hpp"
   #include "PDJE_Rule.hpp"
   #include <cstdint>
   
   namespace PDJE_JUDGE {
   
   void
   Judge_Init::SetCoreLine(const PDJE_CORE_DATA_LINE &_coreline)
   {
       if (_coreline.maxCursor != nullptr && _coreline.nowCursor != nullptr &&
           _coreline.preRenderedData != nullptr && _coreline.syncD != nullptr) {
           coreline = _coreline;
       }
   }
   
   void
   Judge_Init::SetCustomEvents(const Custom_Events &events)
   {
       lambdas = events;
   }
   
   void
   Judge_Init::SetInputLine(const PDJE_INPUT_DATA_LINE &_inputline)
   {
       if (_inputline.input_arena != nullptr) {
           inputline = _inputline;
       }
   }
   
   void
   Judge_Init::SetRail(const DeviceData &devData,
                       const BITMASK     DeviceKey,
                       const int64_t     offset_microsecond,
                       const uint64_t    MatchRail)
   {
       if (devData.Type == PDJE_Dev_Type::UNKNOWN) {
           return;
       }
       RAIL_META meta;
       meta.Device_Name = devData.Name;
       if (meta.Device_Name.size() > 255) {
           meta.Device_Name = std::string(meta.Device_Name, 255);
       }
       meta.DeviceKey = DeviceKey;
       RAIL_SETTINGS settings;
       settings.MatchRail          = MatchRail;
       settings.offset_microsecond = offset_microsecond;
       settings.Type               = devData.Type;
       OFFSET offset;
       offset.offset_microsecond = offset_microsecond;
       if (devparser.railData.contains(meta)) {
           return;
       }
       devparser.railData[meta]               = settings;
       devparser.offsetData[meta.Device_Name] = offset;
   }
   
   void
   Judge_Init::SetEventRule(const EVENT_RULE &event_rule)
   {
       ev_rule = event_rule;
   }
   void
   Judge_Init::DefaultFill(NOTE            &obj,
                           const uint64_t   railid,
                           const LOCAL_TIME axis1,
                           const LOCAL_TIME axis2)
   {
       note_objects->Fill<BUFFER_MAIN>(obj, railid);
       if (axis2 != 0 && axis2 > axis1) {
           obj.isDown      = false;
           obj.microsecond = axis2;
           note_objects->Fill<BUFFER_SUB>(obj, railid);
       }
   }
   void
   Judge_Init::NoteObjectCollector(const std::string        noteType,
                                   const uint16_t           noteDetail,
                                   const std::string        firstArg,
                                   const std::string        secondArg,
                                   const std::string        thirdArg,
                                   const unsigned long long Y_Axis,
                                   const unsigned long long Y_Axis_2,
                                   const uint64_t           railID)
   {
   
       if (devparser.railData.empty()) {
           return;
       }
       if (!note_objects.has_value()) {
           note_objects.emplace();
       }
       auto       tempobj  = NOTE{ .type        = noteType,
                                   .detail      = noteDetail,
                                   .first       = firstArg,
                                   .second      = secondArg,
                                   .third       = thirdArg,
                                   .microsecond = 0,
                                   .used        = false,
                                   .isDown      = true };
       LOCAL_TIME micro_Y1 = Convert_Frame_Into_MicroSecond(Y_Axis);
       LOCAL_TIME micro_Y2 = Convert_Frame_Into_MicroSecond(Y_Axis_2);
       tempobj.microsecond = micro_Y1;
   
       PDJE_Dev_Type val;
       for (const auto &k : devparser.railData) {
           if (k.second.MatchRail == railID) {
               if (k.first.Device_Name == "") {
                   return;
               }
               val = k.second.Type;
           }
       }
   
       switch (val) {
       case PDJE_Dev_Type::KEYBOARD:
           DefaultFill(tempobj, railID, micro_Y1, micro_Y2);
   
           break;
       case PDJE_Dev_Type::MOUSE:
           if (tempobj.type == "AXIS") { // axis type
               tempobj.isDown = false;
               note_objects->Fill<BUFFER_SUB>(tempobj, railID);
           } else {
               DefaultFill(tempobj, railID, micro_Y1, micro_Y2);
           }
           break;
       case PDJE_Dev_Type::MIDI:
           DefaultFill(tempobj, railID, micro_Y1, micro_Y2);
           break;
       case PDJE_Dev_Type::HID:
           DefaultFill(tempobj, railID, micro_Y1, micro_Y2);
           break;
   
       default:
           break;
       }
   }
   }; // namespace PDJE_JUDGE
