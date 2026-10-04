#include "AxisModel/MidiAxis.hpp"
#include "PDJE_Match.hpp"
#include <doctest/doctest.h>

#include <cstring>
#include <limits>
#include <vector>

using namespace PDJE_JUDGE;
using MidiType = libremidi::message_type;

namespace {
PDJE_MIDI::MIDI_EV
MidiEvent(MidiType type, uint8_t pos = 0, uint16_t value = 0)
{
    PDJE_MIDI::MIDI_EV event{};
    event.type         = static_cast<uint8_t>(type);
    event.pos          = pos;
    event.value        = value;
    event.highres_time = 1000;
    std::memcpy(event.port_name, "dummy-midi", 10);
    event.port_name_len = 10;
    return event;
}

struct AxisSample {
    LOCAL_TIME time;
    uint64_t   rail;
    double     value;
};

struct MidiHarness {
    Judge_Init              init;
    PreProcess              pre{ &init };
    Match                   match{ &pre, &init };
    std::vector<AxisSample> samples;

    MidiHarness()
    {
        pre.use_range = 2000;
        init.ev_rule  = EVENT_RULE{ 100, 100 };
        init.note_objects.emplace();
        Custom_Events events;
        events.midi_axis =
            [this](LOCAL_TIME time, uint64_t rail, double value) {
                samples.push_back({ time, rail, value });
            };
        init.SetCustomEvents(events);
    }

    void
    Rail(MidiType           type,
         uint8_t            pos,
         uint64_t           id,
         uint8_t            ch   = 0,
         const std::string &port = "dummy-midi")
    {
        init.SetRail(port, id, static_cast<uint8_t>(type), ch, pos);
    }
};
} // namespace

TEST_CASE(
    "midi axis: all paired CC positions share MSB routing without mutation")
{
    for (uint8_t pos = 0; pos < 32; ++pos) {
        const auto msb = MidiEvent(MidiType::CONTROL_CHANGE, pos, 12345);
        const auto lsb = MidiEvent(MidiType::CONTROL_CHANGE, pos + 32, 12345);
        const auto a   = ParseMidiAxis(msb);
        const auto b   = ParseMidiAxis(lsb);
        REQUIRE(a.has_value());
        REQUIRE(b.has_value());
        CHECK(a->pos == pos);
        CHECK(b->pos == pos);
        CHECK(a->value == b->value);
        CHECK(lsb.pos == pos + 32);
        CHECK(lsb.value == 12345);
    }
}

TEST_CASE(
    "midi axis: exhaustive CC and pitch domains stay monotonic and bounded")
{
    for (auto type : { MidiType::CONTROL_CHANGE, MidiType::PITCH_BEND }) {
        auto   event    = MidiEvent(type);
        double previous = -1.0;
        for (uint16_t value = 0; value <= 16383; ++value) {
            event.value     = value;
            const auto axis = ParseMidiAxis(event);
            REQUIRE(axis.has_value());
            CHECK(axis->value >= previous);
            CHECK(axis->value >= -1.0);
            CHECK(axis->value <= 1.0);
            CHECK(NormalizeMidiAxis(event) == axis->value);
            previous = axis->value;
        }
        CHECK(previous == 1.0);
    }
    auto pitch = MidiEvent(MidiType::PITCH_BEND, 0, 8192);
    CHECK(NormalizeMidiAxis(pitch) == 0.0);
    pitch.value = 8191;
    REQUIRE(NormalizeMidiAxis(pitch).has_value());
    CHECK(*NormalizeMidiAxis(pitch) < 0.0);
    pitch.value = 8193;
    CHECK(*NormalizeMidiAxis(pitch) > 0.0);
}

TEST_CASE(
    "midi axis: all seven-bit CC values honor mode and controller identity")
{
    for (uint8_t pos : { 0, 31, 32, 63, 64, 119 }) {
        for (uint16_t value = 0; value <= 127; ++value) {
            const auto event =
                MidiEvent(MidiType::CONTROL_CHANGE, pos, value << 7);
            const auto axis = ParseMidiAxis(event, false);
            REQUIRE(axis.has_value());
            CHECK(axis->pos == pos);
            CHECK(axis->value == doctest::Approx(2.0 * value / 127.0 - 1.0));
            if (pos >= 64) {
                CHECK(NormalizeMidiAxis(event, true) == axis->value);
            }
        }
    }
}

