
.. _program_listing_file_include_input_linux_RT_RTSocket.cpp:

Program Listing for File RTSocket.cpp
=====================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_input_linux_RT_RTSocket.cpp>` (``include/input/linux/RT/RTSocket.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "RTSocket.hpp"
   #include "Common_Features.hpp"
   #include "RTEvent.hpp"
   #include <exception>
   #include <stdexcept>
   #include <string>
   #include <sys/mman.h>
   #include <sys/socket.h>
   #include <sys/un.h>
   
   #include <iostream>
   int
   RTSocket::SocketOpen(const std::string &socket_path)
   {
       importants.host_socket = socket(AF_UNIX, SOCK_STREAM, 0);
   
       sockaddr_un addr{};
       addr.sun_family = AF_UNIX;
       if (connect(importants.host_socket,
                   reinterpret_cast<sockaddr *>(&addr),
                   sizeof(addr)) < 0) {
           std::cerr << errno << std::endl;
           return errno;
       }
       return 0;
   }
   
   int
   RTSocket::Communication()
   {
       std::string msg;
       int         resFlag = 0;
       while (true) {
           msg.clear();
           resFlag = Common_Features::LPRecv(importants.host_socket, msg);
           if (resFlag != 0) {
               return resFlag;
           }
           int parseFlag = ParseMsg(msg);
           if (parseFlag > 0) {
               return 0;
           }
   
           if (parseFlag != 0) {
               if (!errorHandler.contains(parseFlag)) {
                   throw std::runtime_error("failed to handle error." +
                                            std::to_string(parseFlag));
               }
               errorHandler.at(parseFlag)();
           }
       }
   }
   
   int
   RTSocket::SocketClose()
   {
       return close(importants.host_socket);
   }
   
   RTSocket::~RTSocket()
   {
       SocketClose();
       munlockall();
   }
   
   RTSocket::RTSocket(const std::string &socket_path, RTEvent *ptr)
   {
       rtev = ptr;
       setups.emplace();
       SocketOpen(socket_path);
       RegisterFunctions();
   }
   #include <iostream> //debug
   int
   RTSocket::ParseMsg(const std::string &raw_json_msg)
   {
       nj parsed;
       try {
           parsed = nj::parse(raw_json_msg);
   
       } catch (const std::exception &e) {
           return PDJE_RT_ERROR::FAILED_TO_PARSE_JSON;
       }
       if (parsed.contains("HEAD") && parsed.contains("BODY")) {
           if (parsed["HEAD"].is_string() && parsed["BODY"].is_array() &&
               functionRegistry.contains(parsed["HEAD"].get<std::string>())) {
               return functionRegistry[parsed["HEAD"].get<std::string>()](
                   parsed["BODY"].get<std::vector<std::string>>());
   
           } else {
               std::cout << raw_json_msg << std::endl;
   
               return PDJE_RT_ERROR::FAILED_TO_PARSE_JSON_HEAD_BODY;
           }
       } else {
           return PDJE_RT_ERROR::INVALID_JSON_FORMAT;
       }
   }
