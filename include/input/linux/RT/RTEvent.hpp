#pragma once
#include <filesystem>
#include <libevdev/libevdev.h>
#include <unordered_map>
namespace fs = std::filesystem;
class RTEvent {
  private:
    std::unordered_map<int, libevdev *> events;
    void
    DrainEvents(const int epFD, int FD, libevdev *evdev);
    void
    use_event(const input_event &evtrig);

  public:
    void
    Reset();
    int
    Add(const fs::path &target);
    void
    Trig();
    RTEvent() = default;
    ~RTEvent();
};