#include "PDJE_Input.hpp"
#include "dummy_midi_backend.hpp"

#include <doctest/doctest.h>

#include <memory>
#include <string>
#include <vector>

struct PDJE_Input_TestAccess {
    static void
    SetMidiBackend(PDJE_Input                                 &input,
                   std::shared_ptr<PDJE_MIDI::detail::Backend> backend)
    {
        REQUIRE(input.GetState() == PDJE_INPUT_STATE::DEAD);
        input.midi_backend = std::move(backend);
    }

    static bool
    HasDefaultDevices(const PDJE_Input &input)
    {
        return input.default_devs.has_value();
    }

    static bool
    HasMidiEngine(const PDJE_Input &input)
    {
        return input.midi_engine.has_value();
    }
};

namespace {

struct MidiFixture {
    std::shared_ptr<DummyMidiBackend> backend =
        std::make_shared<DummyMidiBackend>();
    PDJE_Input              input;
    std::vector<DeviceData> devices;

    MidiFixture()
    {
        PDJE_Input_TestAccess::SetMidiBackend(input, backend);
    }

    void
    Configure()
    {
        REQUIRE(input.Init());
        REQUIRE(input.Config(devices, input.GetMIDIDevs()));
    }
};

void
CheckDead(PDJE_Input &input)
{
    CHECK(input.GetState() == PDJE_INPUT_STATE::DEAD);
    CHECK(input.PullOutDataLine().input_arena == nullptr);
    CHECK(input.PullOutDataLine().midi_datas == nullptr);
    CHECK_FALSE(PDJE_Input_TestAccess::HasDefaultDevices(input));
    CHECK_FALSE(PDJE_Input_TestAccess::HasMidiEngine(input));
}

} // namespace

TEST_CASE("input/midi-only: real lifecycle delivers dummy device messages")
{
    MidiFixture f;
    REQUIRE(f.input.Init());
    CHECK(f.input.GetState() == PDJE_INPUT_STATE::DEVICE_CONFIG_STATE);
    CHECK(f.input.GetCurrentInputBackend() == "none");
    CHECK_FALSE(PDJE_Input_TestAccess::HasDefaultDevices(f.input));
    CHECK(f.input.PullOutDataLine().midi_datas == nullptr);
    CHECK(f.backend->open_calls == 0);

    const auto ports = f.input.GetMIDIDevs();
    REQUIRE(ports.size() == 1);
    CHECK(ports[0].port_name == "dummy-midi");
    CHECK(f.backend->enumerate_calls == 1);
    REQUIRE(f.input.Config(f.devices, ports));
    CHECK(f.input.GetState() == PDJE_INPUT_STATE::INPUT_LOOP_READY);
    const auto line = f.input.PullOutDataLine();
    CHECK(line.input_arena == nullptr);
    REQUIRE(line.midi_datas != nullptr);
    CHECK(f.backend->open_calls == 0);
    REQUIRE(f.input.Run());
    CHECK(f.input.GetState() == PDJE_INPUT_STATE::INPUT_LOOP_RUNNING);
    CHECK(f.input.PullOutDataLine().midi_datas == line.midi_datas);
    CHECK(f.backend->active_connections == 1);
    CHECK_FALSE(PDJE_Input_TestAccess::HasDefaultDevices(f.input));

    f.backend->Emit("dummy-midi", { 0x90, 60, 100 });
    f.backend->Emit("dummy-midi", { 0x9F, 61, 0 });
    const auto &events = *line.midi_datas->Get();
    REQUIRE(events.size() == 2);
    CHECK(events[0].type ==
          static_cast<uint8_t>(libremidi::message_type::NOTE_ON));
    CHECK(events[0].ch == 0);
    CHECK(events[0].pos == 60);
    CHECK(events[0].value == 100);
    CHECK(std::string(events[0].port_name, events[0].port_name_len) ==
          "dummy-midi");
    CHECK(events[1].type ==
          static_cast<uint8_t>(libremidi::message_type::NOTE_OFF));
    CHECK(events[1].ch == 15);
    CHECK(events[1].pos == 61);
    CHECK(events[1].value == 0);
    CHECK(events[1].highres_time >= events[0].highres_time);

    REQUIRE(f.input.Kill());
    CheckDead(f.input);
    CHECK(f.backend->active_connections == 0);
    CHECK(f.backend->closed_connections == 1);
    // No stale callback is invoked after teardown. Do not dereference old line.
    f.backend->Emit("dummy-midi", { 0x90, 60, 100 });
    CHECK(f.input.Kill());
    CHECK(f.backend->closed_connections == 1);
}

