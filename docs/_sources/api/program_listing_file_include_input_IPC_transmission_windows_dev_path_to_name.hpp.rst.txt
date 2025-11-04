
.. _program_listing_file_include_input_IPC_transmission_windows_dev_path_to_name.hpp:

Program Listing for File dev_path_to_name.hpp
=============================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_input_IPC_transmission_windows_dev_path_to_name.hpp>` (``include/input/IPC/transmission/windows/dev_path_to_name.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include <setupapi.h>
   #include <string>
   #include <vector>
   #include <windows.h>
   #pragma comment(lib, "setupapi.lib")
   
   inline std::wstring
   NormalizeNt(std::wstring p)
   {
       if (p.rfind(L"\\??\\", 0) == 0)
           p.replace(0, 4, L"\\\\?\\");
       return p;
   }
   
   // HID interface path(\\?\HID#...) -> FriendlyName or DeviceDesc
   static std::wstring
   GetFriendlyNameFromHidPath(const std::wstring &rawPath)
   {
       const std::wstring path = NormalizeNt(rawPath);
   
       HDEVINFO hset = SetupDiCreateDeviceInfoList(nullptr, nullptr);
       if (hset == INVALID_HANDLE_VALUE)
           return L"";
   
       SP_DEVICE_INTERFACE_DATA ifd{ sizeof(ifd) };
       if (!SetupDiOpenDeviceInterfaceW(hset, path.c_str(), 0, &ifd)) {
           SetupDiDestroyDeviceInfoList(hset);
           return L"";
       }
   
       // get SP_DEVINFO_DATA
       DWORD need = 0;
       SetupDiGetDeviceInterfaceDetailW(hset, &ifd, nullptr, 0, &need, nullptr);
       std::vector<BYTE> buf(need);
       auto             *det =
           reinterpret_cast<SP_DEVICE_INTERFACE_DETAIL_DATA_W *>(buf.data());
       det->cbSize = sizeof(*det);
       SP_DEVINFO_DATA dev{ sizeof(dev) };
       if (!SetupDiGetDeviceInterfaceDetailW(
               hset, &ifd, det, need, nullptr, &dev)) {
           SetupDiDestroyDeviceInfoList(hset);
           return L"";
       }
       // get friendly name first
       wchar_t name[512];
       if (SetupDiGetDeviceRegistryPropertyW(hset,
                                             &dev,
                                             SPDRP_FRIENDLYNAME,
                                             nullptr,
                                             reinterpret_cast<PBYTE>(name),
                                             sizeof(name),
                                             nullptr) ||
           SetupDiGetDeviceRegistryPropertyW(hset,
                                             &dev,
                                             SPDRP_DEVICEDESC,
                                             nullptr,
                                             reinterpret_cast<PBYTE>(name),
                                             sizeof(name),
                                             nullptr)) {
           SetupDiDestroyDeviceInfoList(hset);
           return name;
       }
   
       SetupDiDestroyDeviceInfoList(hset);
       return L"";
   }
