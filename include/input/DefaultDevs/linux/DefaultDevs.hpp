#pragma once

#include "Input_State.hpp"
#include "WaylandRuntimeLoader.hpp"

#include "InputCore.hpp"
#include "PDJE_Input_DataLine.hpp"
#include "PDJE_Input_Device_Data.hpp"
#include "PDJE_LOG_SETTER.hpp"
#include <exception>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <optional>
#include <thread>
#include <unordered_map>
#include <vector>
namespace PDJE_DEFAULT_DEVICES {
using namespace PDJE_IPC;
using nj     = nlohmann::json;
namespace fs = std::filesystem;
class DefaultDevs {
  private:
    PDJE_IPC::PDJE_Input_Transfer input_buffer;
    struct device_metadata {
        fs::path      dev_path;
        PDJE_Dev_Type dev_type;
    };

    // key: device name (cross-platform compatibility key)
    std::unordered_map<std::string, device_metadata> stored_dev;

    std::optional<InputCore>   IC;
    std::optional<std::thread> input_thread;
    WaylandRuntimeLoader       wayland_loader;

  public:
    bool
    Kill()
    {
        return true; // compatibility no-op (windows parity)
    }
    std::vector<DeviceData>
    GetDevices();

    PDJE_IPC::PDJE_Input_Transfer *
    GetInputBufferPTR()
    {

        return &(input_buffer);
    }
    void
    Ready();

    void
    RunLoop()
    {
        if (!input_thread) {
            Ready();

            input_thread.emplace([this]() { IC->Trig(); });
        }
    }
    void
    TerminateLoop()
    {
        try {
            if (input_thread) {
                IC->Stop();
                if (input_thread->joinable()) {
                    input_thread->join();
                }
                input_thread.reset();
                IC.reset();
            }
        } catch (const std::exception &e) {
            critlog("input_thread join failed on linux. What: ");
            critlog(e.what());
            return;
        }
    }

    bool
    Config(const std::vector<DeviceData> &devs);
    DefaultDevs();
    ~DefaultDevs();
};

}; // namespace PDJE_DEFAULT_DEVICES
