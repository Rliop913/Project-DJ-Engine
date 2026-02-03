
.. _program_listing_file_include_input_trashbin_DefaultDevs_Origin.hpp:

Program Listing for File DefaultDevs_Origin.hpp
===============================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_input_trashbin_DefaultDevs_Origin.hpp>` (``include/input/trashbin/DefaultDevs_Origin.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include "PDJE_Input_Device_Data.hpp"
   
   namespace PDJE_DEFAULT_DEVICES {
   class DefaultDevs_Origin {
     public:
       std::vector<DeviceData>
       GetDevices();
   
       bool
       Config();
   
       bool
       Kill();
       DefaultDevs_Origin()  = default;
       ~DefaultDevs_Origin() = default;
   };
   }; // namespace PDJE_DEFAULT_DEVICES
