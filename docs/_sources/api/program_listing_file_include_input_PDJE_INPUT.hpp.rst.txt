
.. _program_listing_file_include_input_PDJE_INPUT.hpp:

Program Listing for File PDJE_INPUT.hpp
=======================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_input_PDJE_INPUT.hpp>` (``include/input/PDJE_INPUT.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include <string>
   #include <vector>
   
   #ifdef WIN32
   // #define WIN32_LEAN_AND_MEAN
   // #include <Windows.h>
   // using DevID = HANDLE;
   
   #else
   
   #endif
   
   struct PDJE_IDEV {
       struct Finders {
           std::string devName;
           std::string vendorID;
           std::string productID;
       };
       std::string busType;
       std::string devType;
       struct Specifiers {
           DevID devID;
       };
   };
   
   class PDJE_Input {
     private:
     public:
       void
       search();
   
       void
       set();
   
       void
       get();
   
       void
       pair_job();
   
       PDJE_Input();
   
       ~PDJE_Input();
   };
   
   // struct DuckTypeDevice{
   //     bool HAS_KEY_Q_W_E_R_T_Y = false;
   //     bool HAS_KEY = false;
   //     bool HAS_RELATIVE_AXIS_SENSOR = false;
   //     bool HAS_ABSOLUTE_AXIS_SENSOR = false;
   
   // };
   
   // struct DeviceData{
   //     std::string deviceName;
   //     DuckTypeDevice deviceType;
   // };
   
   // using DEV_LIST = std::vector<DeviceData>;
   
   // template<typename OS_INPUT>
   // class InputEngine{
   // private:
   //     OS_INPUT osAPI;
   //     DEV_LIST activated_devices;
   // public:
   //     InputEngine();
   //     ~InputEngine();
   //     void StoreDeviceList(const DEV_LIST& list);
   //     DEV_LIST SearchDevices();
   //     DEV_LIST GetStoredDeviceList();
   //     void setDevices(DEV_LIST);
   //     void ActivateEngine();
   //     void StopEngine();
   // };
