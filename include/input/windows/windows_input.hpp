#pragma once
#include <optional>
#include <Windows.h>
#include <thread>
#include <vector>
#include <avrt.h>
#include <string>
#include <mmsystem.h>
#include "QPC_Timer.hpp"
#include "Input_State.hpp"
#include <mutex>
#include <hidsdi.h>
// void 
// input_worker(DWORD& tid) 
// {
//     // 윈도우 클래스 등록
//     // auto now = qpc_now();
//     HINSTANCE hst = GetModuleHandleW(nullptr);
//     WNDCLASSW wc{};
//     wc.lpfnWndProc   = DefWindowProc;
//     wc.hInstance     = hst;
//     wc.lpszClassName = L"RawInputDemoWndClass";
//     RegisterClassW(&wc);


//     HWND msgOnly = CreateWindowExW(
//         0, wc.lpszClassName, L"",
//         0,0,0,0,0,
//         HWND_MESSAGE, nullptr, hst, nullptr
//     );
    
//     if (!msgOnly) return;


//     //=========================================================================================Dev Config=============================================================================================================
//     RAWINPUTDEVICE rids[2]{};
//     rids[0] = {0x01, 0x02, RIDEV_INPUTSINK | RIDEV_NOLEGACY, msgOnly}; // mouse
//     rids[1] = {0x01, 0x06, RIDEV_INPUTSINK | RIDEV_NOLEGACY, msgOnly}; // keyboard
//     auto regres = RegisterRawInputDevices(rids, 2, sizeof(RAWINPUTDEVICE));
//     if(!regres){
//         return;
//     }
    
//     //up thread priority
        
//     HANDLE task = nullptr;
//     DWORD  idx  = 0;
//     task = AvSetMmThreadCharacteristicsW(L"Games", &idx);
//     if (task) {
//         AvSetMmThreadPriority(task, AVRT_PRIORITY_HIGH);
//     }

//     //stop power throttling
//     #ifdef THREAD_POWER_THROTTLING_CURRENT_VERSION
//         THREAD_POWER_THROTTLING_STATE s{};
//         s.Version = THREAD_POWER_THROTTLING_CURRENT_VERSION;
//         s.ControlMask = THREAD_POWER_THROTTLING_EXECUTION_SPEED;
//         s.StateMask   = 0; // Disable throttling
//         SetThreadInformation(GetCurrentThread(), ThreadPowerThrottling, &s, sizeof(s));
//     #endif
//     tid = GetCurrentThreadId();
//     MSG msg;
//     int temp = 0;
//     double ms = 0;



//     //=========================================================================================Before loop=============================================================================================================


//     // now = qpc_now();
//     // RawInputParser msgp = RawInputParser();
//     while(true){
//         //=========================================================================================IN loop=============================================================================================================
//         DWORD w = MsgWaitForMultipleObjectsEx(
//             0, nullptr, INFINITE,
//             QS_RAWINPUT | QS_POSTMESSAGE,
//             MWMO_INPUTAVAILABLE | MWMO_ALERTABLE
//         );
//         if(w == WAIT_OBJECT_0){

//             if(PeekMessageW(&msg, nullptr, WM_QUIT, WM_QUIT, PM_REMOVE)){
//                 // std::cout << "trigged" << std::endl;
//                 break;
//             }
//             while(PeekMessageW(&msg, nullptr, WM_INPUT, WM_INPUT, PM_REMOVE)){
//                 // std::cout << "pressed" << qpc_ticks_to_ms(qpc_now() - now) << std::endl;
//                 // msgp.msgparse(reinterpret_cast<HRAWINPUT>(msg.lParam));
//                 // ms += qpc_ticks_to_ms(qpc_now() - now);
//                 // now = qpc_now();
//                 ++temp;
                
//                 // reinterpret_cast<HRAWINPUT>(msg.lParam);
//                 //use msg
//             }
            
//             while (PeekMessageW(&msg, nullptr, 0, WM_QUIT - 1, PM_REMOVE)) {}

//             while (PeekMessageW(&msg, nullptr, WM_QUIT + 1, 0xFFFF, PM_REMOVE)) {}
//         }
//     }
//     //=========================================================================================DEAD=============================================================================================================
//     if (task) AvRevertMmThreadCharacteristics(task);
    
//     // std::cout << "result" << ms / static_cast<double>(temp) << std::endl;
//     return;
// }

#include <future>


struct RawDeviceData {
    RID_DEVICE_INFO info{};
    std::wstring deviceHIDPath;
};

struct OS_Input{
private:
    HWND init();
    bool config();
    void run();
public:
    std::vector<RawDeviceData> getDevices();
    static std::wstring hid_label_from_path(const std::wstring& path);
    std::wstring Invisible_window_name = L"PDJE_Invisible_RawInput_Worker";
    DWORD ThreadID;

    // std::promise<DeviceData> config_data;
    std::future<std::vector<DeviceData>> config_data;
    std::optional<std::thread> worker;
    // std::mutex config_lock;
    std::mutex run_lock;
    void work();
};