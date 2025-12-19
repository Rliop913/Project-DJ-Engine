#include "SubProcess.hpp"
#include "ListDevice.hpp"
#include "ipc_shared_memory.hpp"
#include <SetupAPI.h>
#include <Windows.h>
#include <avrt.h>
#include <hidsdi.h>
namespace PDJE_IPC {

bool
SubProc::RecvIPCSharedMem(const std::string &mem_path,
                          const std::string &dataType,
                          const uint64_t     data_count)
{
    try {

        if (dataType == "spinlock") {
            spinlock_run.emplace();
            spinlock_run->GetIPCSharedMemory(mem_path, data_count);
            return true;
        } else if (dataType == "input_buffer") {
            input_buffer.emplace(mem_path, data_count);
            return true;
        }
        return false;
    } catch (const std::exception &e) {
        critlog("failed to receive memory. WHY: ");
        critlog(e.what());
        return false;
    }
}

std::string
SubProc::ListDev()
{
    auto                    rawDevs = getRawDeviceDatas();
    std::vector<DeviceData> out;
    out.reserve(rawDevs.size());
    for (auto &i : rawDevs) {
        DeviceData tempdata;
        switch (i.info.dwType) {
        case RIM_TYPEMOUSE:
            tempdata.Type = PDJE_Dev_Type::MOUSE;
            break;
        case RIM_TYPEKEYBOARD:
            tempdata.Type = PDJE_Dev_Type::KEYBOARD;
            break;
        case RIM_TYPEHID:
            tempdata.Type = PDJE_Dev_Type::HID;
            break;
        default:
            tempdata.Type = PDJE_Dev_Type::UNKNOWN;
            break;
        }
        tempdata.Name               = hid_label_from_path(i.deviceHIDPath);
        tempdata.device_specific_id = wstring_to_utf8_nt(i.deviceHIDPath);
        out.push_back(tempdata);
    }
    nlohmann::json nj;
    nj["body"] = nlohmann::json::array();
    for (const auto &dev : out) {
        std::unordered_map<std::string, std::string> kv;
        kv["id"]   = dev.device_specific_id;
        kv["name"] = dev.Name;
        switch (dev.Type) {
        case PDJE_Dev_Type::KEYBOARD:
            kv["type"] = "KEYBOARD";
            nj["body"].push_back(kv);
            break;
        case PDJE_Dev_Type::MOUSE:
            kv["type"] = "MOUSE";
            nj["body"].push_back(kv);
            break;
        case PDJE_Dev_Type::MIDI:
            kv["type"] = "MIDI";
            nj["body"].push_back(kv);
            break;
        case PDJE_Dev_Type::HID:
            kv["type"] = "HID";
            nj["body"].push_back(kv);
            break;
        default:
            break;
        }
    }
    return nj.dump();
}

void
SubProc::RunServer(const int port)
{
    server.listen("127.0.0.1", port);
}
void
SubProc::EndTransmission(const httplib::Request &, httplib::Response &res)
{
    res.set_content("stopped", "text/plain");
    server.stop();
}

void
SubProc::LoopTrig()
{

    auto msgOnly = reinterpret_cast<HWND>(Init());

    if (!msgOnly)
        return;
    if (configed_devices.empty()) {
        critlog("no device has been configured. shutdown rawinput.");
        return;
    }
    if (!spinlock_run) {
        critlog("spinlock is not initialized.");
        return;
    }
    if (!input_buffer) {
        critlog("input buffer is not initialized.");
        return;
    }
    std::vector<RAWINPUTDEVICE> devTypes;
    bool                        hasKeyBoard = false;
    bool                        hasMouse    = false;
    bool                        hasHID      = false;
    for (const auto &dev : configed_devices) {
        switch (dev.Type) {
        case PDJE_Dev_Type::MOUSE:
            hasMouse = true;
            break;
        case PDJE_Dev_Type::KEYBOARD:
            hasKeyBoard = true;
            break;
        case PDJE_Dev_Type::HID:
            hasHID = true;
            break;
        default:
            break;
        }
        unlisted_targets[dev.device_specific_id] = dev.Name;
    }

    if (hasKeyBoard) {
        auto temp = RAWINPUTDEVICE{
            0x01, 0x06, RIDEV_INPUTSINK | RIDEV_NOLEGACY, msgOnly
        };
        devTypes.push_back(temp);
    }
    if (hasMouse) {
        auto temp = RAWINPUTDEVICE{
            0x01, 0x02, RIDEV_INPUTSINK | RIDEV_NOLEGACY, msgOnly
        };
        devTypes.push_back(temp);
    }
    if (hasHID) {
        auto temp = RAWINPUTDEVICE{
            0x0C, 0x01, RIDEV_INPUTSINK | RIDEV_NOLEGACY, msgOnly
        };
        devTypes.push_back(temp);
    }

    auto regres = RegisterRawInputDevices(
        devTypes.data(), devTypes.size(), sizeof(RAWINPUTDEVICE));
    if (!regres) {
        critlog("failed to register rawinput devices. maybe configed invalid "
                "devices.");
        return;
    }

    HANDLE task = nullptr;
    DWORD  idx  = 0;
    task        = AvSetMmThreadCharacteristicsW(L"Games", &idx);
    if (task) {
        AvSetMmThreadPriority(task, AVRT_PRIORITY_HIGH);
    }

// stop power throttling
#ifdef THREAD_POWER_THROTTLING_CURRENT_VERSION
    THREAD_POWER_THROTTLING_STATE s{};
    s.Version     = THREAD_POWER_THROTTLING_CURRENT_VERSION;
    s.ControlMask = THREAD_POWER_THROTTLING_EXECUTION_SPEED;
    s.StateMask   = 0; // Disable throttling
    SetThreadInformation(
        GetCurrentThread(), ThreadPowerThrottling, &s, sizeof(s));
#endif
    ThreadID = GetCurrentThreadId();

    while ((*spinlock_run->ptr) == 0) { // spinlock
        if ((*spinlock_run->ptr) == -1) {
            return; // terminate
        }
    }

    if (task) {
        AvRevertMmThreadCharacteristics(task);
    }

    Run();
    if (task) {
        AvRevertMmThreadCharacteristics(task);
    }
    return;
}

}; // namespace PDJE_IPC