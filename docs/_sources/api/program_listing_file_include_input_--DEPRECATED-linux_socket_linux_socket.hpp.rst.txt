
.. _program_listing_file_include_input_--DEPRECATED-linux_socket_linux_socket.hpp:

Program Listing for File linux_socket.hpp
=========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_input_--DEPRECATED-linux_socket_linux_socket.hpp>` (``include\input\--DEPRECATED-linux\socket\linux_socket.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   #include "IPC_SHARE_MEM.hpp"
   #include <string>
   #include <sys/socket.h>
   namespace PDJE_LINUX {
   class PDJE_Socket {
     private:
       struct {
           int         socket_fd = -1;
           int         client_fd = -1;
           pid_t       rt_pid    = -1;
           std::string socket_path =
               "/tmp/pdje_input_module_libevdev_socket_path.sock";
       } importants;
   
       int
       OpenClientWithSudo(const std::string &exec_path, const std::string &arg);
       int
       CloseClient();
   
       template <typename T, int MEM_PROT_FLAG>
       int
       Send_IPCMEM_ToRT(const IPCSharedMem<T, MEM_PROT_FLAG> &shared_IPC_mem);
   
     public:
       int
       SocketOpen(const std::string &exec_path);
       void
       SocketClose();
   
       int
       QueryClient(const std::string &query, std::string &result);
       PDJE_Socket()  = default;
       ~PDJE_Socket() = default;
   };
   
   template <typename T, int MEM_PROT_FLAG>
   int
   PDJE_Socket::Send_IPCMEM_ToRT(
       const IPCSharedMem<T, MEM_PROT_FLAG> &shared_IPC_mem)
   {
       struct msghdr msg = { 0 };
       // data trans
       struct iovec iov = { .iov_base =
                                (void *)"F", // 실제 payload 데이터 (필수는 아님)
                            .iov_len = 1 };
       msg.msg_iov      = &iov;
       msg.msg_iovlen   = 1;
       // end dummy data trans
   
       char cmsgbuf[CMSG_SPACE(sizeof(int))]; // ancillary buffer
       msg.msg_control    = cmsgbuf;
       msg.msg_controllen = sizeof(cmsgbuf);
   
       struct cmsghdr *cmsg = CMSG_FIRSTHDR(&msg);
       cmsg->cmsg_level     = SOL_SOCKET;
       cmsg->cmsg_type      = SCM_RIGHTS;
       cmsg->cmsg_len       = CMSG_LEN(sizeof(int));
   
       memcpy(CMSG_DATA(cmsg), &shared_IPC_mem.FD, sizeof(int));
       return sendmsg(importants.client_fd, &msg, 0);
   }
   }; // namespace PDJE_LINUX
