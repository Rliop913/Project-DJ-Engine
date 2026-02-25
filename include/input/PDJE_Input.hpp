#pragma once

#include "Input_State.hpp"

#include "PDJE_EXPORT_SETTER.hpp"
#include "PDJE_Input_DataLine.hpp"
#include "PDJE_Input_Device_Data.hpp"
#include "PDJE_MIDI.hpp"
#include <barrier>
#include <future>
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
    // std::optional<PDJE_IPC::PDJE_Input_Transfer>     input_buffer; // redef
    // on dev pipe
    bool                           FLAG_INPUT_ON = false;
    std::optional<PDJE_MIDI::MIDI> midi_engine;
    bool                           FLAG_MIDI_ON = false;
    PDJE_INPUT_STATE               state        = PDJE_INPUT_STATE::DEAD;
    void                          *platform_ctx0_ = nullptr;
    void                          *platform_ctx1_ = nullptr;
    bool                           use_internal_window_ = false;

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

    Platform contexts (optional):
    - Linux: `platform_ctx0 = wl_display*`, `platform_ctx1 = wl_surface*`
    - Windows: currently ignored (reserved)

    `use_internal_window`:
    - Linux: if true, allows PDJE to create an internal Wayland window when
      evdev -> wayland fallback is needed and host handles are unavailable.
    - Windows: currently ignored (reserved)
    */
    bool
    Init(void *platform_ctx0      = nullptr,
         void *platform_ctx1      = nullptr,
         bool  use_internal_window = false);

    /**
    @brief configure device data.
    */
    bool
    Config(std::vector<DeviceData>                  &devs,
           const std::vector<libremidi::input_port> &midi_dev);

    /**
    @brief run input Loop
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
