
.. _program_listing_file_include_input_trashbin_--DEPRECATED-linux_linux_input.hpp:

Program Listing for File linux_input.hpp
========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_input_trashbin_--DEPRECATED-linux_linux_input.hpp>` (``include/input/trashbin/--DEPRECATED-linux/linux_input.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include "Input_State.hpp"
   #include "PDJE_Input_DataLine.hpp"
   #include "PDJE_Input_Device_Data.hpp"
   #include "linux_socket.hpp"
   
   #include <nlohmann/json.hpp>
   class OS_Input {
     private:
       PDJE_LINUX::PDJE_Socket socket;
   
     public:
       PDJE_INPUT_DATA_LINE
       PullOutDataLine()
       {
           return {};
       }
       bool
       kill()
       {
           return false;
       } // todo- impl this
       std::vector<DeviceData>
       getDevices();
       std::string
       setDevices(const DEV_LIST &devs);
   
       void
       TrigLoop()
       {
       } // todo- impl this
       void
       ResetLoop()
       {
       } // todo- impl this
   
       void
       EndSocketTransmission();
       OS_Input()  = default;
       ~OS_Input() = default;
   };
