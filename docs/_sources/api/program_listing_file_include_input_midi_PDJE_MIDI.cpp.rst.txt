
.. _program_listing_file_include_input_midi_PDJE_MIDI.cpp:

Program Listing for File PDJE_MIDI.cpp
======================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_input_midi_PDJE_MIDI.cpp>` (``include\input\midi\PDJE_MIDI.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "PDJE_MIDI.hpp"
   
   namespace PDJE_MIDI {
   
   MIDI::MIDI(const int buffer_size) : evlog(buffer_size)
   {
       startlog();
   }
   
   void
   MIDI::Run(const bool CC_LSB_ON)
   {
       for (const auto &i : configed_devices) {
           std::string pname = i.port_name;
   
           libremidi::input_configuration input_config{
   
               .on_message =
                   [this, CC_LSB_ON, pname](const libremidi::message &m) {
                       try {
                           const auto &b = m.bytes;
                           if (b.empty()) {
                               return;
                           }
                           auto       type = m.get_message_type();
                           const auto ch   = static_cast<uint8_t>(m.get_channel() &
                                                                0x0F); // 0 ~ 15
                           uint8_t    pos  = 0;
                           uint16_t   value = 0;
   
                           switch (m.get_message_type()) {
                           case libremidi::message_type::NOTE_ON: {
                               B_GUARD(b, 3)
   
                               pos   = PDJE_BIT_PARSE_7(b[1]);
                               value = PDJE_BIT_PARSE_7(b[2]);
                               if (value == 0) {
                                   type = libremidi::message_type::NOTE_OFF;
                               }
                           } break;
                           case libremidi::message_type::NOTE_OFF: // ch - note -
                                                                   // velocity
                           {
                               B_GUARD(b, 3)
   
                               pos   = PDJE_BIT_PARSE_7(b[1]);
                               value = PDJE_BIT_PARSE_7(b[2]);
                           } break;
                           case libremidi::message_type::
                               CONTROL_CHANGE: // status, data num, value or MSB,
                                               // LSB
                           {
                               B_GUARD(b, 3)
   
                               pos           = PDJE_BIT_PARSE_7(b[1]);
                               value         = PDJE_BIT_PARSE_7(b[2]);
                               auto &CC_stat = __CC_stat[pname];
                               if (pos < 32 && CC_LSB_ON) { // msb
   
                                   CC_stat[ch][pos] =
                                       (CC_stat[ch][pos] & 0x007F) | (value << 7);
                                   value = CC_stat[ch][pos];
                               } else if (pos < 64 && CC_LSB_ON) { // lsb
                                   CC_stat[ch][pos - 32] &= 0x3F80;
                                   CC_stat[ch][pos - 32] |= value;
                                   value = CC_stat[ch][pos - 32];
   
                               } else {
                                   value = value << 7;
                               }
                           } break;
                           case libremidi::message_type::PITCH_BEND: // ch - LSB -
                                                                     // MSB
                           {
                               B_GUARD(b, 3)
   
                               value =
                                   (static_cast<uint16_t>(
                                       PDJE_BIT_PARSE_7(b[1]))) |
                                   (static_cast<uint16_t>(PDJE_BIT_PARSE_7(b[2]))
                                    << 7);
                           } break;
   
                           case libremidi::message_type::AFTERTOUCH: // ch -
                                                                     // pressure
                           {
                               B_GUARD(b, 2)
   
                               value = PDJE_BIT_PARSE_7(b[1]);
                           } break;
                           case libremidi::message_type::POLY_PRESSURE: // ch -
                                                                        // note -
                                                                        // pressure
                           {
                               B_GUARD(b, 3)
   
                               pos   = PDJE_BIT_PARSE_7(b[1]);
                               value = PDJE_BIT_PARSE_7(b[2]);
                           } break;
                           default: {
   
                               return;
                           }
                           }
                           MIDI_EV evres{ .type  = static_cast<uint8_t>(type),
                                          .ch    = ch,
                                          .pos   = pos,
                                          .value = value,
                                          .highres_time =
                                              clock.Get_MicroSecond() };
                           evres.port_name_len =
                               pname.size() > 256 ? 256 : pname.size();
                           std::memcpy(evres.port_name,
                                       pname.data(),
                                       sizeof(char) * (evres.port_name_len));
   
                           evlog.Write(evres);
                       } catch (const std::exception &e) {
                           critlog("runtime error on midi input loop. What: ");
                           critlog(e.what());
                           return;
                       }
                   },
   
               .timestamps = libremidi::NoTimestamp
           };
           __CC_stat.try_emplace(pname,
                                 std::array<std::array<uint16_t, 32>, 16>{});
   
           midiin.emplace_back(std::pair(libremidi::midi_in{ input_config }, i));
   
           auto err = midiin.back().first.open_port(i);
           if (err != stdx::error{}) {
               throw std::runtime_error(
                   std::string(err.message().data(), err.message().size()));
           }
       }
   }
   
   } // namespace PDJE_MIDI
