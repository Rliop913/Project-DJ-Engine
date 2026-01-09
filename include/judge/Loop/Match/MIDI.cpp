#include "PDJE_Judge_Loop.hpp"

namespace PDJE_JUDGE {

void
Match::UseEvent(const PDJE_MIDI::MIDI_EV &ilog)
{

    switch (ilog.type) {
    case static_cast<uint8_t>(libremidi::message_type::NOTE_ON): {

    } break;

    case static_cast<uint8_t>(libremidi::message_type::NOTE_OFF): {

    } break;

    case static_cast<uint8_t>(libremidi::message_type::CONTROL_CHANGE): {

    } break;

    case static_cast<uint8_t>(libremidi::message_type::PITCH_BEND): {

    } break;

    default:
        break;
    }
}
}; // namespace PDJE_JUDGE