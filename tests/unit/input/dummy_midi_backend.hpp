#pragma once

#include "PDJE_MIDI_Backend.hpp"

#include <initializer_list>
#include <stdexcept>
#include <string>
#include <utility>

// Synchronous, instance-local device transport: no ports, threads, or sleeps.
// It delivers raw messages into MIDI's production callback, not a test decoder.
class DummyMidiBackend final : public PDJE_MIDI::detail::Backend {
    struct Slot {
        std::string port_name;
        OnMessage   on_message;
    };

    class Connection final : public PDJE_MIDI::detail::Connection {
        DummyMidiBackend     &owner;
        std::shared_ptr<Slot> slot;

      public:
        Connection(DummyMidiBackend &owner_, std::shared_ptr<Slot> slot_)
            : owner(owner_), slot(std::move(slot_))
        {
            ++owner.active_connections;
        }
        ~Connection() override
        {
            slot->on_message = {};
            --owner.active_connections;
            ++owner.closed_connections;
        }
    };

    std::vector<std::weak_ptr<Slot>> slots;

  public:
    std::vector<libremidi::input_port> ports;
    int                                enumerate_calls    = 0;
    int                                open_calls         = 0;
    int                                active_connections = 0;
    int                                closed_connections = 0;
    int                                fail_open_call     = 0;

    explicit DummyMidiBackend(std::vector<std::string> names = { "dummy-midi" })
    {
        for (const auto &name : names) {
            libremidi::input_port port;
            port.port_name    = name;
            port.display_name = name;
            ports.push_back(std::move(port));
        }
    }

    std::vector<libremidi::input_port>
    GetDevices() override
    {
        ++enumerate_calls;
        return ports;
    }

    std::unique_ptr<PDJE_MIDI::detail::Connection>
    Open(const libremidi::input_port &port, OnMessage on_message) override
    {
        if (++open_calls == fail_open_call) {
            throw std::runtime_error("dummy MIDI open failure");
        }
        auto slot = std::make_shared<Slot>(
            Slot{ port.port_name, std::move(on_message) });
        auto connection = std::make_unique<Connection>(*this, slot);
        slots.push_back(slot);
        return connection;
    }

    void
    Emit(const std::string                   &port_name,
         std::initializer_list<unsigned char> bytes)
    {
        libremidi::message message;
        message.bytes.assign(bytes.begin(), bytes.end());
        for (const auto &weak : slots) {
            if (const auto slot = weak.lock();
                slot && slot->port_name == port_name && slot->on_message) {
                slot->on_message(message);
            }
        }
    }
};
