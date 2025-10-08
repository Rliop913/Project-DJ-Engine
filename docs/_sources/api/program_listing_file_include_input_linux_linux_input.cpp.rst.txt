
.. _program_listing_file_include_input_linux_linux_input.cpp:

Program Listing for File linux_input.cpp
========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_input_linux_linux_input.cpp>` (``include/input/linux/linux_input.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "linux_input.hpp"
   #include "Common_Features.hpp"
   #include "Input_State.hpp"
   #include "RTSocket.hpp"
   #include "spawn.h"
   #include <cerrno>
   #include <sys/mman.h>
   #include <sys/socket.h>
   #include <sys/un.h>
   #include <unistd.h>
   
   int
   OS_Input::OpenClientWithSudo(const std::string &exec_path,
                                const std::string &arg)
   {
       char *pkexec_args[] = { (char *)"pkexec",
                               (char *)exec_path.c_str(),
                               (char *)arg.c_str(),
                               nullptr };
       char *sudo_args[]   = {
           (char *)"sudo", (char *)exec_path.c_str(), (char *)arg.c_str(), nullptr
       };
   
       if ((getenv("DISPLAY") || getenv("WAYLAND_DISPLAY")) &&
           access("/usr/bin/pkexec", X_OK) == 0) {
           int spawn_stat = posix_spawn(&importants.rt_pid,
                                        "/usr/bin/pkexec",
                                        nullptr,
                                        nullptr,
                                        pkexec_args,
                                        environ);
           if (spawn_stat == 0) {
               return 0;
           }
       } else {
           int spawn_stat = posix_spawn(&importants.rt_pid,
                                        "/usr/bin/sudo",
                                        nullptr,
                                        nullptr,
                                        sudo_args,
                                        environ);
           if (spawn_stat == 0) {
               return 0;
           }
       }
       return errno;
   }
   
   int
   OS_Input::SocketOpen(const std::string &exec_path)
   {
       unlink(importants.socket_path.c_str());
       importants.socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
   
       sockaddr_un address_temp{};
       address_temp.sun_family = AF_UNIX;
   
       if (bind(importants.socket_fd,
                reinterpret_cast<sockaddr *>(&address_temp),
                sizeof(address_temp)) < 0) {
           return errno;
       }
   
       if (listen(importants.socket_fd, 1) < 0) {
           return errno;
       }
   
       if (OpenClientWithSudo(exec_path, importants.socket_path) < 0) {
           return errno;
       }
   
       importants.client_fd = accept(importants.socket_fd, nullptr, nullptr);
       if (importants.client_fd < 0) {
           return errno;
       }
       return 0;
   }
   
   int
   OS_Input::CloseClient()
   {
       // somthing to close client
       return 0;
   }
   
   void
   OS_Input::SocketClose()
   {
       CloseClient();
       close(importants.client_fd);
       close(importants.socket_fd);
       unlink(importants.socket_path.c_str());
   }
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
       Common_Features::LPSend(importants.client_fd, toSend.dump());
       Common_Features::LPRecv(importants.client_fd, msggot);
       DEV_LIST   lsDev;
       auto       got       = Common_Features::ReadMSG("GET_DEV", msggot);
       bool       flag_name = true;
       DeviceData dd;
       for (const auto &dev : got) {
           if (flag_name) {
               dd.Name   = dev;
               flag_name = false;
           } else {
               dd.Type   = dev;
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
       Common_Features::LPSend(importants.client_fd,
                               Common_Features::MakeMSG("SET_DEV", db));
       std::string msggot;
       Common_Features::LPRecv(importants.client_fd, msggot);
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
       Common_Features::LPSend(importants.client_fd, toSend.dump());
       Common_Features::LPRecv(importants.client_fd, msggot);
       std::cout << msggot << std::endl;
   }
