#pragma once
#include "Input_Transfer.hpp"
#include "PDJE_Highres_Clock.hpp"
#include "PDJE_Input_Device_Data.hpp"
#include <atomic>
#include <bitset>
#include <filesystem>
#include <libevdev/libevdev.h>
#include <unordered_map>
namespace fs = std::filesystem;
class InputCore {
  private:
    PDJE_IPC::PDJE_Input_Transfer            *out;
    std::unordered_map<int, libevdev *>       events;
    std::unordered_map<int, PDJE_Dev_Type>    id_to_type;
    std::unordered_map<int, std::string>      id_to_name;
    std::unordered_map<int, std::bitset<102>> id_pressed;
    void
    DrainEvents(const int epFD, int FD, libevdev *evdev);
    void
    use_event(const input_event &evtrig, const int FD);
    void
    RemoveDeviceFD(int epFD, int FD);
    PDJE_HIGHRES_CLOCK::CLOCK clock;

    void
    kbRead(const input_event &evtrig, const int FD);

    void
    mouseRead(const input_event &evtrig, const int FD);

  public:
    struct {
        std::atomic<bool> loop_switch   = true;
        int               stop_event_fd = -1;
    } switches;

    void
    Reset();

    void
    Stop();
    void
    set_Input_Transfer(PDJE_IPC::PDJE_Input_Transfer *input_trsf)
    {
        out = input_trsf;
    }
    bool
    Add(const fs::path &target, PDJE_Dev_Type type, std::string name);
    void
    Trig();
    InputCore() = default;
    ~InputCore();
};
