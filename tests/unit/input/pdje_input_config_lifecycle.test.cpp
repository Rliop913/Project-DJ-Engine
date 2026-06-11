#include "PDJE_Input.hpp"

#include <doctest/doctest.h>

#include <cstddef>
#include <vector>

struct PDJE_Input_TestAccess {
    static void
    SetState(PDJE_Input &input, const PDJE_INPUT_STATE state)
    {
        input.state = state;
    }

    static void
    EmplaceMidiEngine(PDJE_Input &input)
    {
        input.midi_engine.emplace();
    }

    static bool
    HasMidiEngine(const PDJE_Input &input)
    {
        return input.midi_engine.has_value();
    }

    static bool
    MidiOn(const PDJE_Input &input)
    {
        return input.FLAG_MIDI_ON;
    }

    static std::size_t
    ConfiguredMidiCount(const PDJE_Input &input)
    {
        return input.midi_engine
                   ? input.midi_engine->configed_devices.size()
                   : 0U;
    }
};

namespace {

std::vector<libremidi::input_port>
MakeMidiDevices()
{
    libremidi::input_port port;
    port.port_name    = "unit-midi-port";
    port.display_name = "Unit MIDI Port";
    return { port };
}

} // namespace

TEST_CASE("input/config: invalid state does not commit MIDI config")
{
    PDJE_Input input;
    PDJE_Input_TestAccess::EmplaceMidiEngine(input);
    PDJE_Input_TestAccess::SetState(input, PDJE_INPUT_STATE::INPUT_LOOP_READY);

    std::vector<DeviceData> devices;
    auto                    midi_devices = MakeMidiDevices();

    CHECK_FALSE(input.Config(devices, midi_devices));
    CHECK(input.GetState() == PDJE_INPUT_STATE::INPUT_LOOP_READY);
    CHECK_FALSE(PDJE_Input_TestAccess::MidiOn(input));
    CHECK(PDJE_Input_TestAccess::ConfiguredMidiCount(input) == 0);

    const auto line = input.PullOutDataLine();
    CHECK(line.input_arena == nullptr);
    CHECK(line.midi_datas == nullptr);
}

TEST_CASE("input/config: dead state rejects MIDI config before touching MIDI engine")
{
    PDJE_Input input;

    std::vector<DeviceData> devices;
    auto                    midi_devices = MakeMidiDevices();

    CHECK_FALSE(input.Config(devices, midi_devices));
    CHECK(input.GetState() == PDJE_INPUT_STATE::DEAD);
    CHECK_FALSE(PDJE_Input_TestAccess::HasMidiEngine(input));
    CHECK_FALSE(PDJE_Input_TestAccess::MidiOn(input));

    const auto line = input.PullOutDataLine();
    CHECK(line.input_arena == nullptr);
    CHECK(line.midi_datas == nullptr);
}
