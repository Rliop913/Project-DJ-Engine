
.. _program_listing_file_include_input_IPC_memory_windows_named_event.cpp:

Program Listing for File named_event.cpp
========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_input_IPC_memory_windows_named_event.cpp>` (``include\input\IPC\memory\windows\named_event.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "ipc_named_event.hpp"
   #include <Windows.h>
   
   namespace PDJE_IPC {
   
   void
   EVENT::ClientInit(const MNAME &name)
   {
       hdlp = OpenEventW(
           SYNCHRONIZE | EVENT_MODIFY_STATE, FALSE, name.wstring().c_str());
       if (!hdlp) {
           throw std::runtime_error("failed to RecvInit ipc event.");
       }
   }
   void
   EVENT::HostInit(const MNAME &name)
   {
       hdlp = CreateEventW(nullptr, FALSE, FALSE, name.wstring().c_str());
       if (!hdlp) {
           auto err = GetLastError();
           throw std::runtime_error("failed to SenderInit ipc event." +
                                    std::to_string(err));
       }
   }
   void
   EVENT::Wait(const uint16_t waittime_ms)
   {
       auto w = WaitForSingleObject(hdlp, waittime_ms);
       if (w == WAIT_OBJECT_0 || w == WAIT_TIMEOUT) {
           return;
       } else {
           auto err = GetLastError();
           throw std::runtime_error("failed to wait ipc event." +
                                    std::to_string(err));
       }
   }
   
   void
   EVENT::Wait_Infinite()
   {
       auto w = WaitForSingleObject(hdlp, INFINITE);
       if (w == WAIT_OBJECT_0) {
           return;
       } else {
           auto err = GetLastError();
           throw std::runtime_error("failed to wait ipc event." +
                                    std::to_string(err));
       }
   }
   
   void
   EVENT::Wake()
   {
       if (!SetEvent(hdlp)) {
           auto err = GetLastError();
           throw std::runtime_error("failed to wake other ipc event." +
                                    std::to_string(err));
       }
   }
   
   EVENT::~EVENT()
   {
       if (hdlp) {
           CloseHandle(hdlp);
       }
   }
   
   }; // namespace PDJE_IPC
