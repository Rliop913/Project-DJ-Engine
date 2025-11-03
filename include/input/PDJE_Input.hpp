#pragma once

#include "Input_State.hpp"
#include "PDJE_EXPORT_SETTER.hpp"
#include "PDJE_Input_DataLine.hpp"
#include "PDJE_Input_Device_Data.hpp"
#include <barrier>
#include <future>
#include <string>
#include <vector>
#include <optional>
#include <random>
#include "MainProcess.hpp"

#ifdef WIN32
// #include "windows_input.hpp"

#elif defined(__APPLE__)

#else
// #include "linux_input.hpp"
#endif

/**
 * @brief Input device manager.
 *
 * Searches for connected devices and pairs them with the engine.
 */
class PDJE_API PDJE_Input {
  private:
    std::optional<PDJE_IPC::MainProcess> http_bridge;
    
    PDJE_IPC::SharedMem<std::unordered_map<PDJE_ID, PDJE_NAME>, PDJE_IPC::PDJE_IPC_RW>       id_name;
    
    PDJE_Buffer_Arena<PDJE_Input_Log>            input_buffer;

    PDJE_IPC::SharedMem<int, PDJE_IPC::PDJE_IPC_RW> spinlock_run;// 0 = stop, 1 = go, -1 = terminate
    
    PDJE_INPUT_STATE state = PDJE_INPUT_STATE::DEAD;

  public:
    std::string ErrLog;
    /** @brief Get All Connected devices.
     */
    std::vector<DeviceData>
    GetDevs();

    /**
    @brief initialize pdje input.
    */
    bool
    Init();

    /**
    @brief configure device data.
    */
    bool
    Config(std::vector<DeviceData> &devs);

    /**
    @brief run input Loop
    */
    bool
    Run();

    /**
    @brief kill input Loop
    */
    bool
    Kill();

    /**
    @brief get pdje input module's configuration & running state
    */
    PDJE_INPUT_STATE
    GetState();

    /**
    @brief pull out input data line. The input Loop will pass datas in here.
    */
    PDJE_INPUT_DATA_LINE
    PullOutDataLine();

    /**
    @brief Constructor.
    */
    PDJE_Input();

    /**
    @brief Destructor.
    */
    ~PDJE_Input() = default;
};
