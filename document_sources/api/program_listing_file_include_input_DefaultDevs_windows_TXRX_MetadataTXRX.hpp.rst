
.. _program_listing_file_include_input_DefaultDevs_windows_TXRX_MetadataTXRX.hpp:

Program Listing for File MetadataTXRX.hpp
=========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_input_DefaultDevs_windows_TXRX_MetadataTXRX.hpp>` (``include/input/DefaultDevs/windows/TXRX/MetadataTXRX.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include "PDJE_Crypto.hpp"
   #include "Secured_IPC_TX_RX.hpp"
   #include <optional>
   
   #include "Input_State.hpp"
   #include "NameGen.hpp"
   #include "PDJE_Input_DataLine.hpp"
   #include "PDJE_Input_Device_Data.hpp"
   #include <future>
   
   namespace PDJE_IPC {
   class MetadataTXRX {
     private:
       std::optional<PDJE_CRYPTO::TX_RX> txrx;
   
       PDJE_CRYPTO::PSK psk;
       struct {
           std::optional<std::promise<bool>>                    HEALTH_CHECK;
           std::optional<std::promise<bool>>                    STOP;
           std::optional<std::promise<bool>>                    KILL;
           std::optional<std::promise<std::vector<DeviceData>>> DEVICE_LIST;
           std::optional<std::promise<bool>>                    DEVICE_CONFIG;
           std::optional<std::promise<bool>>                    SEND_IPC_SHMEM;
           std::optional<std::promise<bool>> SEND_INPUT_TRANSFER_SHMEM;
       } TXRX_RESPONSE;
   
       void
       SetTXRX_Features();
       bool
       EndTransmission();
   
     public:
       bool
       SendInputTransfer(PDJE_Input_Transfer &trsf);
   
       bool
       SendIPCSharedMemory(const uint64_t     mem_length,
                           const std::string &mem_path,
                           const std::string &dataType);
   
       std::stringstream
       GenTXRX();
       void
       Listen()
       {
           txrx->Listen();
       }
       bool
       QueryHealth();
       bool
       QueryConfig(const std::string &dumped_json);
       std::vector<DeviceData>
       QueryDevices();
       bool
       Kill();
   
       MetadataTXRX()  = default;
       ~MetadataTXRX() = default;
   };
   } // namespace PDJE_IPC
