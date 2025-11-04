
.. _program_listing_file_include_input_PDJE_Input.hpp:

Program Listing for File PDJE_Input.hpp
=======================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_input_PDJE_Input.hpp>` (``include/input/PDJE_Input.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include "Input_State.hpp"
   #include "PDJE_EXPORT_SETTER.hpp"
   #include "PDJE_Input_DataLine.hpp"
   #include "PDJE_Input_Device_Data.hpp"
   #include <barrier>
   #include <future>
   #include <string>
   #include <vector>
   #include <optional>
   #include <random>
   #include "MainProcess.hpp"
   
   #ifdef WIN32
   // #include "windows_input.hpp"
   
   #elif defined(__APPLE__)
   
   #else
   // #include "linux_input.hpp"
   #endif
   
   class PDJE_API PDJE_Input {
     private:
       std::optional<PDJE_IPC::MainProcess> http_bridge;
       
       PDJE_IPC::SharedMem<std::unordered_map<PDJE_ID, PDJE_NAME>, PDJE_IPC::PDJE_IPC_RW>       id_name;
       
       PDJE_Buffer_Arena<PDJE_Input_Log>            input_buffer;
   
       PDJE_IPC::SharedMem<int, PDJE_IPC::PDJE_IPC_RW> spinlock_run;// 0 = stop, 1 = go, -1 = terminate
       
       PDJE_INPUT_STATE state = PDJE_INPUT_STATE::DEAD;
   
     public:
       std::string ErrLog;
       std::vector<DeviceData>
       GetDevs();
   
       bool
       Init();
   
       bool
       Config(std::vector<DeviceData> &devs);
   
       bool
       Run();
   
       bool
       Kill();
   
       PDJE_INPUT_STATE
       GetState();
   
       PDJE_INPUT_DATA_LINE
       PullOutDataLine();
   
       PDJE_Input();
   
       ~PDJE_Input(){
         Kill();
       }
   };
