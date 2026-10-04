#include "PDJE_MIDI.hpp"
#include "dummy_midi_backend.hpp"
#include <doctest/doctest.h>

#include <exception>
#include <latch>
#include <memory>
#include <thread>
#include <vector>

namespace {

// Deliberately retain independent callback copies beyond connection lifetime.
// This tests PDJE state ownership, not a driver's callback-quiescence contract.
class RetainingBackend final : public PDJE_MIDI::detail::Backend {
  public:
    DummyMidiBackend       device;
    std::vector<OnMessage> retained;

    std::vector<libremidi::input_port>
    GetDevices() override
    {
        return device.GetDevices();
    }

    std::unique_ptr<PDJE_MIDI::detail::Connection>
    Open(const libremidi::input_port &port, OnMessage callback) override
    {
        retained.push_back(callback);
        return device.Open(port, std::move(callback));
    }
};

struct ReleaseOnExit {
    std::latch &gate;
    ~ReleaseOnExit()
    {
        gate.count_down();
    }
};

libremidi::message
NoteOn()
{
    libremidi::message message;
    message.bytes = { 0x90, 60, 100 };
    return message;
}

} // namespace

TEST_CASE(
    "input/midi-shared: callback on another thread outlives its MIDI owner")
{
    auto  backend = std::make_shared<RetainingBackend>();
    auto  midi    = std::make_unique<PDJE_MIDI::MIDI>(4, backend);
    auto *buffer  = &midi->GetEventBuffer();
    midi->Config(backend->device.ports.front());
    midi->Run();
    REQUIRE(backend->retained.size() == 1);
    CHECK(&midi->GetEventBuffer() == buffer);
    auto callback = std::move(backend->retained.front());
    backend->retained.clear();

    std::latch         entered(1), resume(1), written(1), release(1);
    std::exception_ptr error;
    std::vector<PDJE_MIDI::MIDI_EV> received;
    std::jthread                    worker([callback = std::move(callback),
                                            &entered,
                                            &resume,
                                            &written,
                                            &release,
                                            &error] {
        entered.count_down();
        resume.wait();
        try {
            callback(NoteOn());
        } catch (...) {
            error = std::current_exception();
        }
        written.count_down();
        // Keep the captured state alive until the test observer has read it.
        release.wait();
    });
    entered.wait();
    midi.reset();
    resume.count_down();
    {
        ReleaseOnExit finish{ release };
        written.wait();
        // Test-only observation while the worker owns the retained callback.
        // Normal data-line users must still stop consuming before MIDI
        // teardown.
        received = *buffer->Get();
    }
    worker.join();
    CHECK_FALSE(error);
    CHECK(backend->device.active_connections == 0);
    CHECK(backend->device.closed_connections == 1);
    REQUIRE(received.size() == 1);
    CHECK(received[0].pos == 60);
    CHECK(received[0].value == 100);
    CHECK(received[0].ch == 0);
}

TEST_CASE("input/midi-shared: failed startup callback cannot target a "
          "replacement engine")
{
    auto backend = std::make_shared<RetainingBackend>();
    backend->device.ports.push_back(backend->device.ports.front());
    backend->device.ports.back().port_name = "second-midi";
    backend->device.fail_open_call         = 2;
    auto  midi             = std::make_unique<PDJE_MIDI::MIDI>(4, backend);
    auto *old_buffer       = &midi->GetEventBuffer();
    midi->configed_devices = backend->device.ports;
    CHECK_THROWS_AS(midi->Run(), std::runtime_error);
    REQUIRE(backend->retained.size() == 2);
    CHECK(backend->device.active_connections == 0);
    midi.reset();

    PDJE_MIDI::MIDI replacement(4, backend);
    for (const auto &callback : backend->retained) {
        callback(NoteOn());
    }
    // Retained callbacks keep only the old state alive, never the replacement.
    const auto *events = old_buffer->Get();
    REQUIRE(events->size() == 2);
    CHECK((*events)[0].value == 100);
    CHECK((*events)[1].value == 100);
    CHECK(replacement.GetEventBuffer().Get()->empty());
    backend->retained.clear();
    // old_buffer is no longer valid here.
}
