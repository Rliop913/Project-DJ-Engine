#include "windows_input.hpp"
#include "PDJE_Input.hpp"
#include "PDJE_LOG_SETTER.hpp"
#include "dev_path_to_name.hpp"
#include "windows_keyboard_fill.hpp"
#include <SetupAPI.h>
#include <bitset>
#include <format>
#include <memory_resource>
HWND
OS_Input::init()
{
    HINSTANCE hst = GetModuleHandleW(nullptr);
    WNDCLASSW wc{};
    wc.lpfnWndProc   = DefWindowProc;
    wc.hInstance     = hst;
    wc.lpszClassName = Invisible_window_name.c_str();
    RegisterClassW(&wc);

    return CreateWindowExW(0,
                           wc.lpszClassName,
                           L"",
                           0,
                           0,
                           0,
                           0,
                           0,
                           HWND_MESSAGE,
                           nullptr,
                           hst,
                           nullptr);
}

void
OS_Input::run()
{

    MSG msg;

    DWORD                                               w;
    UINT                                                size = 0;
    uint64_t                                            now;
    PDJE_Dev_Type                                       dtype;
    thread_local std::pmr::unsynchronized_pool_resource mono_arena;
    std::pmr::unsynchronized_pool_resource              hid_arena;
    std::string                                         handlestr;
    PDJE_Input_Event                                    tempEv;
    handlestr.reserve(100);
    PDJE_HID_Event   hidEv;
    std::bitset<101> isPressed;
    bool             Writable = true;
    while (true) {
        try {

            w = MsgWaitForMultipleObjectsEx(0,
                                            nullptr,
                                            INFINITE,
                                            QS_RAWINPUT | QS_POSTMESSAGE,
                                            MWMO_INPUTAVAILABLE |
                                                MWMO_ALERTABLE);
            if (w == WAIT_OBJECT_0) {

                if (PeekMessageW(&msg, nullptr, WM_QUIT, WM_QUIT, PM_REMOVE)) {

                    break;
                }
                while (PeekMessageW(
                    &msg, nullptr, WM_INPUT, WM_INPUT, PM_REMOVE)) {
                    Writable = true;
                    now      = timer.Get_MicroSecond();
                    if (GetRawInputData(reinterpret_cast<HRAWINPUT>(msg.lParam),
                                        RID_INPUT,
                                        nullptr,
                                        &size,
                                        sizeof(RAWINPUTHEADER)) != 0 ||
                        size == 0) {
                        continue;
                    }
                    std::pmr::vector<BYTE> buf(&mono_arena);
                    buf.reserve(size);
                    if (GetRawInputData(reinterpret_cast<HRAWINPUT>(msg.lParam),
                                        RID_INPUT,
                                        buf.data(),
                                        &size,
                                        sizeof(RAWINPUTHEADER)) != size) {
                        continue;
                    }

                    const RAWINPUT *ri =
                        reinterpret_cast<const RAWINPUT *>(buf.data());

                    switch (ri->header.dwType) {
                    case RIM_TYPEMOUSE:
                        dtype = PDJE_Dev_Type::MOUSE;
                        PDJE_RAWINPUT::FillMouseInput(tempEv, ri);
                        break;
                    case RIM_TYPEKEYBOARD:
                        dtype = PDJE_Dev_Type::KEYBOARD;
                        PDJE_RAWINPUT::FillKeyboardInput(tempEv, ri);
                        if (isPressed.test(tempEv.keyboard.k) &&
                            tempEv.keyboard.pressed) {
                            Writable = false;
                        } else {
                            isPressed.set(tempEv.keyboard.k,
                                          tempEv.keyboard.pressed);
                        }

                        break;
                    case RIM_TYPEHID:
                        dtype            = PDJE_Dev_Type::HID;
                        hidEv.hid_buffer = PDJE_RAWINPUT::FillHIDInput(
                            hid_arena, ri, hidEv.hid_byte_size);
                        break;
                    default:
                        dtype = PDJE_Dev_Type::UNKNOWN;
                        break;
                    }
                    handlestr = std::to_string(
                        reinterpret_cast<uintptr_t>(ri->header.hDevice));

                    if (!unlisted_targets.empty()) {
                        if (!id_name.contains(handlestr)) {

                            if (GetRawInputDeviceInfoW(ri->header.hDevice,
                                                       RIDI_DEVICENAME,
                                                       nullptr,
                                                       &size) == (UINT)-1 ||
                                size == 0) {
                            } else {
                                std::wstring path(size, L'\0');
                                if (GetRawInputDeviceInfoW(ri->header.hDevice,
                                                           RIDI_DEVICENAME,
                                                           path.data(),
                                                           &size) == (UINT)-1) {
                                } else {
                                    if (!path.empty() && path.back() == L'\0')
                                        path.pop_back();
                                    std::string device_path =
                                        wstring_to_utf8_nt(path);
                                    if (unlisted_targets.contains(
                                            device_path)) {
                                        id_name[handlestr] =
                                            unlisted_targets[device_path];
                                        unlisted_targets.erase(device_path);
                                    }
                                }
                            }
                        }
                    }
                    if (Writable) {

                        input_buffer.Write({ .type        = dtype,
                                             .event       = tempEv,
                                             .hid_event   = hidEv,
                                             .id          = handlestr,
                                             .microSecond = now });
                    }
                }

                while (PeekMessageW(&msg, nullptr, 0, WM_QUIT - 1, PM_REMOVE)) {
                }

                while (PeekMessageW(
                    &msg, nullptr, WM_QUIT + 1, 0xFFFF, PM_REMOVE)) {
                }
            }
        } catch (const std::exception &e) {
            critlog("runtime err. what: ");
            critlog(e.what());
        }
    }
}

