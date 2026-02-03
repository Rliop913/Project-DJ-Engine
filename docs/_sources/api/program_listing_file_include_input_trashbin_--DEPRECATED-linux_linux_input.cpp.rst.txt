
.. _program_listing_file_include_input_trashbin_--DEPRECATED-linux_linux_input.cpp:

Program Listing for File linux_input.cpp
========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_input_trashbin_--DEPRECATED-linux_linux_input.cpp>` (``include/input/trashbin/--DEPRECATED-linux/linux_input.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "linux_input.hpp"
   #include "Common_Features.hpp"
   #include "Input_State.hpp"
   #include "PDJE_Input_Device_Data.hpp"
   #include "RTSocket.hpp"
   #include "spawn.h"
   #include <cerrno>
   #include <sys/mman.h>
   #include <sys/socket.h>
   #include <sys/un.h>
   #include <unistd.h>
   
   #include <iostream>
   std::vector<DeviceData>
   OS_Input::getDevices()
   {
       nlohmann::json toSend;
       toSend["HEAD"] = "GET_DEV";
   
       std::vector<std::string> strlist;
       strlist.push_back("temp");
       toSend["BODY"] = strlist;
       std::string msggot;
       socket.QueryClient(toSend.dump(), msggot);
   
       DEV_LIST   lsDev;
       auto       got       = Common_Features::ReadMSG("GET_DEV", msggot);
       bool       flag_name = true;
       DeviceData dd;
       for (const auto &dev : got) {
           if (flag_name) {
               dd.Name   = dev;
               flag_name = false;
           } else {
               dd.Type   = PDJE_Dev_Type::UNKNOWN; // todo- change this
               flag_name = true;
               lsDev.push_back(dd);
               dd = DeviceData();
           }
       }
   
       return lsDev;
   }
   
   std::string
   OS_Input::setDevices(const DEV_LIST &devs)
   {
       data_body db;
       for (const auto &dev : devs) {
           db.push_back(dev.Name);
       }
       std::string msggot;
       socket.QueryClient(Common_Features::MakeMSG("SET_DEV", db), msggot);
       auto readed = Common_Features::ReadMSG("SET_DEV", msggot);
       if (readed.empty()) {
           return "";
       }
       return readed.front();
   }
   
   void
   OS_Input::EndSocketTransmission()
   {
       nlohmann::json toSend;
       toSend["HEAD"] = "END_SOCKET";
   
       std::vector<std::string> strlist;
       strlist.push_back("temp");
       toSend["BODY"] = strlist;
       std::string msggot;
       socket.QueryClient(toSend.dump(), msggot);
   
       std::cout << msggot << std::endl;
   }
