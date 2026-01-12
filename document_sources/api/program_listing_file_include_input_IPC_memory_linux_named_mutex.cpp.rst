
.. _program_listing_file_include_input_IPC_memory_linux_named_mutex.cpp:

Program Listing for File named_mutex.cpp
========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_input_IPC_memory_linux_named_mutex.cpp>` (``include\input\IPC\memory\linux\named_mutex.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "ipc_named_mutex.hpp"
   
   namespace PDJE_IPC {
   
   MUTEX::MUTEX()
   {
       startlog();
   }
   
   void
   MUTEX::init(const MNAME &name)
   {
       mutex_handle = CreateMutexW(nullptr, FALSE, name.wstring().c_str());
       if (!mutex_handle) {
           critlog("failed to create ipc mutex. on windows.");
       }
   }
   bool
   MUTEX::lock()
   {
       DWORD rc = WaitForSingleObject(mutex_handle, INFINITE);
       if (rc == WAIT_OBJECT_0) {
           return true;
       }
       if (rc == WAIT_ABANDONED) {
           critlog("IPC MUTEX Returned WAIT_ABANDONED.");
       }
       return false;
   }
   
   void
   MUTEX::unlock()
   {
       if (!ReleaseMutex(mutex_handle)) {
           critlog("failed to release mutex on windows.");
           throw std::runtime_error("failed to release mutex.");
       }
   }
   
   MUTEX::~MUTEX()
   {
       if (mutex_handle) {
           CloseHandle(mutex_handle);
       }
   }
   }; // namespace PDJE_IPC
