#pragma once

#include "AxisModel.hpp"
#include "PDJE_MIDI.hpp"

namespace PDJE_JUDGE {
/** @brief Validated value and canonical controller/note position. */
struct MIDI_AXIS_VALUE {
    uint8_t pos;
    double  value;
};

/**
 * Parse decoded PDJE MIDI events, not MIDI wire bytes. No device state is kept:
 * MIDI::Run has already combined CC MSB/LSB. cc_lsb_on must match that
 * producer. Paired CC 0..63 uses 0..16383 and routes both halves to controller
 * 0..31. Other CC 0..119 uses the producer's seven-bit value shifted left by
 * seven. Channel Mode CC 120..127 is not an axis. Relative CC and RPN/NRPN
 * semantics are not interpreted; the caller chooses which controllers to
 * register. Pressure uses 0..127; pitch bend uses 0..16383 with exact center
 * 8192. Malformed or unsupported events return nullopt, rather than a false
 * endpoint.
 */
[[nodiscard]] inline std::optional<MIDI_AXIS_VALUE>
ParseMidiAxis(const PDJE_MIDI::MIDI_EV &event, bool cc_lsb_on = true) noexcept
{
    if (event.ch > 15 || event.pos > 127) {
        return std::nullopt;
    }

    uint8_t pos     = event.pos;
    double  maximum = 127.0;
    double  center  = 63.5;
    switch (static_cast<libremidi::message_type>(event.type)) {
    case libremidi::message_type::CONTROL_CHANGE:
        if (event.pos >= 120) {
            return std::nullopt;
        }
        if (cc_lsb_on && event.pos < 64) {
            pos     = event.pos % 32;
            maximum = 16383.0;
        } else {
            if (event.value % 128 != 0) {
                return std::nullopt;
            }
            maximum = 16256.0;
        }
        center = maximum / 2.0;
        break;
    case libremidi::message_type::PITCH_BEND:
        if (event.pos != 0) {
            return std::nullopt;
        }
        maximum = 16383.0;
        center  = 8192.0;
        break;
    case libremidi::message_type::AFTERTOUCH:
        if (event.pos != 0) {
            return std::nullopt;
        }
        break;
    case libremidi::message_type::POLY_PRESSURE:
        break;
    default:
        return std::nullopt;
    }
    if (event.value > maximum) {
        return std::nullopt;
    }
    return MIDI_AXIS_VALUE{
        pos, *AXIS_MODEL(0.0, center, maximum).Set(event.value)
    };
}

/** @brief Value-only API sharing validation with rail routing. */
[[nodiscard]] inline std::optional<double>
NormalizeMidiAxis(const PDJE_MIDI::MIDI_EV &event,
                  bool                      cc_lsb_on = true) noexcept
{
    const auto axis = ParseMidiAxis(event, cc_lsb_on);
    return axis ? std::optional<double>{ axis->value } : std::nullopt;
}
} // namespace PDJE_JUDGE
