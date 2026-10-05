#pragma once

#include "Input_State.hpp"

#include "PDJE_EXPORT_SETTER.hpp"
#include "PDJE_Input_DataLine.hpp"
#include "PDJE_Input_Device_Data.hpp"
#include "PDJE_MIDI.hpp"
#include <barrier>
#include <future>
#include <memory>
#include <optional>
#include <random>
#include <string>
#include <vector>

#include "DefaultDevs.hpp"

/**
 * @brief Input device manager.
 *
 * Searches for connected devices and pairs them with the engine.
 */
class PDJE_API PDJE_Input {
  private:
    std::optional<PDJE_DEFAULT_DEVICES::DefaultDevs> default_devs;
    // Null selects libremidi; tests inject a transport per input instance.
    std::shared_ptr<PDJE_MIDI::detail::Backend> midi_backend;

    void
    EnsureDefaultDevices();

    bool                           FLAG_INPUT_ON = false;
    std::optional<PDJE_MIDI::MIDI> midi_engine;
    bool                           FLAG_MIDI_ON    = false;
    bool                           midi_cc_lsb_on_ = true;
    PDJE_INPUT_STATE               state           = PDJE_INPUT_STATE::DEAD;
    void                          *platform_ctx0_  = nullptr;
    void                          *platform_ctx1_  = nullptr;
    bool                           use_internal_window_ = false;
#ifdef PDJE_UNIT_TESTING
    friend struct PDJE_Input_TestAccess;
#endif

  public:
    /** @brief Get All Connected devices.
     */
    std::vector<DeviceData>
    GetDevs();

    /** @brief Get All Connected MIDI devices.
     */
    std::vector<libremidi::input_port>
    GetMIDIDevs();
    /**
    @brief initialize pdje input.

    Device discovery and keyboard/mouse backend startup are deferred until
    GetDevs() or a Config() containing valid keyboard/mouse devices.

    Platform contexts (optional):
    - Linux: `platform_ctx0 = wl_display*`, `platform_ctx1 = wl_surface*`
    - Windows: currently ignored (reserved)

    `use_internal_window`:
    - Linux: if true, allows PDJE to create an internal Wayland window when
      evdev -> wayland fallback is needed and host handles are unavailable.
    - Windows: currently ignored (reserved)
    */
    bool
    Init(void *platform_ctx0       = nullptr,
         void *platform_ctx1       = nullptr,
         bool  use_internal_window = false);

    /**
    @brief configure device data.
    Empty keyboard/mouse devices with nonempty MIDI devices are supported.
    Success enters INPUT_LOOP_READY. Empty selections can be retried;
    backend setup failures tear down to DEAD (call Init() before retrying).
    */
    bool
    Config(std::vector<DeviceData>                  &devs,
           const std::vector<libremidi::input_port> &midi_dev);

    /** @brief Configure devices and CC pairing for the next Run().
     * The two-argument overload retains paired (14-bit) CC behavior.
     * Keep this option equal to Judge Custom_Events::midi_cc_lsb_on.
     */
    bool
    Config(std::vector<DeviceData>                  &devs,
           const std::vector<libremidi::input_port> &midi_dev,
           bool                                      midi_cc_lsb_on);

    /**
    @brief run input Loop
    Startup failure tears down all opened devices and returns to DEAD;
    an invalid-state call leaves the current state unchanged.
    */
    bool
    Run();

    /**
    @brief kill input Loop
    */
    bool
    Kill();

    /**
    @brief get pdje input module's configuration & running state
    */
    PDJE_INPUT_STATE
    GetState();

    /**
    @brief get current active input backend name.
    */
    std::string
    GetCurrentInputBackend() const;

    /**
    @brief pull out input data line. The input Loop will pass datas in here.
    MIDI-only configurations have null input_arena and nonnull midi_datas.
    Borrowed pointers expire on Kill(), startup failure, or destruction.
    */
    PDJE_INPUT_DATA_LINE
    PullOutDataLine();

    /**
    @brief Constructor.
    */
    PDJE_Input();

    /**
    @brief Destructor.
    */
    ~PDJE_Input()
    {
        Kill();
    }
};
