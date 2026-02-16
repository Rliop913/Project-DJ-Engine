
.. _program_listing_file_include_input_trashbin_--DEPRECATED-linux_IPC_SHARE_MEM.hpp:

Program Listing for File IPC_SHARE_MEM.hpp
==========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_input_trashbin_--DEPRECATED-linux_IPC_SHARE_MEM.hpp>` (``include/input/trashbin/--DEPRECATED-linux/IPC_SHARE_MEM.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   #include <limits>
   #include <string>
   #include <sys/mman.h>
   #include <unistd.h>
   
   template <typename T, int MEM_PROT_FLAG> class IPCSharedMem {
     public:
       IPCSharedMem(std::string *external_logger_string)
       {
           external_logger = external_logger_string;
       }
   
       IPCSharedMem(const IPCSharedMem &) = delete;
       IPCSharedMem &
       operator=(const IPCSharedMem &) = delete;
   
       T           *ptr             = nullptr;
       int          FD              = -1;
       size_t       count           = 0;
       size_t       bytes           = 0;
       std::string *external_logger = nullptr;
   
       bool
       MakeIPCSharedMemory(const std::string &memfd_name, size_t data_length)
       {
   
           static_assert(
               std::is_trivially_copyable_v<T>,
               "Shared mem should contain trivially copyable types only.");
           if (data_length == 0) {
               *external_logger += "data_length is zero\n ";
               return false;
           }
           constexpr size_t Tsize    = sizeof(T);
           constexpr auto Max_length = std::numeric_limits<size_t>::max() / Tsize;
           if (Tsize != 0 && data_length > Max_length) {
               *external_logger += "byte size exceeded size_t maximum value\n ";
               return false;
           }
   
           FD = memfd_create(memfd_name.c_str(), MFD_CLOEXEC);
           if (FD < 0) {
               *external_logger += "memfd not created\n ";
               return false;
           }
   
           size_t byte_length = Tsize * data_length;
           if (ftruncate(FD, static_cast<off_t>(byte_length)) != 0) {
               close(FD);
               FD = -1;
               *external_logger += "ftruncate failed\n ";
               return false;
           }
   
           void *p = mmap(nullptr, byte_length, MEM_PROT_FLAG, MAP_SHARED, FD, 0);
           if (p == MAP_FAILED) {
               close(FD);
               FD = -1;
               *external_logger += "map failed\n ";
               return false;
           }
   
           ptr   = reinterpret_cast<T *>(p);
           count = data_length;
           bytes = byte_length;
           return true;
       }
   
       ~IPCSharedMem()
       {
           if (ptr && bytes > 0) {
               if (munmap(ptr, bytes) == -1) {
                   if (external_logger) {
                       *external_logger += "failed to munmap\n ";
                   }
               }
           }
           if (FD != -1) {
               if (close(FD) == -1) {
                   if (external_logger) {
                       *external_logger += "failed to close FD\n ";
                   }
               }
           }
       }
   };
