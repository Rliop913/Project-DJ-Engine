#pragma once
#include "PDJE_INPUT.hpp"
#include <libevdev-1.0/libevdev/libevdev.h>

/**
 * @brief Linux implementation of the input backend using libevdev.
 */
class LinuxEVDEV {
  public:
    /**
     * @brief Enumerate available input devices on the system.
     */
    void
    getList();

    /**
     * @brief Initialize the backend and prepare for input polling.
     */
    void
    init();
};
