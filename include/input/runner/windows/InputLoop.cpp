#include "SubProcess.hpp"
#include "ListDevice.hpp"
#include "windows_keyboard_fill.hpp"
#include <Windows.h>
#include <bitset>
namespace PDJE_IPC {

void *
SubProc::Init()
{
    HINSTANCE hst = GetModuleHandleW(nullptr);
    WNDCLASSW wc{};
    wc.lpfnWndProc   = DefWindowProc;
    wc.hInstance     = hst;
    wc.lpszClassName = L"PDJE_Invisible_RawInput_Worker";
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
SubProc::Run()
{

    MSG msg;

    DWORD                                               w;
    UINT                                                size = 0;
    uint64_t                                            now;
    PDJE_Dev_Type                                       dtype;
    thread_local std::pmr::unsynchronized_pool_resource mono_arena;

    std::string      handlestr;
    std::string      namestr;
    PDJE_Input_Event tempEv;
    handlestr.reserve(100);
    PDJE_HID_Event   hidEv;
    std::bitset<102> isPressed;
    bool             Writable = true;
    auto             killer   = std::thread([&]() {
        while (*spinlock_run->ptr == 1) {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        PostThreadMessageW(ThreadID, WM_QUIT, 0, 0);
    });
    PDJE_Input_Log   cachedLog;
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
                        dtype = PDJE_Dev_Type::HID;
                        PDJE_RAWINPUT::FillHIDInput(
                            hidEv.hid_buffer, ri, hidEv.hid_byte_size);
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
                        cachedLog.type      = dtype;
                        cachedLog.event     = tempEv;
                        cachedLog.hid_event = hidEv;
                        cachedLog.id_len =
                            handlestr.size() > 256 ? 256 : handlestr.size();
                        memcpy(cachedLog.id,
                               handlestr.data(),
                               sizeof(char) * (cachedLog.id_len));

                        namestr = id_name[handlestr];
                        cachedLog.name_len =
                            namestr.size() > 256 ? 256 : namestr.size();
                        memcpy(cachedLog.name,
                               namestr.data(),
                               sizeof(char) * (cachedLog.name_len));
                        cachedLog.microSecond = now;
                        input_buffer->Write(cachedLog);
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

    killer.join();
}

}; // namespace PDJE_IPC