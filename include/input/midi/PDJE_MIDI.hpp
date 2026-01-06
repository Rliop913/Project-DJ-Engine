#pragma once

#include "PDJE_Buffer.hpp"
#include "PDJE_Highres_Clock.hpp"
#include "PDJE_LOG_SETTER.hpp"
#include <libremidi/libremidi.hpp>

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
    char     port_name[256]; // todo - fill here
    uint8_t  port_name_len = 0;
};

class MIDI {
  private:
    libremidi::observer               obs;
    PDJE_HIGHRES_CLOCK::CLOCK         clock;
    std::optional<libremidi::midi_in> midiin;

    uint16_t CC_stat[16][32] = {
        0,
    };

  public:
    Atomic_Double_Buffer<MIDI_EV>      evlog;
    std::vector<libremidi::input_port> configed_devices;
    void
    Run()
    {
        for (const auto &i : configed_devices) {
            auto err = midiin->open_port(i);
            if (err != stdx::error{}) {
                throw std::runtime_error(
                    std::string(err.message().data(), err.message().size()));
            }
        }
    }

    void
    Config(const libremidi::input_port &midi_dev)
    {
        configed_devices.push_back(midi_dev);
    }

    std::vector<libremidi::input_port>
    GetDevices()
    {
        return obs.get_input_ports();
    }
    MIDI(const bool CC_LSB_ON = true, const int buffer_size = 64);
    ~MIDI() = default;
};
}; // namespace PDJE_MIDI