TEST_CASE("input/midi-only: Config selects CC pairing without starting capture")
{
    bool paired = true;
    SUBCASE("paired CC")
    {
        paired = true;
    }
    SUBCASE("independent seven-bit CC")
    {
        paired = false;
    }

    MidiFixture f;
    REQUIRE(f.input.Init());
    const auto ports = f.input.GetMIDIDevs();
    REQUIRE(f.input.Config(f.devices, ports, paired));
    CHECK(f.input.GetState() == PDJE_INPUT_STATE::INPUT_LOOP_READY);
    CHECK(f.backend->open_calls == 0);
    // Invalid reconfiguration must not overwrite the selected decoder mode.
    CHECK_FALSE(f.input.Config(f.devices, ports, !paired));
    REQUIRE(f.input.Run());
    CHECK_FALSE(f.input.Config(f.devices, ports, !paired));
    f.backend->Emit("dummy-midi", { 0xB0, 1, 2 });
    f.backend->Emit("dummy-midi", { 0xB0, 33, 3 });
    {
        const auto &events = *f.input.PullOutDataLine().midi_datas->Get();
        REQUIRE(events.size() == 2);
        CHECK(events[0].value == 256);
        CHECK(events[1].pos == 33);
        CHECK(events[1].value == (paired ? 259 : 384));
    }

    REQUIRE(f.input.Kill());
    REQUIRE(f.input.Init());
    // Legacy Config must restore the original paired default on a new session.
    REQUIRE(f.input.Config(f.devices, ports));
    REQUIRE(f.input.Run());
    f.backend->Emit("dummy-midi", { 0xB0, 33, 3 });
    const auto &events = *f.input.PullOutDataLine().midi_datas->Get();
    REQUIRE(events.size() == 1);
    CHECK(events[0].value == 3);
}

TEST_CASE("input/midi-only: illegal operations preserve the current lifecycle")
{
    MidiFixture f;
    CHECK_FALSE(f.input.Config(f.devices, f.backend->ports));
    CHECK_FALSE(f.input.Run());
    CHECK(f.input.GetDevs().empty());
    CHECK(f.input.GetMIDIDevs().empty());
    CheckDead(f.input);

    REQUIRE(f.input.Init());
    CHECK_FALSE(f.input.Init());
    CHECK_FALSE(f.input.Run());
    CHECK(f.input.GetState() == PDJE_INPUT_STATE::DEVICE_CONFIG_STATE);
    REQUIRE(f.input.Config(f.devices, f.backend->ports));
    const auto line = f.input.PullOutDataLine();
    CHECK_FALSE(f.input.Config(f.devices, {}));
    CHECK_FALSE(f.input.Init());
    CHECK(f.input.GetState() == PDJE_INPUT_STATE::INPUT_LOOP_READY);
    CHECK(f.input.PullOutDataLine().midi_datas == line.midi_datas);
    REQUIRE(f.input.Run());
    CHECK_FALSE(f.input.Config(f.devices, {}));
    CHECK_FALSE(f.input.Init());
    CHECK_FALSE(f.input.Run());
    CHECK(f.input.GetState() == PDJE_INPUT_STATE::INPUT_LOOP_RUNNING);
    CHECK(f.input.PullOutDataLine().midi_datas == line.midi_datas);
    CHECK(f.backend->open_calls == 1);
}

TEST_CASE(
    "input/midi-only: empty config is retryable and invalid input is filtered")
{
    MidiFixture f;
    REQUIRE(f.input.Init());
    f.devices = { { PDJE_Dev_Type::UNKNOWN, "invalid", "invalid" },
                  { PDJE_Dev_Type::KEYBOARD, "", "invalid" } };
    CHECK_FALSE(f.input.Config(f.devices, {}));
    CHECK(f.input.GetState() == PDJE_INPUT_STATE::DEVICE_CONFIG_STATE);
    CHECK(f.input.PullOutDataLine().midi_datas == nullptr);
    REQUIRE(f.input.Config(f.devices, f.backend->ports));
    CHECK(f.devices.size() == 2); // caller's selection is not mutated
    CHECK(f.input.GetState() == PDJE_INPUT_STATE::INPUT_LOOP_READY);
    CHECK_FALSE(PDJE_Input_TestAccess::HasDefaultDevices(f.input));
    REQUIRE(f.input.Run());
}

TEST_CASE(
    "input/midi-only: Kill works before Run and destructor closes connections")
{
    MidiFixture f;
    SUBCASE("initialized only")
    {
        REQUIRE(f.input.Init());
    }
    SUBCASE("ready")
    {
        f.Configure();
    }
    REQUIRE(f.input.Kill());
    CheckDead(f.input);
    CHECK(f.backend->open_calls == 0);

    auto backend = std::make_shared<DummyMidiBackend>();
    {
        PDJE_Input input;
        PDJE_Input_TestAccess::SetMidiBackend(input, backend);
        REQUIRE(input.Init());
        REQUIRE(input.Config(f.devices, backend->ports));
        REQUIRE(input.Run());
        REQUIRE(backend->active_connections == 1);
    }
    CHECK(backend->active_connections == 0);
    CHECK(backend->closed_connections == 1);
}

