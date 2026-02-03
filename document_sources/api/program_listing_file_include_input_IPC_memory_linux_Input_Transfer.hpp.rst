
.. _program_listing_file_include_input_IPC_memory_linux_Input_Transfer.hpp:

Program Listing for File Input_Transfer.hpp
===========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_input_IPC_memory_linux_Input_Transfer.hpp>` (``include/input/IPC/memory/linux/Input_Transfer.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   #include "PDJE_Buffer.hpp"
   #include "PDJE_Input_Log.hpp"
   #include <nlohmann/json.hpp>
   using nj = nlohmann::json;
   
   namespace PDJE_IPC {
   class PDJE_API PDJE_Input_Transfer {
     private:
     public:
       std::vector<PDJE_Input_Log>          datas;
       Atomic_Double_Buffer<PDJE_Input_Log> adbf;
       PDJE_Input_Transfer(const uint32_t max_length);
   
       void
       Write(const PDJE_Input_Log &input);
       void
       Receive();
       ~PDJE_Input_Transfer();
   };
   
   }; // namespace PDJE_IPC
