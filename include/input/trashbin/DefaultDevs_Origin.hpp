#pragma once

#include "PDJE_Input_Device_Data.hpp"

namespace PDJE_DEFAULT_DEVICES {
class DefaultDevs_Origin {
  public:
    std::vector<DeviceData>
    GetDevices();

    bool
    Config();

    bool
    Kill();
    DefaultDevs_Origin()  = default;
    ~DefaultDevs_Origin() = default;
};
}; // namespace PDJE_DEFAULT_DEVICES