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
    */
    bool
    Init();

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
