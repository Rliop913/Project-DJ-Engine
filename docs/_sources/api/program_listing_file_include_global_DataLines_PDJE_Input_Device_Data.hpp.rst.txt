
.. _program_listing_file_include_global_DataLines_PDJE_Input_Device_Data.hpp:

Program Listing for File PDJE_Input_Device_Data.hpp
===================================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_global_DataLines_PDJE_Input_Device_Data.hpp>` (``include\global\DataLines\PDJE_Input_Device_Data.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   #include <cstdint>
   #include <future>
   #include <latch>
   #include <optional>
   #include <string>
   #include <vector>
   enum class PDJE_Dev_Type { MOUSE, KEYBOARD, UNKNOWN };
   
   struct DeviceData {
       PDJE_Dev_Type Type;
       std::string   Name;
       std::string   device_specific_id;
   };
   using DEV_LIST = std::vector<DeviceData>;
