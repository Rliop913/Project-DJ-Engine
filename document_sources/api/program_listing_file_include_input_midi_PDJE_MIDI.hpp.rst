
.. _program_listing_file_include_input_midi_PDJE_MIDI.hpp:

Program Listing for File PDJE_MIDI.hpp
======================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_input_midi_PDJE_MIDI.hpp>` (``include/input/midi/PDJE_MIDI.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include "PDJE_Buffer.hpp"
   #include "PDJE_Highres_Clock.hpp"
   #include "PDJE_LOG_SETTER.hpp"
   #include <libremidi/libremidi.hpp>
   
   #define PDJE_BIT_PARSE_7(N) (N & 0x7F)
   #define B_GUARD(B, N)                                                          \
       if (B.size() < N)                                                          \
           return;
   namespace PDJE_MIDI {
   struct PDJE_API MIDI_EV {
       uint8_t  type;
       uint8_t  ch;
       uint8_t  pos;
       uint16_t value;
       uint64_t highres_time;
       char     port_name[256];
       uint8_t  port_name_len = 0;
   };
   
   class MIDI {
     private:
       libremidi::observer                                               obs;
       PDJE_HIGHRES_CLOCK::CLOCK                                         clock;
       std::vector<std::pair<libremidi::midi_in, libremidi::input_port>> midiin;
       std::unordered_map<std::string, std::array<std::array<uint16_t, 32>, 16>>
           __CC_stat;
   
     public:
       Atomic_Double_Buffer<MIDI_EV>      evlog;
       std::vector<libremidi::input_port> configed_devices;
       void
       Run(const bool CC_LSB_ON = true);
   
       void
       Config(const libremidi::input_port &midi_dev)
       {
           configed_devices.push_back(midi_dev);
       }
   
       std::vector<libremidi::input_port>
       GetDevices()
       {
           return obs.get_input_ports();
       }
       MIDI(const int buffer_size = 64);
       ~MIDI() = default;
   };
   }; // namespace PDJE_MIDI
