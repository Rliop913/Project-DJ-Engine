#pragma once
#include "PDJE_INPUT.hpp"
#include <libevdev-1.0/libevdev/libevdev.h>


class LinuxEVDEV{
public:
    void getList();
    void init();
};