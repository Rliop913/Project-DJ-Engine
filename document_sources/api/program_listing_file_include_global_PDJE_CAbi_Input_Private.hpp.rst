
.. _program_listing_file_include_global_PDJE_CAbi_Input_Private.hpp:

Program Listing for File PDJE_CAbi_Input_Private.hpp
====================================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_global_PDJE_CAbi_Input_Private.hpp>` (``include\global\PDJE_CAbi_Input_Private.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #ifdef __cplusplus
   
   #include <cstddef>
   #include <string>
   #include <vector>
   
   #include "PDJE_Input_Device_Data.hpp"
   #include "PDJE_MIDI.hpp"
   
   struct PDJE_InputHandleV1 {
       void        *input = nullptr;
       std::string  backend_name_cache;
       void        *input_arena = nullptr;
       void        *midi_datas  = nullptr;
   };
   
   struct PDJE_InputDeviceListHandleV1 {
       std::vector<DeviceData> items;
   };
   
   struct PDJE_MidiDeviceListHandleV1 {
       std::vector<libremidi::input_port> items;
   };
   
   namespace PDJE_CABI {
   
   inline const PDJE_InputHandleV1 *
   BorrowInputDataLine(PDJE_InputHandleV1 *input) noexcept
   {
       return input;
   }
   
   inline const DeviceData *
   TryGetInputDevice(const PDJE_InputDeviceListHandleV1 *list, std::size_t index) noexcept
   {
       if (list == nullptr || index >= list->items.size()) {
           return nullptr;
       }
       return &list->items[index];
   }
   
   inline const libremidi::input_port *
   TryGetMidiDevice(const PDJE_MidiDeviceListHandleV1 *list,
                    std::size_t                        index) noexcept
   {
       if (list == nullptr || index >= list->items.size()) {
           return nullptr;
       }
       return &list->items[index];
   }
   
   } // namespace PDJE_CABI
   
   #endif
