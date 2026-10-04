#pragma once

#include "PDJE_Buffer.hpp"
#include "PDJE_Highres_Clock.hpp"
#include "PDJE_LOG_SETTER.hpp"
#include "PDJE_MIDI_Backend.hpp"
#include <libremidi/libremidi.hpp>
#include <memory>

#define PDJE_BIT_PARSE_7(N) (N & 0x7F)
#define B_GUARD(B, N)                                                          \
    if (B.size() < N)                                                          \
        return;
namespace PDJE_MIDI {
struct PDJE_API MIDI_EV {
    uint8_t  type;
    uint8_t  ch;
    uint8_t  pos;
    uint16_t value;
    uint64_t highres_time;
    char     port_name[256];
    uint8_t  port_name_len = 0;
};

// Owned by MIDI and every registered receive callback; no back-reference to
// MIDI.
struct MIDI_shared {
    Spinlock_Double_Buffer<MIDI_EV> evlog;

    explicit MIDI_shared(const int buffer_size) : evlog(buffer_size)
    {
    }
};

class MIDI {
  private:
    std::shared_ptr<detail::Backend>                 backend;
    std::shared_ptr<MIDI_shared>                     shared_data;
    std::vector<std::unique_ptr<detail::Connection>> midiin;

  public:
    // Borrowed buffer: callers must still stop consumers before destroying
    // MIDI.
    Spinlock_Double_Buffer<MIDI_EV> &
    GetEventBuffer() noexcept
    {
        return shared_data->evlog;
    }
    std::vector<libremidi::input_port> configed_devices;
    void
    Run(const bool CC_LSB_ON = true);

    void
    Config(const libremidi::input_port &midi_dev)
    {
        configed_devices.push_back(midi_dev);
    }

    std::vector<libremidi::input_port>
    GetDevices()
    {
        return backend->GetDevices();
    }
    MIDI(const int buffer_size = 64);
    MIDI(int buffer_size, std::shared_ptr<detail::Backend> transport);
    MIDI(const MIDI &) = delete;
    MIDI &
    operator=(const MIDI &) = delete;
    ~MIDI();
};
}; // namespace PDJE_MIDI
