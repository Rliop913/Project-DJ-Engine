#pragma once
#include "SubProcess.hpp"
#include "dev_path_to_name.hpp"
namespace PDJE_IPC {
struct RawDeviceData {
    RID_DEVICE_INFO info{};
    std::wstring    deviceHIDPath;
};
static std::vector<RawDeviceData>
getRawDeviceDatas()
{
    UINT num = 0;
    if (GetRawInputDeviceList(nullptr, &num, sizeof(RAWINPUTDEVICELIST)) != 0 ||
        num == 0)
        return {};

    std::vector<RAWINPUTDEVICELIST> list(num);
    if (GetRawInputDeviceList(list.data(), &num, sizeof(RAWINPUTDEVICELIST)) ==
        (UINT)-1)
        return {};

    std::vector<RawDeviceData> out;
    out.reserve(num);

    for (UINT i = 0; i < num; ++i) {
        RawDeviceData dev;
        auto          h = list[i].hDevice;

        UINT cbSize = dev.info.cbSize = sizeof(RID_DEVICE_INFO);
        if (GetRawInputDeviceInfoW(h, RIDI_DEVICEINFO, &dev.info, &cbSize) ==
            (UINT)-1)
            continue;

        UINT chars = 0;
        GetRawInputDeviceInfoW(h, RIDI_DEVICENAME, nullptr, &chars);
        if (chars > 0) {
            std::wstring path(chars, L'\0');
            if (GetRawInputDeviceInfoW(h, RIDI_DEVICENAME, &path[0], &chars) !=
                (UINT)-1) {
                if (!path.empty() && path.back() == L'\0')
                    path.pop_back();
            }
            dev.deviceHIDPath = path;
        }
        out.push_back(std::move(dev));
    }
    return out;
}

static std::string
wstring_to_utf8_nt(const std::wstring &w)
{
    if (w.empty())
        return {};
    auto target = w;
    if (target.rfind(L"\\??\\", 0) == 0) {
        target.replace(0, 4, L"\\\\?\\");
    }

    int required = WideCharToMultiByte(CP_UTF8,
                                       WC_ERR_INVALID_CHARS,
                                       target.c_str(),
                                       -1,
                                       nullptr,
                                       0,
                                       nullptr,
                                       nullptr);
    if (required <= 0) {

        critlog(
            "pdje input module-Windows impl- WideCharToMultiByte size failed");
        throw std::runtime_error("WideCharToMultiByte size failed");
    }

    std::string out(required, '\0');
    int         written = WideCharToMultiByte(CP_UTF8,
                                      WC_ERR_INVALID_CHARS,
                                      target.c_str(),
                                      -1,
                                      out.data(),
                                      required,
                                      nullptr,
                                      nullptr);
    if (written <= 0) {

        critlog("pdje input module-Windows impl- WideCharToMultiByte convert "
                "failed");
        throw std::runtime_error("WideCharToMultiByte convert failed");
    }

    if (!out.empty() && out.back() == '\0')
        out.pop_back();
    return out;
}

static std::string
hid_label_from_path(const std::wstring &path)
{
    auto name = GetFriendlyNameFromHidPath(path);
    return wstring_to_utf8_nt(name);
}

}; // namespace PDJE_IPC