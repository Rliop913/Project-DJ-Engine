
.. _program_listing_file_include_input_linux_socket_linux_socket.cpp:

Program Listing for File linux_socket.cpp
=========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_input_linux_socket_linux_socket.cpp>` (``include/input/linux/socket/linux_socket.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "linux_socket.hpp"
   #include "Common_Features.hpp"
   #include <spawn.h>
   #include <sys/socket.h>
   #include <sys/un.h>
   #include <unistd.h>
   
   namespace PDJE_LINUX {
   int
   PDJE_Socket::SocketOpen(const std::string &exec_path)
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
   PDJE_Socket::OpenClientWithSudo(const std::string &exec_path,
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
   PDJE_Socket::CloseClient()
   {
       // somthing to close client
       return 0;
   }
   
   void
   PDJE_Socket::SocketClose()
   {
       CloseClient();
       close(importants.client_fd);
       close(importants.socket_fd);
       unlink(importants.socket_path.c_str());
   }
   
   int
   PDJE_Socket::QueryClient(const std::string &query, std::string &result)
   {
       int res = Common_Features::LPSend(importants.client_fd, query);
       if (res != 0) {
           return res;
       }
       res = Common_Features::LPRecv(importants.client_fd, result);
       return res;
   }
   
   }; // namespace PDJE_LINUX
