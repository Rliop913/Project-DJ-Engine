
.. _program_listing_file_include_input_DefaultDevs_windows_DefaultDevs.hpp:

Program Listing for File DefaultDevs.hpp
========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_input_DefaultDevs_windows_DefaultDevs.hpp>` (``include/input/DefaultDevs/windows/DefaultDevs.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include "Input_State.hpp"
   #include "MetadataTXRX.hpp"
   #include "NameGen.hpp"
   #include "PDJE_Crypto.hpp"
   #include "PDJE_EXPORT_SETTER.hpp"
   #include "PDJE_Input_DataLine.hpp"
   #include "PDJE_Input_Device_Data.hpp"
   #include "PDJE_RAII_WRAP.hpp"
   #include "PSKPipe.hpp"
   #include "Secured_IPC_TX_RX.hpp"
   #include "ipc_named_event.hpp"
   #include <Windows.h>
   #include <nlohmann/json.hpp>
   #include <optional>
   namespace PDJE_DEFAULT_DEVICES {
   using namespace PDJE_IPC;
   using nj = nlohmann::json;
   
   struct HandleCloser {
       void
       operator()(HANDLE h) noexcept
       {
           if (h && h != INVALID_HANDLE_VALUE)
               ::CloseHandle(h);
       }
   };
   
   using WINRAII = PDJE_RAII::RAII<HANDLE, HandleCloser>;
   class DefaultDevs {
     private:
       STARTUPINFOW                                 start_up_info{};
       PROCESS_INFORMATION                          process_info{};
       std::optional<PDJE_IPC::PDJE_Input_Transfer> input_buffer;
       WINRAII                                      subprocess_RAII;
       MetadataTXRX                                 meta;
       PDJE_CRYPTO::PSKPipe                         pipe;
   
       struct {
           EVENT input_loop_run_event;
           EVENT terminate_event;
       } events;
   
       bool
       OpenProcess(const fs::path &pt);
   
       void
       InitEvents();
   
     public:
       bool
       Kill()
       {
           return meta.Kill();
       }
       std::vector<DeviceData>
       GetDevices()
       {
           return meta.QueryDevices();
       }
   
       PDJE_IPC::PDJE_Input_Transfer *
       GetInputBufferPTR()
       {
           return &(input_buffer.value());
       }
       void
       Ready();
   
       void
       RunLoop()
       {
           events.input_loop_run_event.Wake();
       }
       void
       TerminateLoop()
       {
           events.terminate_event.Wake();
       }
   
       bool
       Config(const std::vector<DeviceData> &devs);
       DefaultDevs();
       ~DefaultDevs();
   };
   
   }; // namespace PDJE_DEFAULT_DEVICES
