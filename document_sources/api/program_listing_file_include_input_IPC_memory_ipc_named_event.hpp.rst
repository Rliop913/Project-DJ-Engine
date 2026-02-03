
.. _program_listing_file_include_input_IPC_memory_ipc_named_event.hpp:

Program Listing for File ipc_named_event.hpp
============================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_input_IPC_memory_ipc_named_event.hpp>` (``include/input/IPC/memory/ipc_named_event.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   #include <filesystem>
   namespace PDJE_IPC {
   using MNAME = std::filesystem::path;
   class EVENT {
     private:
       std::string name_cache;
   
     public:
       void *hdlp = nullptr;
       void
       Wait(const uint16_t waittime_ms = 100);
       void
       Wait_Infinite();
       void
       Wake();
       void
       HostInit(const MNAME &name);
       void
       ClientInit(const MNAME &name);
       EVENT() = default;
       ~EVENT();
   };
   }; // namespace PDJE_IPC
