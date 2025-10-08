
.. _program_listing_file_include_input_linux_RT_RTSocket.hpp:

Program Listing for File RTSocket.hpp
=====================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_input_linux_RT_RTSocket.hpp>` (``include/input/linux/RT/RTSocket.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   #include "Common_Features.hpp"
   #include "Input_State.hpp"
   #include "OneTimeSysSetup.hpp"
   #include "RTEvent.hpp"
   #include "nlohmann/json_fwd.hpp"
   #include <cstddef>
   #include <fcntl.h>
   #include <filesystem>
   #include <functional>
   
   #include <optional>
   
   #include <string>
   #include <sys/epoll.h>
   #include <sys/mman.h>
   #include <sys/socket.h>
   #include <sys/un.h>
   #include <unistd.h>
   
   #include <nlohmann/json.hpp>
   
   using nj            = nlohmann::json;
   using data_body     = std::vector<std::string>;
   using regi_function = std::function<int(const data_body &)>;
   namespace fs        = std::filesystem;
   struct RT_ID {
       int host_socket = -1;
   };
   
   class RTSocket {
     private:
       RTEvent                                       *rtev;
       std::unordered_map<std::string, regi_function> functionRegistry;
       std::unordered_map<int, std::function<void()>> errorHandler;
       std::optional<OneTimeSysSetup>                 setups;
       RT_ID                                          importants;
   
       int
       ParseMsg(const std::string &raw_json_msg);
   
       int
       SocketOpen(const std::string &socket_path);
   
       int
       SocketClose();
   
       void
       RegisterFunctions();
   
       DEV_LIST
       ListDevices();
   
     public:
       std::unordered_map<std::string, fs::path> stored_dev_path;
       void
       SendMsg(const std::string &msg);
       int
       Communication();
   
       RTSocket(const std::string &socket_path, RTEvent *ptr);
       ~RTSocket();
       std::string ErrMsg;
   };
