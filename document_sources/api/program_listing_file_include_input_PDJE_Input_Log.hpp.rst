
.. _program_listing_file_include_input_PDJE_Input_Log.hpp:

Program Listing for File PDJE_Input_Log.hpp
===========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_input_PDJE_Input_Log.hpp>` (``include/input/PDJE_Input_Log.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include "Input_State.hpp"
   #include "PDJE_EXPORT_SETTER.hpp"
   #include "PDJE_Input_Device_Data.hpp"
   
   struct PDJE_API PDJE_Input_Log {
       PDJE_Dev_Type    type;
       PDJE_Input_Event event;
       PDJE_HID_Event   hid_event;
       char             id[256];
       char             name[256];
       uint16_t         id_len;
       uint16_t         name_len;
       uint64_t         microSecond;
   };
