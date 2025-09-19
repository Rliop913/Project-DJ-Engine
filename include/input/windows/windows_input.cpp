#include "windows_input.hpp"
#include "PDJE_Input.hpp"


PDJE_Input::PDJE_Input()
{
    data.config_lock.lock();
    data.run_lock.lock();
}

bool
PDJE_Input::Init()
{
    if(state != PDJE_INPUT_STATE::DEAD){
        return false;
    }
    try
    {
        data.worker.emplace(std::thread([this](){
            this->data.work();
        }));
        state = PDJE_INPUT_STATE::DEVICE_CONFIG_STATE;
        return true;
    }
    catch(const std::exception& e)
    {
        ErrLog += e.what();
        ErrLog += "\n";
        return false;
    }
}

bool
PDJE_Input::Config()
{
    if(state != PDJE_INPUT_STATE::DEVICE_CONFIG_STATE){
        return false;
    }
    data.config_lock.unlock();
    state = PDJE_INPUT_STATE::INPUT_LOOP_READY;
    return true;
}

bool
PDJE_Input::Run()
{
    if(state != PDJE_INPUT_STATE::INPUT_LOOP_READY){
        return false;
    }
    data.run_lock.unlock();
    
    state = PDJE_INPUT_STATE::INPUT_LOOP_RUNNING;
    return true;
}

bool
PDJE_Input::Kill()
{
    if(state != PDJE_INPUT_STATE::INPUT_LOOP_RUNNING){
        return false;
    }
    bool flagOK = PostThreadMessageW(data.ThreadID, WM_QUIT, 0, 0);
    if(!flagOK){
        return false;
    }
    data.worker->join();
    state = PDJE_INPUT_STATE::DEAD;
    data.config_lock.lock();
    data.run_lock.lock();
    return true;
}

HWND
OS_Input::init()
{
    HINSTANCE hst = GetModuleHandleW(nullptr);
    WNDCLASSW wc{};
    wc.lpfnWndProc = DefWindowProc;
    wc.hInstance = hst;
    wc.lpszClassName = Invisible_window_name.c_str();
    RegisterClass(&wc);

    return CreateWindowExW(
        0, wc.lpszClassName, L"",
        0,0,0,0,0,
        HWND_MESSAGE, nullptr, hst, nullptr
    );
    
}

void
OS_Input::run()
{
    
    MSG msg;

    DWORD w;
    while(true){
        w = MsgWaitForMultipleObjectsEx(
            0, nullptr, INFINITE,
            QS_RAWINPUT | QS_POSTMESSAGE,
            MWMO_INPUTAVAILABLE | MWMO_ALERTABLE
        );
        if(w == WAIT_OBJECT_0){

            if(PeekMessageW(&msg, nullptr, WM_QUIT, WM_QUIT, PM_REMOVE)){
                // std::cout << "trigged" << std::endl;
                break;
            }
            while(PeekMessageW(&msg, nullptr, WM_INPUT, WM_INPUT, PM_REMOVE)){
                // std::cout << "pressed" << qpc_ticks_to_ms(qpc_now() - now) << std::endl;
                // msgp.msgparse(reinterpret_cast<HRAWINPUT>(msg.lParam));
                // ms += qpc_ticks_to_ms(qpc_now() - now);
                // now = qpc_now();
                // ++temp;
                
                // reinterpret_cast<HRAWINPUT>(msg.lParam);
                //use msg
            }
            
            while (PeekMessageW(&msg, nullptr, 0, WM_QUIT - 1, PM_REMOVE)) {}

            while (PeekMessageW(&msg, nullptr, WM_QUIT + 1, 0xFFFF, PM_REMOVE)) {}
        }
    }
}

