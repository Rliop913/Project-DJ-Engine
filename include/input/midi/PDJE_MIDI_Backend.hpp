#pragma once

#include <functional>
#include <libremidi/libremidi.hpp>
#include <memory>
#include <vector>

namespace PDJE_MIDI::detail {

// Internal transport seam. Decoding and event storage remain in MIDI.
class Connection {
  public:
    // Destruction must stop delivery and wait for any in-flight callback.
    virtual ~Connection() = default;
};

class Backend {
  public:
    using OnMessage = std::function<void(const libremidi::message &)>;

    virtual ~Backend() = default;
    virtual std::vector<libremidi::input_port>
    GetDevices() = 0;
    // Failure throws; a successful connection owns callback delivery.
    // Delivery is serialized per connection; different connections may overlap.
    virtual std::unique_ptr<Connection>
    Open(const libremidi::input_port &port, OnMessage on_message) = 0;
};

} // namespace PDJE_MIDI::detail
