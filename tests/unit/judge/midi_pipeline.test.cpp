#include "../input/dummy_midi_backend.hpp"
#include "AxisModel/MidiAxis.hpp"
#include "PDJE_Match.hpp"
#include <doctest/doctest.h>

using namespace PDJE_JUDGE;

TEST_CASE("midi pipeline: dummy wire CC reaches the real judge axis callback")
{
    auto            backend = std::make_shared<DummyMidiBackend>();
    PDJE_MIDI::MIDI midi(64, backend);
    midi.Config(backend->ports.front());
    midi.Run();

    Judge_Init init;
    PreProcess pre(&init);
    Match      match(&pre, &init);
    init.SetRail("dummy-midi",
                 42,
                 static_cast<uint8_t>(libremidi::message_type::CONTROL_CHANGE),
                 0,
                 1);
    std::vector<double> values;
    init.lambdas.midi_axis = [&](LOCAL_TIME, uint64_t rail, double value) {
        CHECK(rail == 42);
        values.push_back(value);
    };

    backend->Emit("dummy-midi", { 0xB0, 1, 0 });
    backend->Emit("dummy-midi", { 0xB0, 1, 127 });
    backend->Emit("dummy-midi", { 0xB0, 33, 127 });
    backend->Emit("dummy-midi",
                  { 0xB0, 33, 128 }); // Invalid data, not an endpoint.
    const auto events = midi.GetEventBuffer().Get();
    REQUIRE(events->size() == 3);
    CHECK((*events)[0].ch == 0);
    CHECK((*events)[1].value == 16256);
    CHECK((*events)[2].value == 16383);
    CHECK((*events)[2].pos == 33);
    for (const auto &event : *events) {
        match.UseEvent(event);
    }
    REQUIRE(values.size() == 3);
    CHECK(values[0] == -1.0);
    CHECK(values[1] == doctest::Approx(2.0 * 16256.0 / 16383.0 - 1.0));
    CHECK(values[2] == 1.0);
}

TEST_CASE("midi pipeline: CC state is isolated by channel and connection")
{
    auto backend = std::make_shared<DummyMidiBackend>(
        std::vector<std::string>{ "first", "second" });
    PDJE_MIDI::MIDI midi(64, backend);
    midi.configed_devices = backend->ports;
    midi.Run();
    backend->Emit("first", { 0xB0, 1, 64 });
    backend->Emit("first", { 0xBF, 33, 127 });
    backend->Emit("second", { 0xB0, 33, 127 });
    backend->Emit("first", { 0xB0, 33, 1 });
    const auto events = midi.GetEventBuffer().Get();
    REQUIRE(events->size() == 4);
    CHECK((*events)[0].value == 8192);
    CHECK((*events)[1].ch == 15);
    CHECK((*events)[1].value == 127);
    CHECK((*events)[2].value == 127);
    CHECK((*events)[3].value == 8193);
    CHECK_THROWS_AS(midi.Run(), std::logic_error);
    CHECK(backend->open_calls == 2);
}

TEST_CASE(
    "midi pipeline: unpaired CC pitch and pressure use their decoded ranges")
{
    auto            backend = std::make_shared<DummyMidiBackend>();
    PDJE_MIDI::MIDI midi(64, backend);
    midi.Config(backend->ports.front());
    midi.Run(false);
    backend->Emit("dummy-midi", { 0xB0, 1, 127 });
    backend->Emit("dummy-midi", { 0xB0, 33, 0 });
    backend->Emit("dummy-midi", { 0xE0, 0, 64 });
    backend->Emit("dummy-midi", { 0xD0, 127 });
    backend->Emit("dummy-midi", { 0xA0, 60, 0 });
    const auto events = midi.GetEventBuffer().Get();
    REQUIRE(events->size() == 5);
    const double expected[] = { 1.0, -1.0, 0.0, 1.0, -1.0 };
    for (std::size_t i = 0; i < events->size(); ++i) {
        CHECK(NormalizeMidiAxis((*events)[i], false) == expected[i]);
    }
    const auto cc       = ParseMidiAxis((*events)[1], false);
    const auto pressure = ParseMidiAxis((*events)[4], false);
    REQUIRE(cc.has_value());
    REQUIRE(pressure.has_value());
    CHECK(cc->pos == 33);
    CHECK(pressure->pos == 60);
}
