
.. _program_listing_file_include_input_DefaultDevs_linux_ParseMouse.cpp:

Program Listing for File ParseMouse.cpp
=======================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_input_DefaultDevs_linux_ParseMouse.cpp>` (``include/input/DefaultDevs/linux/ParseMouse.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "InputCore.hpp"
   #include "LinuxMouseButtonMapping.hpp"
   #include <algorithm>
   #include <cstring>
   
   void
   InputCore::mouseRead(const input_event &evtrig, const int FD)
   {
       constexpr std::size_t kStringCap = 256;
   
       PDJE_Input_Log ilog{};
       ilog.type   = PDJE_Dev_Type::MOUSE;
       auto idstr  = std::to_string(FD);
       ilog.id_len =
           static_cast<uint16_t>(std::min(idstr.size(), kStringCap));
       std::memcpy(ilog.id, idstr.data(), ilog.id_len);
       ilog.microSecond        = clock.ConvertToMicroSecond(evtrig.time);
       auto name_it = id_to_name.find(FD);
       if (name_it == id_to_name.end()) {
           return;
       }
       const std::string &name = name_it->second;
       ilog.name_len =
           static_cast<uint16_t>(std::min(name.size(), kStringCap));
       std::memcpy(ilog.name, name.data(), ilog.name_len);
       switch (evtrig.type) {
       case EV_REL:
           ilog.event.mouse.axis_type   = PDJE_Mouse_Axis_Type::REL;
           ilog.event.mouse.button_type = 0;
           ilog.event.mouse.wheel_move  = 0;
           ilog.event.mouse.x           = 0;
           ilog.event.mouse.y           = 0;
           if (evtrig.code == REL_X) {
               ilog.event.mouse.x = evtrig.value;
           } else if (evtrig.code == REL_Y) {
               ilog.event.mouse.y = evtrig.value;
           } else if (evtrig.code == REL_WHEEL ||
                      evtrig.code == REL_HWHEEL) {
               ilog.event.mouse.axis_type = PDJE_Mouse_Axis_Type::PDJE_AXIS_IGNORE;
               ilog.event.mouse.wheel_move = evtrig.value;
           } else {
               return;
           }
           break;
       case EV_ABS:
           ilog.event.mouse.axis_type   = PDJE_Mouse_Axis_Type::ABS;
           ilog.event.mouse.button_type = 0;
           ilog.event.mouse.wheel_move  = 0;
           ilog.event.mouse.x           = 0;
           ilog.event.mouse.y           = 0;
           if (evtrig.code == ABS_X) {
               ilog.event.mouse.x = evtrig.value;
           } else if (evtrig.code == ABS_Y) {
               ilog.event.mouse.y = evtrig.value;
           } else {
               return;
           }
           break;
       case EV_KEY: {
           ilog.event.mouse.axis_type = PDJE_Mouse_Axis_Type::PDJE_AXIS_IGNORE;
   
           ilog.event.mouse.wheel_move = 0;
           ilog.event.mouse.x          = 0;
           ilog.event.mouse.y          = 0;
           const bool down             = (evtrig.value != 0);
           const auto mapped = PDJE_DEFAULT_DEVICES::LINUX_INPUT_MAP::
               TryMapLinuxMouseButton(evtrig.code, down);
           if (!mapped.has_value()) {
               return;
           }
           ilog.event.mouse.button_type = *mapped;
       } break;
       default:
           return;
       }
       out->Write(ilog);
   }
