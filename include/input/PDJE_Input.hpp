#pragma once

#include <string>
#include <vector>

#include "PDJE_Input_DataLine.hpp"

#ifdef WIN32
// #define WIN32_LEAN_AND_MEAN
// #include <Windows.h>
// using DevID = HANDLE;

#else

#endif

enum PDJE_INPUT_TYPE{
    KEYBOARD = 0,
    MOUSE,
    MIDI
};

enum PDJE_MIDI_EVENTS{

};

struct Input_Type{
    std::string device_name;
    PDJE_INPUT_TYPE type;
};


using PDJE_KEY_CODE = std::string; // temp

struct Keyboard_Input_Data{
    PDJE_KEY_CODE key_code;
    bool down;
};

struct Mouse_Input_Data{
    bool is_btn_type;
    bool is_rel;
    int x;
    int y;
};

struct Midi_Input_Data{
    PDJE_MIDI_EVENTS event_type;
    uint8_t channel;
    uint8_t note;
    uint8_t velocity;
};

struct Input_Event{

};

/**
 * @brief Input device manager.
 *
 * Searches for connected devices and pairs them with the engine.
 */
class PDJE_Input {
  private:
  public:
    /**
     * @brief Scan the system for available input devices.
     */
    void
    search();

    /**
     * @brief Register a device with the engine.
     */
    void
    set();

    /**
     * @brief Retrieve the current device state.
     */
    void
    get();

    /**
     * @brief Pair the found devices with appropriate handlers.
     */
    void
    pair_job();

    PDJE_INPUT_DATA_LINE
    PullOutDataLine();

    /// Constructor.
    PDJE_Input();

    /// Destructor.
    ~PDJE_Input();
};

// struct DuckTypeDevice{
//     bool HAS_KEY_Q_W_E_R_T_Y = false;
//     bool HAS_KEY = false;
//     bool HAS_RELATIVE_AXIS_SENSOR = false;
//     bool HAS_ABSOLUTE_AXIS_SENSOR = false;

// };

// struct DeviceData{
//     std::string deviceName;
//     DuckTypeDevice deviceType;
// };

// using DEV_LIST = std::vector<DeviceData>;

// template<typename OS_INPUT>
// class InputEngine{
// private:
//     OS_INPUT osAPI;
//     DEV_LIST activated_devices;
// public:
//     InputEngine();
//     ~InputEngine();
//     void StoreDeviceList(const DEV_LIST& list);
//     DEV_LIST SearchDevices();
//     DEV_LIST GetStoredDeviceList();
//     void setDevices(DEV_LIST);
//     void ActivateEngine();
//     void StopEngine();
// };
