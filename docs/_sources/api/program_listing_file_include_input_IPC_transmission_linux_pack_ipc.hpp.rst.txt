
.. _program_listing_file_include_input_IPC_transmission_linux_pack_ipc.hpp:

Program Listing for File pack_ipc.hpp
=====================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_input_IPC_transmission_linux_pack_ipc.hpp>` (``include/input/IPC/transmission/linux/pack_ipc.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   #include "ipc_shared_memory.hpp"
   #include <sys/socket.h>
   
   namespace PDJE_IPC {
   
   template <typename T, int MEM_PROT_FLAG> struct IPC_SHM_LINUX {
     private:
       iovec    iov = { .iov_base = (void *)"F", .iov_len = 1 };
       cmsghdr *cmsg;
   
     public:
       msghdr msg = { 0 };
       IPC_SHM_LINUX(const SharedMem<T, MEM_PROT_FLAG> &shared_IPC_mem)
       {
           msg.msg_iov    = &iov;
           msg.msg_iovlen = 1;
           char cmsgbuf[CMSG_SPACE(sizeof(int))];
           msg.msg_control    = cmsgbuf;
           msg.msg_controllen = sizeof(cmsgbuf);
   
           cmsg             = CMSG_FIRSTHDR(&msg);
           cmsg->cmsg_level = SOL_SOCKET;
           cmsg->cmsg_type  = SCM_RIGHTS;
           cmsg->cmsg_len   = CMSG_LEN(sizeof(int));
           memcpy(CMSG_DATA(cmsg), &shared_IPC_mem.FD, sizeof(int));
       }
   };
   }; // namespace PDJE_IPC
