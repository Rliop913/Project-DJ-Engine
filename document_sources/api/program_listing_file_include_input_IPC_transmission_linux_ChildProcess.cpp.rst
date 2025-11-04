
.. _program_listing_file_include_input_IPC_transmission_linux_ChildProcess.cpp:

Program Listing for File ChildProcess.cpp
=========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_input_IPC_transmission_linux_ChildProcess.cpp>` (``include/input/IPC/transmission/linux/ChildProcess.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "ChildProcess.hpp"
   #include "ipc_shared_memory.hpp"
   namespace PDJE_IPC {
   
   void
   ChildProcess::RunServer(const int port)
   {
       server.listen("0.0.0.0", port);
   }
   void
   ChildProcess::EndTransmission(const httplib::Request &, httplib::Response &res)
   {
       res.set_content("stopped", "text/plain");
       server.stop();
   }
   bool
   ChildProcess::RecvIPCSharedMem(const std::string &mem_path,
                                  const std::string &dataType,
                                  const uint64_t     data_count)
   {
       return false; // todo -impl
   }
   std::string
   ChildProcess::ListDev()
   {
       return {}; // todo - impl
   }
   
   void
   ChildProcess::LoopTrig()
   {
       return; // todo - impl
   }
   
   }; // namespace PDJE_IPC
