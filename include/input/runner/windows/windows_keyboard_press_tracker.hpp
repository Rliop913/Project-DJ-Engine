#pragma once

#include "Input_State.hpp"

#include <bitset>
#include <cstddef>
#include <cstdint>
#include <unordered_map>

namespace PDJE_RAWINPUT {

class KeyboardPressTracker {
  private:
    static constexpr std::size_t key_count =
        static_cast<std::size_t>(PDJE_KEY::UNKNOWN) + 1;

    std::unordered_map<std::uintptr_t, std::bitset<key_count>>
        pressed_by_device;

  public:
    bool
    ShouldWrite(std::uintptr_t device_id, PDJE_KEY key, bool pressed)
    {
        const auto key_index = static_cast<std::size_t>(key);
        if (key_index >= key_count) {
            return true;
        }

        auto &device_pressed = pressed_by_device[device_id];
        if (pressed && device_pressed.test(key_index)) {
            return false;
        }

        device_pressed.set(key_index, pressed);
        return true;
    }
};

} // namespace PDJE_RAWINPUT