TEST_CASE("midi axis: pressure domains retain polyphonic note identity")
{
    for (auto type : { MidiType::AFTERTOUCH, MidiType::POLY_PRESSURE }) {
        for (uint16_t value = 0; value <= 127; ++value) {
            const uint8_t pos  = type == MidiType::POLY_PRESSURE ? 127 : 0;
            const auto    axis = ParseMidiAxis(MidiEvent(type, pos, value));
            REQUIRE(axis.has_value());
            CHECK(axis->pos == pos);
            CHECK(axis->value == doctest::Approx(2.0 * value / 127.0 - 1.0));
        }
    }
}

TEST_CASE("midi axis: malformed data and non-axis messages are rejected")
{
    auto event = MidiEvent(MidiType::CONTROL_CHANGE, 0, 16384);
    CHECK_FALSE(ParseMidiAxis(event).has_value());
    event.value = 1;
    CHECK_FALSE(ParseMidiAxis(event, false).has_value());
    event.pos = 64;
    CHECK_FALSE(ParseMidiAxis(event).has_value());
    event.value = 16384;
    CHECK_FALSE(ParseMidiAxis(event).has_value());
    event.value = 0;
    for (int pos = 120; pos <= 255; ++pos) {
        event.pos = static_cast<uint8_t>(pos);
        CHECK_FALSE(ParseMidiAxis(event).has_value());
        CHECK_FALSE(ParseMidiAxis(event, false).has_value());
    }
    event.pos = 0;
    for (uint8_t channel : { 16, 255 }) {
        event.ch = channel;
        CHECK_FALSE(ParseMidiAxis(event).has_value());
    }
    event.ch = 15;
    CHECK(ParseMidiAxis(event).has_value());

    for (auto type : { MidiType::PITCH_BEND, MidiType::AFTERTOUCH }) {
        CHECK_FALSE(ParseMidiAxis(MidiEvent(type, 1)).has_value());
        CHECK_FALSE(ParseMidiAxis(MidiEvent(type, 0, 65535)).has_value());
    }
    CHECK_FALSE(
        ParseMidiAxis(MidiEvent(MidiType::POLY_PRESSURE, 128)).has_value());
    CHECK_FALSE(
        ParseMidiAxis(MidiEvent(MidiType::POLY_PRESSURE, 0, 128)).has_value());
    for (auto type : { MidiType::NOTE_ON,
                       MidiType::NOTE_OFF,
                       static_cast<MidiType>(0),
                       static_cast<MidiType>(0xF8) }) {
        CHECK_FALSE(ParseMidiAxis(MidiEvent(type)).has_value());
    }
}

TEST_CASE("midi axis: judge maps paired halves to one rail and keeps port and "
          "channel")
{
    MidiHarness h;
    h.Rail(MidiType::CONTROL_CHANGE, 1, 42);
    h.Rail(MidiType::CONTROL_CHANGE, 33, 99); // Not a fallback in paired mode.
    h.Rail(MidiType::CONTROL_CHANGE, 1, 43, 1);
    h.Rail(MidiType::CONTROL_CHANGE, 1, 44, 0, "other-midi");
    auto event = MidiEvent(MidiType::CONTROL_CHANGE, 1, 0);
    h.match.UseEvent(event);
    event.pos          = 33;
    event.value        = 16383;
    event.highres_time = 1001;
    h.match.UseEvent(event);
    event.ch = 1;
    h.match.UseEvent(event);
    event.ch = 0;
    std::memcpy(event.port_name, "other-midi", 10);
    h.match.UseEvent(event);
    REQUIRE(h.samples.size() == 4);
    CHECK(h.samples[0].rail == 42);
    CHECK(h.samples[0].time == 1000);
    CHECK(h.samples[0].value == -1.0);
    CHECK(h.samples[1].rail == 42);
    CHECK(h.samples[1].time == 1001);
    CHECK(h.samples[1].value == 1.0);
    CHECK(h.samples[2].rail == 43);
    CHECK(h.samples[3].rail == 44);
    CHECK(event.pos == 33);
    CHECK(event.value == 16383);
}

