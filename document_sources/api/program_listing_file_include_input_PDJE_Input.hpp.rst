
.. _program_listing_file_include_input_PDJE_Input.hpp:

Program Listing for File PDJE_Input.hpp
=======================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_input_PDJE_Input.hpp>` (``include/input/PDJE_Input.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include "Input_State.hpp"
   #include "PDJE_Input_DataLine.hpp"
   #include "PDJE_Input_Device_Data.hpp"
   #include <barrier>
   #include <future>
   #include <string>
   #include <vector>
   #ifdef WIN32
   #include "windows_input.hpp"
   #elif defined(__APPLE__)
   
   #else
   #include "linux_input.hpp"
   #endif
   
   class PDJE_Input {
     private:
       OS_Input data;
   
       template <typename P, typename F>
       void
       InitOneShot(P &promise, F &future, ONE_SHOT_SYNC &sync);
   
       template <typename P, typename F>
       void
       ResetOneShot(P &promise, F &future, ONE_SHOT_SYNC &sync);
       // std::vector<DeviceData> activated_devices;
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
   
       ONE_SHOT_DEV_PROMISE config_promise;
       ONE_SHOT_RUN_PROMISE run_command;
       PDJE_Input() = default;
   
       ~PDJE_Input() = default;
   };
   
   template <typename P, typename F>
   void
   PDJE_Input::InitOneShot(P &promise, F &future, ONE_SHOT_SYNC &sync)
   {
       promise.emplace();
       future.emplace();
       sync.emplace(2);
       future = promise->get_future();
   }
   
   template <typename P, typename F>
   void
   PDJE_Input::ResetOneShot(P &promise, F &future, ONE_SHOT_SYNC &sync)
   {
       promise.reset();
       future.reset();
       sync.reset();
   }