void
OS_Input::work()
{

    auto msgOnly = init();

    if(!msgOnly) return;

    auto device_datas = config_data.get();
    std::vector<RAWINPUTDEVICE> devTypes;
    bool hasKeyBoard = false;
    bool hasMouse = false;
    bool hasHID = false;
    for(const auto& dev : device_datas){
        if(dev.Type == "MOUSE") hasMouse = true;
        else if(dev.Type == "KEYBOARD") hasKeyBoard = true;
        else if(dev.Type == "HID") hasHID = true;
    }

    if(hasKeyBoard){
        auto temp = RAWINPUTDEVICE{0x01, 0x06, RIDEV_INPUTSINK | RIDEV_NOLEGACY, msgOnly};
        devTypes.push_back(temp);
    }
    if(hasMouse){
        auto temp = RAWINPUTDEVICE{0x01, 0x02, RIDEV_INPUTSINK | RIDEV_NOLEGACY, msgOnly};
        devTypes.push_back(temp);
    }
    if(hasHID){
        auto temp = RAWINPUTDEVICE{0x0C, 0x01, RIDEV_INPUTSINK | RIDEV_NOLEGACY, msgOnly};
        devTypes.push_back(temp);
    }

    auto regres = RegisterRawInputDevices(devTypes.data(), devTypes.size(), sizeof(RAWINPUTDEVICE));
    if(!regres){
        return;
    }

    HANDLE task = nullptr;
    DWORD  idx  = 0;
    task = AvSetMmThreadCharacteristicsW(L"Games", &idx);
    if (task) {
        AvSetMmThreadPriority(task, AVRT_PRIORITY_HIGH);
    }

    //stop power throttling
    #ifdef THREAD_POWER_THROTTLING_CURRENT_VERSION
        THREAD_POWER_THROTTLING_STATE s{};
        s.Version = THREAD_POWER_THROTTLING_CURRENT_VERSION;
        s.ControlMask = THREAD_POWER_THROTTLING_EXECUTION_SPEED;
        s.StateMask   = 0; // Disable throttling
        SetThreadInformation(GetCurrentThread(), ThreadPowerThrottling, &s, sizeof(s));
    #endif
    ThreadID = GetCurrentThreadId();

    run_lock.lock();
    run_lock.unlock();

    run();

    if (task) AvRevertMmThreadCharacteristics(task);

    return;
}


std::vector<RawDeviceData> 
OS_Input::getDevices()
{
    UINT num = 0;
    if (GetRawInputDeviceList(nullptr, &num, sizeof(RAWINPUTDEVICELIST)) != 0 || num == 0)
        return {};

    std::vector<RAWINPUTDEVICELIST> list(num);
    if (GetRawInputDeviceList(list.data(), &num, sizeof(RAWINPUTDEVICELIST)) == (UINT)-1)
        return {};


    
    std::vector<RawDeviceData> out;
    out.reserve(num);

    for (UINT i = 0; i < num; ++i) {
        RawDeviceData dev;
        auto h = list[i].hDevice;

        UINT cbSize = dev.info.cbSize = sizeof(RID_DEVICE_INFO);
        if (GetRawInputDeviceInfoW(h, RIDI_DEVICEINFO, &dev.info, &cbSize) == (UINT)-1)
            continue;

        UINT chars = 0;
        GetRawInputDeviceInfoW(h, RIDI_DEVICENAME, nullptr, &chars);
        if (chars > 0) {
            std::wstring path(chars, L'\0');
            if (GetRawInputDeviceInfoW(h, RIDI_DEVICENAME, &path[0], &chars) != (UINT)-1) {
                if (!path.empty() && path.back() == L'\0') path.pop_back();
            }
            dev.deviceHIDPath = path;
        }
        out.push_back(std::move(dev));
    }
    return out;
}

std::wstring
OS_Input::hid_label_from_path(const std::wstring& path)
{
    HANDLE fh = CreateFileW(path.c_str(),
                            GENERIC_READ,
                            FILE_SHARE_READ | FILE_SHARE_WRITE,
                            nullptr, OPEN_EXISTING,
                            FILE_ATTRIBUTE_NORMAL, nullptr);
    if (fh == INVALID_HANDLE_VALUE) return L"";

    WCHAR man[128]{}, prod[128]{};
    std::wstring out;
    if (HidD_GetManufacturerString(fh, man, sizeof(man))) out += man;
    if (HidD_GetProductString(fh, prod, sizeof(prod))) {
        if (!out.empty()) out += L" ";
        out += prod;
    }
    CloseHandle(fh);
    return out;
}


std::vector<DeviceData>
PDJE_Input::GetDevs()
{
    auto devs = data.getDevices();
    std::vector<DeviceData> out;
    out.reserve(devs.size());
    for(auto& i : devs){
        DeviceData tempdata;
        switch(i.info.dwType){
        case RIM_TYPEMOUSE:    
            tempdata.Type = "MOUSE";
            break;
        case RIM_TYPEKEYBOARD: 
            tempdata.Type = "KEYBOARD";
            break;
        case RIM_TYPEHID:
            tempdata.Type = "HID";
            break;
        default:
            tempdata.Type = "???";
            break;
        }
        tempdata.Name = data.hid_label_from_path(i.deviceHIDPath);
        out.push_back(tempdata);
    }
    return out;
}