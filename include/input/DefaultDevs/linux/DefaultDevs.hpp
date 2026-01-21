#pragma once

#include "Input_State.hpp"

#include "PDJE_EXPORT_SETTER.hpp"
#include "PDJE_Input_DataLine.hpp"
#include "PDJE_Input_Device_Data.hpp"
#include <nlohmann/json.hpp>
#include <optional>
#include <unordered_map>
namespace PDJE_DEFAULT_DEVICES {
using namespace PDJE_IPC;
using nj = nlohmann::json;

// using WINRAII = PDJE_RAII::RAII<HANDLE, HandleCloser>;
class DefaultDevs {
  private:
    std::optional<PDJE_IPC::PDJE_Input_Transfer> input_buffer;
    struct device_metadata {
        fs::path      dev_path;
        PDJE_Dev_Type dev_type;
    };

    std::unordered_map<std::string, device_metadata> stored_dev;
    // struct {
    //     EVENT input_loop_run_event;
    //     EVENT terminate_event;
    // } events;

    bool
    OpenProcess(const fs::path &pt);

    void
    InitEvents();

  public:
    bool
    Kill()
    {
        // return meta.Kill();
    }
    std::vector<DeviceData>
    GetDevices();

    PDJE_IPC::PDJE_Input_Transfer *
    GetInputBufferPTR()
    {
        return &(input_buffer.value());
    }
    void
    Ready();

    void
    RunLoop()
    {
        // events.input_loop_run_event.Wake();
    }
    void
    TerminateLoop()
    {
        // events.terminate_event.Wake();
    }

    bool
    Config(const std::vector<DeviceData> &devs);
    DefaultDevs();
    ~DefaultDevs();
};

}; // namespace PDJE_DEFAULT_DEVICES