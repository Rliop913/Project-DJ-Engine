
.. _program_listing_file_include_input_DefaultDevs_linux_LinuxMouseButtonMapping.hpp:

Program Listing for File LinuxMouseButtonMapping.hpp
====================================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_input_DefaultDevs_linux_LinuxMouseButtonMapping.hpp>` (``include/input/DefaultDevs/linux/LinuxMouseButtonMapping.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include "Input_State.hpp"
   
   #include <cstdint>
   #include <linux/input-event-codes.h>
   #include <optional>
   
   namespace PDJE_DEFAULT_DEVICES::LINUX_INPUT_MAP {
   
   inline std::optional<BITMASK>
   TryMapLinuxMouseButton(uint32_t button, bool pressed) noexcept
   {
       switch (button) {
       case BTN_LEFT:
           return static_cast<BITMASK>(pressed ? PDJE_MOUSE_L_BTN_DOWN
                                               : PDJE_MOUSE_L_BTN_UP);
       case BTN_RIGHT:
           return static_cast<BITMASK>(pressed ? PDJE_MOUSE_R_BTN_DOWN
                                               : PDJE_MOUSE_R_BTN_UP);
       case BTN_MIDDLE:
           return static_cast<BITMASK>(pressed ? PDJE_MOUSE_M_BTN_DOWN
                                               : PDJE_MOUSE_M_BTN_UP);
       case BTN_SIDE:
           return static_cast<BITMASK>(pressed ? PDJE_MOUSE_SIDE_BTN_DOWN
                                               : PDJE_MOUSE_SIDE_BTN_UP);
       case BTN_EXTRA:
           return static_cast<BITMASK>(pressed ? PDJE_MOUSE_EX_BTN_DOWN
                                               : PDJE_MOUSE_EX_BTN_UP);
       default:
           return std::nullopt;
       }
   }
   
   } // namespace PDJE_DEFAULT_DEVICES::LINUX_INPUT_MAP
