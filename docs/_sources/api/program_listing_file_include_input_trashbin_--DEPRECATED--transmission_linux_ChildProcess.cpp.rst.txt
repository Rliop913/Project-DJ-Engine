
.. _program_listing_file_include_input_trashbin_--DEPRECATED--transmission_linux_ChildProcess.cpp:

Program Listing for File ChildProcess.cpp
=========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_input_trashbin_--DEPRECATED--transmission_linux_ChildProcess.cpp>` (``include/input/trashbin/--DEPRECATED--transmission/linux/ChildProcess.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "ChildProcess.hpp"
   #include "ipc_shared_memory.hpp"
   namespace PDJE_IPC {
   
   void
   TXRXListener::RunServer(const int port)
   {
       server.listen("0.0.0.0", port);
   }
   void
   TXRXListener::EndTransmission(const httplib::Request &, httplib::Response &res)
   {
       res.set_content("stopped", "text/plain");
       server.stop();
   }
   bool
   TXRXListener::RecvIPCSharedMem(const std::string &mem_path,
                                  const std::string &dataType,
                                  const uint64_t     data_count)
   {
       return false; // todo -impl
   }
   std::string
   TXRXListener::ListDev()
   {
       return {}; // todo - impl
   }
   
   void
   TXRXListener::LoopTrig()
   {
       return; // todo - impl
   }
   
   }; // namespace PDJE_IPC
