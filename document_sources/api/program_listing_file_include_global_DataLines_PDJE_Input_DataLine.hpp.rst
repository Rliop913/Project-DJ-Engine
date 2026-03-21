
.. _program_listing_file_include_global_DataLines_PDJE_Input_DataLine.hpp:

Program Listing for File PDJE_Input_DataLine.hpp
================================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_global_DataLines_PDJE_Input_DataLine.hpp>` (``include/global/DataLines/PDJE_Input_DataLine.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include "Input_State.hpp"
   #include "Input_Transfer.hpp"
   #include "PDJE_EXPORT_SETTER.hpp"
   #include "PDJE_Input_Device_Data.hpp"
   #include "PDJE_MIDI.hpp"
   #include <unordered_map>
   using PDJE_NAME = std::string;
   using PDJE_ID   = std::string;
   
   struct PDJE_API PDJE_INPUT_DATA_LINE {
       PDJE_IPC::PDJE_Input_Transfer            *input_arena = nullptr;
       Atomic_Double_Buffer<PDJE_MIDI::MIDI_EV> *midi_datas  = nullptr;
   };
