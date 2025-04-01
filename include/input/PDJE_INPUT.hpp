#pragma once

#include <string>
#include <vector>
struct DuckTypeDevice{
    bool HAS_KEY_Q_W_E_R_T_Y = false;
    bool HAS_KEY = false;
    bool HAS_RELATIVE_AXIS_SENSOR = false;
    bool HAS_ABSOLUTE_AXIS_SENSOR = false;
    
};

struct DeviceData{
    std::string deviceName;
    DuckTypeDevice deviceType;
};

using DEV_LIST = std::vector<DeviceData>;

template<typename OS_INPUT>
class InputEngine{
private:
    OS_INPUT osAPI;
    DEV_LIST activated_devices;
public:
    InputEngine();
    ~InputEngine();
    void StoreDeviceList(const DEV_LIST& list);
    DEV_LIST SearchDevices();
    DEV_LIST GetStoredDeviceList();
    void setDevices(DEV_LIST);
    void ActivateEngine();
    void StopEngine();
};