
.. _program_listing_file_include_input_IPC_memory_linux_Input_Transfer.cpp:

Program Listing for File Input_Transfer.cpp
===========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_input_IPC_memory_linux_Input_Transfer.cpp>` (``include/input/IPC/memory/linux/Input_Transfer.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "Input_Transfer.hpp"
   namespace PDJE_IPC {
   
   void
   PDJE_Input_Transfer::Receive()
   {
       datas = *adbf.Get();
   }
   PDJE_Input_Transfer::PDJE_Input_Transfer(const uint32_t max_length)
       : adbf(max_length)
   {
   }
   void
   PDJE_Input_Transfer::Write(const PDJE_Input_Log &input)
   {
       adbf.Write(input);
   }
   
   PDJE_Input_Transfer::~PDJE_Input_Transfer()
   {
   }
   
   }; // namespace PDJE_IPC
