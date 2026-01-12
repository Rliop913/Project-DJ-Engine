
.. _program_listing_file_include_judge_Loop_Match_MIDI.cpp:

Program Listing for File MIDI.cpp
=================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_judge_Loop_Match_MIDI.cpp>` (``include\judge\Loop\Match\MIDI.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "PDJE_Judge_Loop.hpp"
   
   namespace PDJE_JUDGE {
   
   void
   Match::UseEvent(const PDJE_MIDI::MIDI_EV &ilog)
   {
   
       RAIL_KEY::MIDI key;
       key.ch = ilog.ch;
       key.port_name.assign(ilog.port_name, ilog.port_name_len);
       key.pos  = ilog.pos;
       key.type = ilog.type;
       auto res = init->raildb.GetID(key);
       if (!res) {
           return;
       }
   
       switch (ilog.type) {
       case static_cast<uint8_t>(libremidi::message_type::NOTE_ON): {
           init->note_objects->Get<BUFFER_MAIN>(
               pre->use_range, res.value(), found_list);
           Work(ilog.highres_time, found_list, res.value(), true);
       } break;
   
       case static_cast<uint8_t>(libremidi::message_type::NOTE_OFF): {
           init->note_objects->Get<BUFFER_SUB>(
               pre->use_range, res.value(), found_list);
           Work(ilog.highres_time, found_list, res.value(), false);
       } break;
   
       case static_cast<uint8_t>(libremidi::message_type::CONTROL_CHANGE): {
           // skip until axismodel implemented.
       } break;
   
       case static_cast<uint8_t>(libremidi::message_type::PITCH_BEND): {
           // skip until axismodel implemented.
       } break;
   
       default:
           break;
       }
   }
   }; // namespace PDJE_JUDGE
