#pragma once
#include <optional>
#include <future>
#include <string>

enum PDJE_INPUT_STATE{
    DEVICE_CONFIG_STATE = 0,
    INPUT_LOOP_READY,
    INPUT_LOOP_RUNNING,
    DEAD
};


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

using DEV_LIST = std::vector<DeviceData>;
using ONE_SHOT_DEV_PROMISE = std::optional<std::promise<DEV_LIST>>;
using ONE_SHOT_DEV_FUTURE = std::optional<std::future<DEV_LIST>>;

using ONE_SHOT_RUN_PROMISE = std::optional<std::promise<bool>>;
using ONE_SHOT_RUN_FUTURE = std::optional<std::future<bool>>;