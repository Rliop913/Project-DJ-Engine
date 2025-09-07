
.. _program_listing_file_include_input_linux_LINUX_SUDO_GETTER.cpp:

Program Listing for File LINUX_SUDO_GETTER.cpp
==============================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_input_linux_LINUX_SUDO_GETTER.cpp>` (``include/input/linux/LINUX_SUDO_GETTER.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "LINUX_INPUT.hpp"
   
   #include <fcntl.h>
   #include <filesystem>
   #include <unistd.h>
   constexpr auto EVENT_ROOT = "/dev/input/";
   
   namespace fs = std::filesystem;
   
   using EV_LIST  = std::vector<std::string>;
   using DEV_LIST = std::vector<DeviceData>;
   
   EV_LIST
   EventListGetter()
   {
       if (!fs::exists(EVENT_ROOT) || !fs::is_directory(EVENT_ROOT)) {
           return EV_LIST();
       }
       EV_LIST lists;
       for (const auto &fitr : fs::directory_iterator(EVENT_ROOT)) {
           if (fitr.path().filename().string().find("event") == 0) {
               lists.push_back(fs::absolute(fitr.path()));
           }
       }
       return lists;
   }
   
   bool
   CHK_QWERTY(libevdev *dev)
   {
       return libevdev_has_event_code(dev, EV_KEY, KEY_Q) == 1 &&
              libevdev_has_event_code(dev, EV_KEY, KEY_W) == 1 &&
              libevdev_has_event_code(dev, EV_KEY, KEY_E) == 1 &&
              libevdev_has_event_code(dev, EV_KEY, KEY_R) == 1 &&
              libevdev_has_event_code(dev, EV_KEY, KEY_T) == 1 &&
              libevdev_has_event_code(dev, EV_KEY, KEY_Y) == 1;
   }
   
   DEV_LIST
   DeviceDataGetter(const EV_LIST &list)
   {
       DEV_LIST outs;
       for (const auto &evp : list) {
           int       evpFd = open(evp.c_str(), O_RDONLY);
           libevdev *dev   = nullptr;
           if (libevdev_new_from_fd(evpFd, &dev) < 0) {
               close(evpFd);
               continue;
           } else {
               DeviceData temp;
               temp.deviceName = libevdev_get_name(dev);
               temp.deviceType.HAS_ABSOLUTE_AXIS_SENSOR =
                   libevdev_has_event_type(dev, EV_ABS) == 1;
               temp.deviceType.HAS_KEY_Q_W_E_R_T_Y = CHK_QWERTY(dev);
               temp.deviceType.HAS_RELATIVE_AXIS_SENSOR =
                   libevdev_has_event_type(dev, EV_REL) == 1;
               if (temp.deviceType.HAS_ABSOLUTE_AXIS_SENSOR ||
                   temp.deviceType.HAS_KEY_Q_W_E_R_T_Y ||
                   temp.deviceType.HAS_RELATIVE_AXIS_SENSOR) {
                   outs.push_back(temp);
               }
               close(evpFd);
               libevdev_free(dev);
           }
       }
       return outs;
   }
   
   #include <iostream>
   int
   main(int argc, char *argv[])
   {
       auto res  = EventListGetter();
       auto Dres = DeviceDataGetter(res);
       for (auto i : Dres) {
           std::cout << i.deviceName << std::endl;
           std::cout << " Has qwerty: " << i.deviceType.HAS_KEY_Q_W_E_R_T_Y
                     << " Has Abs sensor: "
                     << i.deviceType.HAS_ABSOLUTE_AXIS_SENSOR
                     << " Has Rel sensor: "
                     << i.deviceType.HAS_RELATIVE_AXIS_SENSOR << std::endl;
       }
       return 0;
   }
