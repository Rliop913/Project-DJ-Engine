#include "AxisModel/MidiAxis.hpp"
#include "PDJE_Match.hpp"
#include <limits>

namespace PDJE_JUDGE {

void
Match::UseEvent(const PDJE_MIDI::MIDI_EV &ilog)
{
    const auto axis = ParseMidiAxis(ilog, init->lambdas.midi_cc_lsb_on);

    RAIL_KEY::MIDI key;
    key.ch        = ilog.ch;
    key.port_name = NormalizeRailIdentity(ilog.port_name, ilog.port_name_len);
    key.pos       = axis ? axis->pos : ilog.pos;
    key.type      = ilog.type;
    auto res      = init->raildb.GetID(key);
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

    default:
        // ParseMidiAxis is the single authority for supported axis messages.
        if (axis && init->lambdas.midi_axis &&
            ilog.highres_time <=
                static_cast<uint64_t>(std::numeric_limits<LOCAL_TIME>::max())) {
            init->lambdas.midi_axis(static_cast<LOCAL_TIME>(ilog.highres_time),
                                    res.value(),
                                    axis->value);
        }
        break;
    }
}
}; // namespace PDJE_JUDGE
