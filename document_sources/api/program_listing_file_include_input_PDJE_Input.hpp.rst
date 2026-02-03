
.. _program_listing_file_include_input_PDJE_Input.hpp:

Program Listing for File PDJE_Input.hpp
=======================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_input_PDJE_Input.hpp>` (``include/input/PDJE_Input.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include "Input_State.hpp"
   
   #include "PDJE_EXPORT_SETTER.hpp"
   #include "PDJE_Input_DataLine.hpp"
   #include "PDJE_Input_Device_Data.hpp"
   #include "PDJE_MIDI.hpp"
   #include <barrier>
   #include <future>
   #include <optional>
   #include <random>
   #include <string>
   #include <vector>
   
   #include "DefaultDevs.hpp"
   
   class PDJE_API PDJE_Input {
     private:
       std::optional<PDJE_DEFAULT_DEVICES::DefaultDevs> default_devs;
       // std::optional<PDJE_IPC::PDJE_Input_Transfer>     input_buffer; // redef
       // on dev pipe
       bool                           FLAG_INPUT_ON = false;
       std::optional<PDJE_MIDI::MIDI> midi_engine;
       bool                           FLAG_MIDI_ON = false;
       PDJE_INPUT_STATE               state        = PDJE_INPUT_STATE::DEAD;
   
     public:
       std::vector<DeviceData>
       GetDevs();
   
       std::vector<libremidi::input_port>
       GetMIDIDevs();
       bool
       Init();
   
       bool
       Config(std::vector<DeviceData>                  &devs,
              const std::vector<libremidi::input_port> &midi_dev);
   
       bool
       Run();
   
       bool
       Kill();
   
       PDJE_INPUT_STATE
       GetState();
   
       PDJE_INPUT_DATA_LINE
       PullOutDataLine();
   
       PDJE_Input();
   
       ~PDJE_Input()
       {
           Kill();
       }
   };
