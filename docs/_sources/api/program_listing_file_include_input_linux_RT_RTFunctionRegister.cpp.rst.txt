
.. _program_listing_file_include_input_linux_RT_RTFunctionRegister.cpp:

Program Listing for File RTFunctionRegister.cpp
===============================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_input_linux_RT_RTFunctionRegister.cpp>` (``include/input/linux/RT/RTFunctionRegister.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "Common_Features.hpp"
   #include "Input_State.hpp"
   #include "RTSocket.hpp"
   #include <filesystem>
   #include <iconv.h>
   #include <iostream>
   #include <string>
   #include <unistd.h>
   
   namespace fs = std::filesystem;
   void
   RTSocket::RegisterFunctions()
   {
       functionRegistry["GET_DEV"] = [this](const data_body &data) {
           auto      devs = ListDevices();
           data_body db;
           for (const auto &dev : devs) {
               db.push_back(dev.Name);
               db.push_back(dev.Type);
           }
   
           int sendFlag = Common_Features::LPSend(
               importants.host_socket, Common_Features::MakeMSG("GET_DEV", db));
           if (sendFlag < 0) {
               return sendFlag;
           }
           return 0;
       };
   
       functionRegistry["END_SOCKET"] = [this](const data_body &data) {
           std::cout << "got end socket" << std::endl;
           Common_Features::LPSend(
               importants.host_socket,
               Common_Features::MakeMSG("END_SOCKET", "Ended Ready Loop"));
           return 1;
       };
   
       functionRegistry["SET_DEV"] = [this](const data_body &data) {
           if (data.empty()) {
               return int(PDJE_RT_ERROR::FAILED_TO_SET_DEV__INVALID_DATA);
           }
           for (auto &name : data) {
               rtev->Add(stored_dev_path[name]);
           }
   
           Common_Features::LPSend(
               importants.host_socket,
               Common_Features::MakeMSG("SET_DEV", "Added Device."));
           return 0;
       };
       functionRegistry["RESET_DEV"] = [this](const data_body &data) {
           rtev->Reset();
   
           Common_Features::LPSend(
               importants.host_socket,
               Common_Features::MakeMSG("RESET_DEV", "Reset Added Devices."));
           return 0;
       };
   }
   
   DEV_LIST
   RTSocket::ListDevices()
   {
       DEV_LIST lsdev;
       fs::path device_root("/dev/input/");
       for (const auto &dev : fs::directory_iterator(device_root)) {
           if (!dev.is_character_file()) {
               continue;
           }
   
           const std::string dev_path = dev.path().string();
           if (dev_path.find("event") == std::string::npos) {
               continue;
           }
   
           int FD = open(dev_path.c_str(), O_RDONLY | O_NONBLOCK);
   
           if (FD < 0) {
               continue;
           }
           libevdev  *info = nullptr;
           DeviceData dd;
           if (libevdev_new_from_fd(FD, &info) == 0) {
               const char *dev_name = libevdev_get_name(info);
               if (dev_name) {
   
                   dd.Name = std::string(dev_name);
   
                   if (!stored_dev_path.contains(dd.Name)) {
                       stored_dev_path[dd.Name] = dev.path();
                   }
   
                   if (libevdev_has_event_type(info, EV_KEY) &&
                       libevdev_has_event_code(info, EV_KEY, KEY_A) &&
                       libevdev_has_event_code(info, EV_KEY, KEY_SPACE) &&
                       libevdev_has_event_code(info, EV_KEY, KEY_ENTER)) {
                       dd.Type = "KEYBOARD";
                   } else if (libevdev_has_event_type(info, EV_REL) &&
                              libevdev_has_event_code(info, EV_REL, REL_X) &&
                              libevdev_has_event_code(info, EV_REL, REL_Y)) {
                       dd.Type = "MOUSE";
                   } else if (libevdev_has_event_type(info, EV_ABS) &&
                              (libevdev_has_event_code(
                                   info, EV_KEY, BTN_GAMEPAD) ||
                               libevdev_has_event_code(
                                   info, EV_KEY, BTN_JOYSTICK))) {
                       dd.Type = "GAMEPAD";
                   } else if (libevdev_has_event_type(info, EV_ABS) &&
                              libevdev_has_event_code(info, EV_KEY, BTN_TOUCH)) {
                       dd.Type = "TOUCH";
                   } else {
                       dd.Type = "HID";
                   }
                   lsdev.push_back(dd);
               }
               libevdev_free(info);
               close(FD);
           } else {
               close(FD);
               continue;
           }
       }
       return lsdev;
   }
