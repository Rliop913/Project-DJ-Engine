#include "PDJE_MIDI.hpp"
#include <algorithm>
#include <array>
#include <cstring>
#include <optional>
#include <stdexcept>
#include <utility>

namespace PDJE_MIDI {

namespace {

class LibreMidiConnection final : public detail::Connection {
    libremidi::midi_in input;

  public:
    LibreMidiConnection(const libremidi::input_port &port,
                        detail::Backend::OnMessage   on_message)
        : input(libremidi::input_configuration{
              .on_message = std::move(on_message),
              .timestamps = libremidi::NoTimestamp })
    {
        const auto err = input.open_port(port);
        if (err != stdx::error{}) {
            throw std::runtime_error(
                std::string(err.message().data(), err.message().size()));
        }
    }
};

class LibreMidiBackend final : public detail::Backend {
    std::optional<libremidi::observer> observer;

  public:
    std::vector<libremidi::input_port>
    GetDevices() override
    {
        if (!observer) {
            observer.emplace();
        }
        return observer->get_input_ports();
    }

    std::unique_ptr<detail::Connection>
    Open(const libremidi::input_port &port, OnMessage on_message) override
    {
        return std::make_unique<LibreMidiConnection>(port,
                                                     std::move(on_message));
    }
};

} // namespace

MIDI::MIDI(const int buffer_size) : MIDI(buffer_size, nullptr)
{
}

MIDI::MIDI(const int buffer_size, std::shared_ptr<detail::Backend> transport)
    : backend(transport ? std::move(transport)
                        : std::make_shared<LibreMidiBackend>()),
      shared_data(std::make_shared<MIDI_shared>(buffer_size))
{
}

MIDI::~MIDI()
{
    // Stop transport delivery first. Retained callbacks own their buffer state.
    midiin.clear();
}

void
MIDI::Run(const bool CC_LSB_ON)
{
    if (!midiin.empty()) {
        throw std::logic_error("MIDI is already running");
    }
    midiin.reserve(configed_devices.size());
    try {
        for (const auto &i : configed_devices) {
            std::string pname = i.port_name;

            auto on_message = [shared = shared_data,
                               CC_LSB_ON,
                               pname,
                               clock = PDJE_HIGHRES_CLOCK::CLOCK{},
                               CC_stat =
                                   std::array<std::array<uint16_t, 32>, 16>{}](
                                  const libremidi::message &m) mutable {
                try {
                    const auto &b = m.bytes;
                    if (b.empty()) {
                        return;
                    }
                    auto       type = m.get_message_type();
                    const auto byte_count =
                        type == libremidi::message_type::AFTERTOUCH ? 2U : 3U;
                    // Channel messages have a fixed length and seven-bit data.
                    // Reject malformed input instead of masking it into a
                    // value.
                    if (b.size() != byte_count || (b[0] & 0x80) == 0 ||
                        std::any_of(b.begin() + 1, b.end(), [](uint8_t byte) {
                            return (byte & 0x80) != 0;
                        })) {
                        return;
                    }
                    // libremidi::get_channel() is 1-based; the data line is
                    // 0-based.
                    const auto ch    = static_cast<uint8_t>(b[0] & 0x0F);
                    uint8_t    pos   = 0;
                    uint16_t   value = 0;

                    switch (type) {
                    case libremidi::message_type::NOTE_ON:
                    case libremidi::message_type::NOTE_OFF:
                        pos   = b[1];
                        value = b[2];
                        if (type == libremidi::message_type::NOTE_ON &&
                            value == 0) {
                            type = libremidi::message_type::NOTE_OFF;
                        }
                        break;
                    case libremidi::message_type::CONTROL_CHANGE:
                        pos   = b[1];
                        value = b[2];
                        if (CC_LSB_ON && pos < 64) {
                            auto &pair = CC_stat[ch][pos % 32];
                            if (pos < 32) {
                                pair = (pair & 0x007F) | (value << 7);
                            } else {
                                pair = (pair & 0x3F80) | value;
                            }
                            value = pair;
                        } else {
                            value <<= 7;
                        }
                        break;
                    case libremidi::message_type::PITCH_BEND:
                        value = static_cast<uint16_t>(b[1]) |
                                (static_cast<uint16_t>(b[2]) << 7);
                        break;
                    case libremidi::message_type::AFTERTOUCH:
                        value = b[1];
                        break;
                    case libremidi::message_type::POLY_PRESSURE:
                        pos   = b[1];
                        value = b[2];
                        break;
                    default:
                        return;
                    }
                    MIDI_EV    evres{ .type  = static_cast<uint8_t>(type),
                                      .ch    = ch,
                                      .pos   = pos,
                                      .value = value,
                                      .highres_time = clock.Get_MicroSecond() };
                    const auto port_name_len = std::min<std::size_t>(
                        pname.size(), sizeof(evres.port_name) - 1);
                    evres.port_name_len = static_cast<uint8_t>(port_name_len);
                    std::memcpy(evres.port_name,
                                pname.data(),
                                sizeof(char) * port_name_len);

                    shared->evlog.Write(evres);
                } catch (const std::exception &e) {
                    critlog("runtime error on midi input loop. What: ");
                    critlog(e.what());
                    return;
                }
            };
            auto connection = backend->Open(i, std::move(on_message));
            if (!connection) {
                throw std::runtime_error("MIDI backend returned no connection");
            }
            midiin.push_back(std::move(connection));
        }
    } catch (...) {
        midiin.clear();
        throw;
    }
}

} // namespace PDJE_MIDI