void
OS_Input::work()
{

    auto msgOnly = init();

    if (!msgOnly)
        return;

    auto device_datas = config_data->get();
    config_sync->arrive_and_wait();
    if (device_datas.empty()) {
        return;
    }

    std::vector<RAWINPUTDEVICE> devTypes;
    bool                        hasKeyBoard = false;
    bool                        hasMouse    = false;
    bool                        hasHID      = false;
    for (const auto &dev : device_datas) {
        switch (dev.Type) {
        case PDJE_Dev_Type::MOUSE:
            hasMouse = true;
            break;
        case PDJE_Dev_Type::KEYBOARD:
            hasKeyBoard = true;
            break;
        case PDJE_Dev_Type::HID:
            hasHID = true;
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

    bool ok = run_ok->get();
    run_sync->arrive_and_wait();
    if (!ok) {
        if (task)
            AvRevertMmThreadCharacteristics(task);
        return;
    }

    run();

    if (task)
        AvRevertMmThreadCharacteristics(task);

    return;
}

// std::vector<RawDeviceData>
// OS_Input::getRawDeviceDatas()
// {
//     UINT num = 0;
//     if (GetRawInputDeviceList(nullptr, &num, sizeof(RAWINPUTDEVICELIST)) != 0 ||
//         num == 0)
//         return {};

//     std::vector<RAWINPUTDEVICELIST> list(num);
//     if (GetRawInputDeviceList(list.data(), &num, sizeof(RAWINPUTDEVICELIST)) ==
//         (UINT)-1)
//         return {};

//     std::vector<RawDeviceData> out;
//     out.reserve(num);

//     for (UINT i = 0; i < num; ++i) {
//         RawDeviceData dev;
//         auto          h = list[i].hDevice;

//         UINT cbSize = dev.info.cbSize = sizeof(RID_DEVICE_INFO);
//         if (GetRawInputDeviceInfoW(h, RIDI_DEVICEINFO, &dev.info, &cbSize) ==
//             (UINT)-1)
//             continue;

//         UINT chars = 0;
//         GetRawInputDeviceInfoW(h, RIDI_DEVICENAME, nullptr, &chars);
//         if (chars > 0) {
//             std::wstring path(chars, L'\0');
//             if (GetRawInputDeviceInfoW(h, RIDI_DEVICENAME, &path[0], &chars) !=
//                 (UINT)-1) {
//                 if (!path.empty() && path.back() == L'\0')
//                     path.pop_back();
//             }
//             dev.deviceHIDPath = path;
//         }
//         out.push_back(std::move(dev));
//     }
//     return out;
// }
#include <iostream>

#include <filesystem>
// std::string
// OS_Input::hid_label_from_path(const std::wstring &path)
// {
//     auto name = GetFriendlyNameFromHidPath(path);
//     return wstring_to_utf8_nt(name);
// }

// #include <iostream>
// std::vector<DeviceData>
// OS_Input::getDevices()
// {
//     auto                    devs = getRawDeviceDatas();
//     std::vector<DeviceData> out;
//     out.reserve(devs.size());
//     for (auto &i : devs) {
//         DeviceData tempdata;
//         switch (i.info.dwType) {
//         case RIM_TYPEMOUSE:
//             tempdata.Type = PDJE_Dev_Type::MOUSE;
//             break;
//         case RIM_TYPEKEYBOARD:
//             tempdata.Type = PDJE_Dev_Type::KEYBOARD;
//             break;
//         case RIM_TYPEHID:
//             tempdata.Type = PDJE_Dev_Type::HID;
//             break;
//         default:
//             tempdata.Type = PDJE_Dev_Type::UNKNOWN;
//             break;
//         }
//         tempdata.Name               = hid_label_from_path(i.deviceHIDPath);
//         tempdata.device_specific_id = wstring_to_utf8_nt(i.deviceHIDPath);
//         out.push_back(tempdata);
//     }
//     return out;
// }

bool
OS_Input::kill()
{
    return PostThreadMessageW(ThreadID, WM_QUIT, 0, 0);
}

void
OS_Input::TrigLoop()
{
    worker.emplace(std::thread([this]() { this->work(); }));
}

void
OS_Input::ResetLoop()
{
    worker->join();
    worker.reset();
}

PDJE_INPUT_DATA_LINE
OS_Input::PullOutDataLine()
{
    PDJE_INPUT_DATA_LINE dline;
    dline.input_arena  = &input_buffer;
    dline.id_name_conv = &id_name;
    return dline;
}