#pragma once

#include <string>
#include <vector>

#ifdef WIN32
// #define WIN32_LEAN_AND_MEAN
// #include <Windows.h>
// using DevID = HANDLE;

#else

#endif

/**
 * @brief Generic device identifier used by the input module.
 */
struct PDJE_IDEV {
    struct Finders {
        std::string devName;
        std::string vendorID;
        std::string productID;
    };
    std::string busType;
    std::string devType;
    struct Specifiers {
        DevID devID;
    };
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
