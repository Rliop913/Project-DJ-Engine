
.. _program_listing_file_include_input_host_MainProcess.cpp:

Program Listing for File MainProcess.cpp
========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_input_host_MainProcess.cpp>` (``include\input\host\MainProcess.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "MainProcess.hpp"
   
   namespace PDJE_IPC {
   using namespace MAINPROC;
   bool
   TXRXTransport::CheckHealth()
   {
       TXRX_RESPONSE.HEALTH_CHECK.emplace();
       auto resp = TXRX_RESPONSE.HEALTH_CHECK->get_future();
       bool res  = txrx->Send(PDJE_CRYPTO::TXRXHEADER::HEALTH_CHECK, "");
   
       if (res) {
           res = resp.get();
       }
   
       TXRX_RESPONSE.HEALTH_CHECK.reset();
       if (res)
           return true;
       else {
           critlog("health check failed.");
           return false;
       }
   }
   
   std::vector<DeviceData>
   TXRXTransport::GetDevices()
   {
       TXRX_RESPONSE.DEVICE_LIST.emplace();
       auto resp = TXRX_RESPONSE.DEVICE_LIST->get_future();
       bool res  = txrx->Send(PDJE_CRYPTO::TXRXHEADER::DEVICE_LIST, "");
   
       if (!res) {
           TXRX_RESPONSE.DEVICE_LIST.reset();
           critlog("failed to request device list.");
           return {};
       }
       return resp.get();
   }
   
   bool
   TXRXTransport::QueryConfig(const std::string &dumped_json)
   {
       TXRX_RESPONSE.DEVICE_CONFIG.emplace();
       auto resp = TXRX_RESPONSE.DEVICE_CONFIG->get_future();
       bool res  = txrx->Send(PDJE_CRYPTO::TXRXHEADER::DEVICE_CONFIG, dumped_json);
       if (res) {
           res = resp.get();
       }
   
       TXRX_RESPONSE.DEVICE_CONFIG.reset();
       if (res)
           return true;
       else {
           critlog("query configure failed.");
           return false;
       }
   }
   
   void
   TXRXTransport::InitEvents()
   {
       auto namegen  = PDJE_IPC::RANDOM_GEN();
       auto loop_run = namegen.Gen("PDJE_IPC_EVENT_LOOP_RUN_");
       auto term     = namegen.Gen("PDJE_IPC_EVENT_TERMINATE_");
   
       events.input_loop_run_event.HostInit(loop_run);
       events.terminate_event.HostInit(term);
       SendIPCSharedMemory(1, loop_run, "EVENT_input_loop_run");
       SendIPCSharedMemory(1, term, "EVENT_terminate");
   }
   bool
   TXRXTransport::Kill()
   {
       TXRX_RESPONSE.KILL.emplace();
       auto resp = TXRX_RESPONSE.KILL->get_future();
       bool res  = txrx->Send(PDJE_CRYPTO::TXRXHEADER::TXRX_KILL, "");
   
       if (res) {
           res = resp.get();
       }
   
       TXRX_RESPONSE.KILL.reset();
       txrx.reset();
       if (res)
           return true;
       else {
           critlog("failed to send kill signal.");
           return false;
       }
   }
   }; // namespace PDJE_IPC
