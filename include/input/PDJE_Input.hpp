#pragma once

#include <string>
#include <vector>

#include "PDJE_Input_DataLine.hpp"
#include "Input_State.hpp"

#ifdef WIN32
    #include "windows_input.hpp"
#elif defined(__APPLE__)

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

struct DeviceData{
    std::string Type;
    std::wstring Name;
};

/**
 * @brief Input device manager.
 *
 * Searches for connected devices and pairs them with the engine.
 */
class PDJE_Input {
private:
    OS_Input data;
    bool Init();
    bool Config();
    bool Run();
    bool Kill();
public:
    std::string ErrLog;
    
    PDJE_INPUT_STATE state = PDJE_INPUT_STATE::DEAD;
    std::vector<DeviceData> GetDevs();
    void NEXT();
    PDJE_INPUT_DATA_LINE
    PullOutDataLine();

    /// Constructor.
    PDJE_Input();

    /// Destructor.
    ~PDJE_Input();
};
