#include "PDJE_MIDI.hpp"

namespace PDJE_MIDI {
MIDI::MIDI(const bool CC_LSB_ON, const int buffer_size) : evlog(buffer_size)
{
    startlog();
    libremidi::input_configuration input_config{

        .on_message =
            [this, &CC_LSB_ON](const libremidi::message &m) {
                try {
                    const auto &b = m.bytes;
                    if (b.empty()) {
                        return;
                    }
                    auto       type = m.get_message_type();
                    const auto ch =
                        static_cast<uint8_t>(m.get_channel() & 0x0F); // 0 ~ 15
                    uint8_t  pos   = 0;
                    uint16_t value = 0;
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
                        CONTROL_CHANGE: // status, data num, value or MSB, LSB
                    {
                        B_GUARD(b, 3)

                        pos   = PDJE_BIT_PARSE_7(b[1]);
                        value = PDJE_BIT_PARSE_7(b[2]);
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
                    case libremidi::message_type::PITCH_BEND: // ch - LSB - MSB
                    {
                        B_GUARD(b, 3)

                        value =
                            (static_cast<uint16_t>(PDJE_BIT_PARSE_7(b[1]))) |
                            (static_cast<uint16_t>(PDJE_BIT_PARSE_7(b[2]))
                             << 7);
                    } break;

                    case libremidi::message_type::AFTERTOUCH: // ch - pressure
                    {
                        B_GUARD(b, 2)

                        value = PDJE_BIT_PARSE_7(b[1]);
                    } break;
                    case libremidi::message_type::POLY_PRESSURE: // ch - note -
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

                    evlog.Write(
                        MIDI_EV{ .type         = static_cast<uint8_t>(type),
                                 .ch           = ch,
                                 .pos          = pos,
                                 .value        = value,
                                 .highres_time = clock.Get_MicroSecond() });
                } catch (const std::exception &e) {
                    critlog("runtime error on midi input loop. What: ");
                    critlog(e.what());
                    return;
                }
            },

        .timestamps = libremidi::NoTimestamp
    };

    midiin = libremidi::midi_in{ input_config };
}
} // namespace PDJE_MIDI