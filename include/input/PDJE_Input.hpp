#pragma once

#include "Input_State.hpp"
#include "PDJE_Input_DataLine.hpp"
#include "PDJE_Input_Device_Data.hpp"
#include <barrier>
#include <future>
#include <string>
#include <vector>
#ifdef WIN32
#include "windows_input.hpp"
#elif defined(__APPLE__)

#else
#include "linux_input.hpp"
#endif

/**
 * @brief Input device manager.
 *
 * Searches for connected devices and pairs them with the engine.
 */
class PDJE_Input {
  private:
    OS_Input data;

    template <typename P, typename F>
    void
    InitOneShot(P &promise, F &future, ONE_SHOT_SYNC &sync);

    template <typename P, typename F>
    void
    ResetOneShot(P &promise, F &future, ONE_SHOT_SYNC &sync);
    // std::vector<DeviceData> activated_devices;
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

    ONE_SHOT_DEV_PROMISE config_promise;
    ONE_SHOT_RUN_PROMISE run_command;
    /**
    @brief Constructor.
    */
    PDJE_Input() = default;

    /**
    @brief Destructor.
    */
    ~PDJE_Input() = default;
};

template <typename P, typename F>
void
PDJE_Input::InitOneShot(P &promise, F &future, ONE_SHOT_SYNC &sync)
{
    promise.emplace();
    future.emplace();
    sync.emplace(2);
    future = promise->get_future();
}

template <typename P, typename F>
void
PDJE_Input::ResetOneShot(P &promise, F &future, ONE_SHOT_SYNC &sync)
{
    promise.reset();
    future.reset();
    sync.reset();
}