TEST_CASE("input/midi-only: failed opens roll back all ports and allow "
          "reinitialization")
{
    MidiFixture f;
    f.backend->ports.push_back(f.backend->ports.front());
    f.backend->ports.back().port_name = "second-midi";
    SUBCASE("first port fails")
    {
        f.backend->fail_open_call = 1;
    }
    SUBCASE("second port fails after first opens")
    {
        f.backend->fail_open_call = 2;
    }
    f.Configure();
    CHECK_FALSE(f.input.Run());
    CheckDead(f.input);
    CHECK(f.backend->active_connections == 0);
    CHECK(f.backend->closed_connections == f.backend->fail_open_call - 1);
    CHECK_FALSE(f.input.Run());
    CHECK_FALSE(f.input.Config(f.devices, f.backend->ports));
    CHECK(f.input.Kill());

    f.backend->fail_open_call = 0;
    f.Configure();
    REQUIRE(f.input.Run());
    CHECK(f.backend->active_connections == 2);
    f.backend->Emit("second-midi", { 0x80, 60, 20 });
    const auto &events = *f.input.PullOutDataLine().midi_datas->Get();
    REQUIRE(events.size() == 1);
    CHECK(std::string(events[0].port_name, events[0].port_name_len) ==
          "second-midi");
}

TEST_CASE("input/midi-only: restarts have no stale events or controller state")
{
    MidiFixture f;
    f.Configure();
    REQUIRE(f.input.Run());
    f.backend->Emit("dummy-midi", { 0xB0, 1, 64 });
    f.backend->Emit("dummy-midi", { 0x90, 60, 100 });
    REQUIRE(f.input.Kill());
    f.Configure();
    REQUIRE(f.input.Run());
    auto line = f.input.PullOutDataLine();
    CHECK(line.midi_datas->Get()->empty());
    f.backend->Emit("dummy-midi", { 0xB0, 33, 1 });
    const auto &events = *line.midi_datas->Get();
    REQUIRE(events.size() == 1);
    CHECK(events[0].value == 1);
    CHECK(f.backend->active_connections == 1);
}

TEST_CASE(
    "input/midi-only: dummy backends and event buffers are instance-local")
{
    MidiFixture first;
    MidiFixture second;
    first.Configure();
    second.Configure();
    REQUIRE(first.input.Run());
    REQUIRE(second.input.Run());
    first.backend->Emit("dummy-midi", { 0x90, 60, 100 });
    CHECK(first.input.PullOutDataLine().midi_datas->Get()->size() == 1);
    CHECK(second.input.PullOutDataLine().midi_datas->Get()->empty());
    REQUIRE(first.input.Kill());
    second.backend->Emit("dummy-midi", { 0x90, 61, 90 });
    CHECK(second.input.PullOutDataLine().midi_datas->Get()->size() == 1);
    CHECK(second.backend->active_connections == 1);
}

TEST_CASE(
    "input/midi-only: malformed messages are ignored by the production decoder")
{
    MidiFixture f;
    f.Configure();
    REQUIRE(f.input.Run());
    f.backend->Emit("dummy-midi", {});
    f.backend->Emit("dummy-midi", { 0x90, 60 });
    f.backend->Emit("dummy-midi", { 0x80 });
    f.backend->Emit("dummy-midi", { 0xB0, 1 });
    f.backend->Emit("dummy-midi", { 0xE0, 1 });
    f.backend->Emit("dummy-midi", { 0xD0 });
    f.backend->Emit("dummy-midi", { 0xA0, 60 });
    f.backend->Emit("dummy-midi", { 0xF8 });
    f.backend->Emit("dummy-midi", { 0x90, 60, 100, 1 });
    f.backend->Emit("dummy-midi", { 0x90, 0x80, 100 });
    f.backend->Emit("dummy-midi", { 0xB0, 1, 0xFF });
    f.backend->Emit("dummy-midi", { 0xD0, 0xFF });
    f.backend->Emit("dummy-midi", { 0x10, 60, 100 });
    CHECK(f.input.PullOutDataLine().midi_datas->Get()->empty());
    f.backend->Emit("dummy-midi", { 0xE0, 0, 64 });
    const auto &events = *f.input.PullOutDataLine().midi_datas->Get();
    REQUIRE(events.size() == 1);
    CHECK(events[0].value == 8192);
}
