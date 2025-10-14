#pragma once
#include "Input_State.hpp"
#include "PDJE_Highres_Clock.hpp"
#include <SetupAPI.h>
#include <Windows.h>
#include <avrt.h>
#include <future>
#include <hidsdi.h>
#include <mmsystem.h>
#include <mutex>
#include <optional>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#include "PDJE_Buffer.hpp"
#include "PDJE_Input_DataLine.hpp"

struct RawDeviceData {
    RID_DEVICE_INFO info{};
    std::wstring    deviceHIDPath;
};

using PDJE_DEV_PATH = std::string;

class OS_Input {
  private:
    HWND
    init();
    bool
    config();
    void
    run();
    std::vector<RawDeviceData>
    getRawDeviceDatas();
    std::string
                              hid_label_from_path(const std::wstring &path);
    DWORD                     ThreadID;
    PDJE_HIGHRES_CLOCK::CLOCK timer;
    std::unordered_map<PDJE_DEV_PATH, PDJE_NAME> unlisted_targets;
    std::unordered_map<PDJE_ID, PDJE_NAME>       id_name;
    PDJE_Buffer_Arena<PDJE_Input_Log>            input_buffer;

  public:
    bool
    kill();
    void
    TrigLoop();
    void
    ResetLoop();
    void
    work();
    std::vector<DeviceData>
    getDevices();

    std::wstring Invisible_window_name = L"PDJE_Invisible_RawInput_Worker";

    ONE_SHOT_DEV_FUTURE        config_data;
    ONE_SHOT_RUN_FUTURE        run_ok;
    std::optional<std::thread> worker;
    PDJE_INPUT_DATA_LINE
    PullOutDataLine();

    OS_Input()  = default;
    ~OS_Input() = default;
};