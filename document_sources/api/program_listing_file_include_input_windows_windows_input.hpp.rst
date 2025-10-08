
.. _program_listing_file_include_input_windows_windows_input.hpp:

Program Listing for File windows_input.hpp
==========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_input_windows_windows_input.hpp>` (``include/input/windows/windows_input.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   #include <optional>
   #include <unordered_map>
   #include <Windows.h>
   #include <thread>
   #include <vector>
   #include <avrt.h>
   #include <string>
   #include <mmsystem.h>
   #include "QPC_Timer.hpp"
   #include "Input_State.hpp"
   #include <mutex>
   #include <hidsdi.h>
   #include <SetupAPI.h>
   #include <future>
   
   #include "PDJE_Input_DataLine.hpp"
   #include "PDJE_Buffer.hpp"
   
   struct RawDeviceData {
       RID_DEVICE_INFO info{};
       std::wstring deviceHIDPath;
   };
   
   using PDJE_DEV_PATH = std::string;
   
   
   
   class OS_Input{
   private:
       HWND init();
       bool config();
       void run();
       std::vector<RawDeviceData> getRawDeviceDatas();
       std::string hid_label_from_path(const std::wstring& path);
       DWORD ThreadID;
       QPC_Timer qpc;
       std::unordered_map<PDJE_DEV_PATH, PDJE_NAME> unlisted_targets;
       std::unordered_map<PDJE_ID, PDJE_NAME> id_name;
       PDJE_Buffer_Arena<PDJE_Input_Log> input_buffer;
   public:
       bool kill();
       void TrigLoop();
       void ResetLoop();
       void work();
       std::vector<DeviceData> getDevices();
       
       std::wstring Invisible_window_name = L"PDJE_Invisible_RawInput_Worker";
   
       ONE_SHOT_DEV_FUTURE config_data;
       ONE_SHOT_RUN_FUTURE run_ok;
       std::optional<std::thread> worker;
       PDJE_INPUT_DATA_LINE
       PullOutDataLine();
   
       OS_Input() = default;
       ~OS_Input() = default;
       
   };
