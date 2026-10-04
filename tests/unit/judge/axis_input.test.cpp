#include "AxisModel/MidiAxis.hpp"
#include "AxisModel/MouseAxis.hpp"
#include "PDJE_Match.hpp"
#include <doctest/doctest.h>

#include <cstring>
#include <limits>

using namespace PDJE_JUDGE;

TEST_CASE("axis input: mouse absolute relative and ignored coordinates")
{
    MOUSE_AXIS_MODEL mouse(AXIS_MODEL(-100.0, 0.0, 100.0),
                           AXIS_MODEL(0.0, 100.0, 200.0));
    auto             value = mouse.Update(50, 150, ABS);
    REQUIRE(value.has_value());
    CHECK((*value)[0] == 0.5);
    CHECK((*value)[1] == 0.5);

    value = mouse.Update(-25, -50, REL);
    REQUIRE(value.has_value());
    CHECK((*value)[0] == 0.25);
    CHECK((*value)[1] == 0.0);
    CHECK_FALSE(mouse.Update(100, 200, PDJE_AXIS_IGNORE).has_value());
    value = mouse.Update(0, 0, REL);
    REQUIRE(value.has_value());
    CHECK((*value)[0] == 0.25);
    CHECK((*value)[1] == 0.0);

    value = mouse.Update(0, 0, ABS);
    REQUIRE(value.has_value());
    CHECK((*value)[0] == 0.0);
    CHECK((*value)[1] == -1.0);
    value = mouse.Update(-100, 200, VIRTUAL_DESKTOP_ABS);
    REQUIRE(value.has_value());
    CHECK((*value)[0] == -1.0);
    CHECK((*value)[1] == 1.0);

    mouse.Reset();
    PDJE_Mouse_Event event{};
    event.axis_type = REL;
    value           = mouse.Update(event);
    REQUIRE(value.has_value());
    CHECK((*value)[0] == 0.0);
    CHECK((*value)[1] == 0.0);
    value = mouse.Update(
        std::numeric_limits<int>::max(), std::numeric_limits<int>::min(), REL);
    REQUIRE(value.has_value());
    CHECK((*value)[0] == 1.0);
    CHECK((*value)[1] == -1.0);
}

TEST_CASE("axis input: MIDI CC uses decoded resolution rather than wire range")
{
    PDJE_MIDI::MIDI_EV event{};
    event.type = static_cast<uint8_t>(libremidi::message_type::CONTROL_CHANGE);
    for (uint8_t controller : { 0, 31, 32, 63, 64, 119 }) {
        event.pos = controller;
        for (bool high_resolution : { false, true }) {
            const bool paired  = high_resolution && controller < 64;
            const int  maximum = paired ? 16383 : 16256;
            event.value        = 0;
            CHECK(NormalizeMidiAxis(event, high_resolution) == -1.0);
            event.value = static_cast<uint16_t>(maximum);
            CHECK(NormalizeMidiAxis(event, high_resolution) == 1.0);
            event.value      = 8192;
            const auto value = NormalizeMidiAxis(event, high_resolution);
            REQUIRE(value.has_value());
            CHECK(*value == doctest::Approx(2.0 * 8192.0 / maximum - 1.0));
        }
    }
    event.pos   = 1;
    event.value = 16383;
    CHECK(NormalizeMidiAxis(event) == 1.0);
    event.value = std::numeric_limits<uint16_t>::max();
    CHECK_FALSE(NormalizeMidiAxis(event).has_value());
}

TEST_CASE("axis input: MIDI pitch bend has an exact neutral position")
{
    PDJE_MIDI::MIDI_EV event{};
    event.type  = static_cast<uint8_t>(libremidi::message_type::PITCH_BEND);
    event.value = 0;
    CHECK(NormalizeMidiAxis(event) == -1.0);
    event.value = 8192;
    CHECK(NormalizeMidiAxis(event) == 0.0);
    event.value = 16383;
    CHECK(NormalizeMidiAxis(event) == 1.0);
}

TEST_CASE("axis input: MIDI pressure is seven bit and notes are not axes")
{
    PDJE_MIDI::MIDI_EV event{};
    for (auto type : { libremidi::message_type::AFTERTOUCH,
                       libremidi::message_type::POLY_PRESSURE }) {
        event.type  = static_cast<uint8_t>(type);
        event.value = 0;
        CHECK(NormalizeMidiAxis(event) == -1.0);
        event.value = 127;
        CHECK(NormalizeMidiAxis(event) == 1.0);
    }
    for (auto type : { libremidi::message_type::NOTE_ON,
                       libremidi::message_type::NOTE_OFF }) {
        event.type = static_cast<uint8_t>(type);
        CHECK_FALSE(NormalizeMidiAxis(event).has_value());
    }
    event.type = 0;
    CHECK_FALSE(NormalizeMidiAxis(event).has_value());
}

TEST_CASE(
    "axis input: judge mouse callback can attach the model without ABI changes")
{
    Judge_Init init;
    init.note_objects.emplace();
    RAIL_KEY::KB_MOUSE key;
    key.Device_Name = "mouse";
    key.DeviceKey   = DEVICE_MOUSE_EVENT::PDJE_AXIS_MOVE;
    init.raildb.Add(key, PDJE_Dev_Type::MOUSE, 42, 0);
    PreProcess pre(&init);
    pre.use_range = 1000;
    Match match(&pre, &init);

    MOUSE_AXIS_MODEL mouse(AXIS_MODEL(0.0, 50.0, 100.0),
                           AXIS_MODEL(0.0, 50.0, 100.0));

    int calls = 0;

    std::optional<std::array<double, 2>> value;
    init.lambdas.custom_mouse_parse = [&](LOCAL_TIME           time,
                                          const P_NOTE_VEC    &notes,
                                          uint64_t             rail,
                                          int                  x,
                                          int                  y,
                                          PDJE_Mouse_Axis_Type type) {
        ++calls;
        CHECK(time == 123);
        CHECK(rail == 42);
        CHECK(notes.empty());
        value = mouse.Update(x, y, type);
    };

    PDJE_Input_Log log{};
    log.type = PDJE_Dev_Type::MOUSE;
    std::memcpy(log.name, "mouse", 5);
    log.name_len              = 5;
    log.microSecond           = 123;
    log.event.mouse           = {};
    log.event.mouse.axis_type = ABS;
    match.UseEvent<PDJE_Dev_Type::MOUSE>(log);
    CHECK(calls == 1);
    REQUIRE(value.has_value());
    CHECK((*value)[0] == -1.0);
    CHECK((*value)[1] == -1.0);

    log.event.mouse.axis_type = VIRTUAL_DESKTOP_ABS;
    match.UseEvent<PDJE_Dev_Type::MOUSE>(log);
    CHECK(calls == 2);
    log.event.mouse.axis_type = REL;
    match.UseEvent<PDJE_Dev_Type::MOUSE>(log);
    CHECK(calls == 2); // Zero relative motion remains filtered.
    log.event.mouse.x = 25;
    match.UseEvent<PDJE_Dev_Type::MOUSE>(log);
    CHECK(calls == 3);
    REQUIRE(value.has_value());
    CHECK((*value)[0] == -0.5);
    CHECK((*value)[1] == -1.0);

    log.event.mouse.axis_type = PDJE_AXIS_IGNORE;
    match.UseEvent<PDJE_Dev_Type::MOUSE>(log);
    CHECK(calls == 3);
    log.event.mouse.axis_type = ABS;
    log.name[0] = 'x'; // Unregistered devices must not reach the callback.
    match.UseEvent<PDJE_Dev_Type::MOUSE>(log);
    CHECK(calls == 3);
}