TEST_CASE("midi axis: judge seven-bit mode does not merge separate CC rails")
{
    MidiHarness h;
    h.init.lambdas.midi_cc_lsb_on = false;
    h.Rail(MidiType::CONTROL_CHANGE, 1, 42);
    h.Rail(MidiType::CONTROL_CHANGE, 33, 43);
    h.match.UseEvent(MidiEvent(MidiType::CONTROL_CHANGE, 1, 0));
    h.match.UseEvent(MidiEvent(MidiType::CONTROL_CHANGE, 33, 127 << 7));
    REQUIRE(h.samples.size() == 2);
    CHECK(h.samples[0].rail == 42);
    CHECK(h.samples[0].value == -1.0);
    CHECK(h.samples[1].rail == 43);
    CHECK(h.samples[1].value == 1.0);
}

TEST_CASE(
    "midi axis: disabled unknown malformed and mode messages never dispatch")
{
    MidiHarness h;
    h.Rail(MidiType::PITCH_BEND, 0, 42);
    h.Rail(MidiType::CONTROL_CHANGE, 120, 43);
    h.Rail(MidiType::CONTROL_CHANGE, 33, 44);
    h.match.UseEvent(MidiEvent(MidiType::CONTROL_CHANGE, 120));
    h.match.UseEvent(MidiEvent(MidiType::CONTROL_CHANGE, 33));
    h.match.UseEvent(MidiEvent(MidiType::PITCH_BEND, 0, 16384));
    auto event = MidiEvent(MidiType::PITCH_BEND, 0, 8192);
    event.ch   = 1;
    h.match.UseEvent(event);
    event.ch           = 0;
    event.highres_time = std::numeric_limits<uint64_t>::max();
    h.match.UseEvent(event);
    event.highres_time       = 0;
    h.init.lambdas.midi_axis = {};
    CHECK_NOTHROW(h.match.UseEvent(event));
    CHECK(h.samples.empty());
}

TEST_CASE("midi axis: pitch and pressure dispatch without consuming notes")
{
    MidiHarness h;
    h.Rail(MidiType::PITCH_BEND, 0, 42);
    h.Rail(MidiType::AFTERTOUCH, 0, 43);
    h.Rail(MidiType::POLY_PRESSURE, 60, 44);
    NOTE note;
    note.microsecond = 1000;
    h.init.note_objects->Fill<BUFFER_SUB>(note, 42);
    h.init.note_objects->Sort();
    h.match.UseEvent(MidiEvent(MidiType::PITCH_BEND, 0, 8192));
    h.match.UseEvent(MidiEvent(MidiType::AFTERTOUCH, 0, 127));
    h.match.UseEvent(MidiEvent(MidiType::POLY_PRESSURE, 60, 0));
    h.match.UseEvent(MidiEvent(MidiType::PITCH_BEND, 0, 8192));
    REQUIRE(h.samples.size() == 4);
    CHECK(h.samples[0].rail == 42);
    CHECK(h.samples[0].value == 0.0);
    CHECK(h.samples[1].rail == 43);
    CHECK(h.samples[1].value == 1.0);
    CHECK(h.samples[2].rail == 44);
    CHECK(h.samples[2].value == -1.0);
    CHECK(h.samples[3].rail == 42);
    P_NOTE_VEC notes;
    h.init.note_objects->Get<BUFFER_SUB>(2000, 42, notes);
    REQUIRE(notes.size() == 1);
    CHECK_FALSE(notes[0]->used);
    CHECK(h.pre.Event_Datas.use_queue.Get()->empty());
}

TEST_CASE(
    "midi axis: existing note on and off still use the normal judge queue")
{
    MidiHarness h;
    h.Rail(MidiType::NOTE_ON, 60, 42);
    h.Rail(MidiType::NOTE_OFF, 60, 42);
    NOTE note;
    note.microsecond = 1000;
    h.init.note_objects->Fill<BUFFER_MAIN>(note, 42);
    note.isDown = false;
    h.init.note_objects->Fill<BUFFER_SUB>(note, 42);
    h.init.note_objects->Sort();
    h.match.UseEvent(MidiEvent(MidiType::NOTE_ON, 60, 100));
    h.match.UseEvent(MidiEvent(MidiType::NOTE_OFF, 60, 0));
    CHECK(h.samples.empty());
    const auto events = h.pre.Event_Datas.use_queue.Get();
    REQUIRE(events->size() == 2);
    CHECK((*events)[0].railid == 42);
    CHECK((*events)[0].Pressed);
    CHECK((*events)[0].diff == 0);
    CHECK((*events)[1].railid == 42);
    CHECK_FALSE((*events)[1].Pressed);
    CHECK((*events)[1].diff == 0);
}
