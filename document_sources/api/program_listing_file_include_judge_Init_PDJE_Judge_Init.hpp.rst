
.. _program_listing_file_include_judge_Init_PDJE_Judge_Init.hpp:

Program Listing for File PDJE_Judge_Init.hpp
============================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_judge_Init_PDJE_Judge_Init.hpp>` (``include/judge/Init/PDJE_Judge_Init.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include "PDJE_Core_DataLine.hpp"
   #include "PDJE_EXPORT_SETTER.hpp"
   #include "PDJE_Input_DataLine.hpp"
   #include "PDJE_Judge_Init_Structs.hpp"
   
   #include "InputParser.hpp"
   #include "PDJE_Note_OBJ.hpp"
   #include "PDJE_RAIL.hpp"
   #include <optional>
   #include <unordered_map>
   namespace PDJE_JUDGE {
   
   class PDJE_API Judge_Init {
     private:
       void
       DefaultFill(NOTE            &obj,
                   const uint64_t   railid,
                   const LOCAL_TIME axis1,
                   const LOCAL_TIME axis2);
   
     public:
       Custom_Events                       lambdas;
       std::optional<PDJE_CORE_DATA_LINE>  coreline;
       std::optional<PDJE_INPUT_DATA_LINE> inputline;
   
       // note object
       std::optional<OBJ> note_objects;
   
       // rules
       std::optional<EVENT_RULE> ev_rule;
   
       RAIL_DB raildb;
   
       void
       SetRail(const DeviceData &devData,
               const BITMASK     DeviceKey,
               const int64_t     offset_microsecond,
               const uint64_t    MatchRail);
   
       void
       SetRail(const std::string &midi_port_name,
               const uint64_t     MatchRail,
               const uint8_t      type,
               const uint8_t      ch,
               const uint8_t      pos                = 0,
               const int64_t      offset_microsecond = 0);
       void
       SetRail(const libremidi::input_port &midi_port,
               const uint64_t               MatchRail,
               const uint8_t                type,
               const uint8_t                ch,
               const uint8_t                pos                = 0,
               const int64_t                offset_microsecond = 0);
       void
       SetEventRule(const EVENT_RULE &event_rule);
   
       void
       SetCustomEvents(const Custom_Events &events);
   
       void
       NoteObjectCollector(const std::string        noteType,
                           const uint16_t           noteDetail,
                           const std::string        firstArg,
                           const std::string        secondArg,
                           const std::string        thirdArg,
                           const unsigned long long Y_Axis,
                           const unsigned long long Y_Axis_2,
                           const uint64_t           railID);
   
       void
       SetCoreLine(const PDJE_CORE_DATA_LINE &coreline);
       void
       SetInputLine(const PDJE_INPUT_DATA_LINE &inputline);
   };
   }; // namespace PDJE_JUDGE
