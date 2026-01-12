
.. _program_listing_file_include_input_IPC_memory_ipc_shared_memory.hpp:

Program Listing for File ipc_shared_memory.hpp
==============================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_input_IPC_memory_ipc_shared_memory.hpp>` (``include\input\IPC\memory\ipc_shared_memory.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include "PDJE_Crypto.hpp"
   #include "PDJE_INPUT_PROCESS_HASH.hpp"
   #include "PDJE_LOG_SETTER.hpp"
   #include <algorithm>
   #include <cctype>
   #include <cstddef>
   #include <filesystem>
   #include <fstream>
   #include <ios>
   #include <limits>
   
   #include <string>
   #include <vector>
   #ifdef WIN32
   #define NOMINMAX
   #include <Windows.h>
   #endif
   namespace PDJE_IPC {
   
   namespace fs              = std::filesystem;
   constexpr int PDJE_NO_IPC = 2;
   constexpr int PDJE_IPC_R  = 1;
   constexpr int PDJE_IPC_RW = 0;
   
   template <typename T, int MEM_PROT_FLAG> class SharedMem {
     public:
       SharedMem()
       {
           startlog();
       }
   
       SharedMem(const SharedMem &) = delete;
       SharedMem &
       operator=(const SharedMem &) = delete;
   
       T       *ptr        = nullptr;
       uint64_t data_count = 0;
   #ifdef WIN32
       HANDLE memory_handle = nullptr;
   #elif defined(__linux__)
       std::string mem_name_if_owner = "";
   #endif
   
       bool
       GetIPCSharedMemory(const fs::path &memfd_name, const uint64_t count);
   
       bool
       MakeIPCSharedMemory(const fs::path &memfd_name, const uint64_t count);
   
       ~SharedMem();
   };
   
   }; // namespace PDJE_IPC
   #ifdef WIN32
   #include "windows_ipc.tpp"
   #elif defined(__linux__)
   #include "linux_ipc.tpp"
   #endif
