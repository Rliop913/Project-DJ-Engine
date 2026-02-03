
.. _program_listing_file_include_input_DefaultDevs_linux_ParseKeyboard.cpp:

Program Listing for File ParseKeyboard.cpp
==========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_input_DefaultDevs_linux_ParseKeyboard.cpp>` (``include/input/DefaultDevs/linux/ParseKeyboard.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "InputCore.hpp"
   #include "evdev_codemap.hpp"
   
   void
   InputCore::kbRead(const input_event &evtrig, const int FD)
   {
       PDJE_Input_Log ilog;
       ilog.type             = PDJE_Dev_Type::KEYBOARD;
       ilog.event.keyboard.k = PDJE_EVDEV_KEYMAP::keyboard_map(evtrig.code);
       bool writable         = true;
   
       if (evtrig.value == 0) {
           ilog.event.keyboard.pressed = false;
       } else if (evtrig.value == 1) {
           ilog.event.keyboard.pressed = true;
       } else {
           writable = false;
       }
       if (writable) {
           if (id_pressed[FD].test(ilog.event.keyboard.k) ==
               ilog.event.keyboard.pressed) {
               writable = false;
           } else {
               id_pressed[FD].set(ilog.event.keyboard.k,
                                  ilog.event.keyboard.pressed);
           }
       }
       if (writable) {
           auto idstr = std::to_string(FD);
           std::memcpy(ilog.id, idstr.data(), idstr.size());
           ilog.id_len             = idstr.size();
           const std::string &name = id_to_name[FD];
   
           ilog.microSecond = clock.ConvertToMicroSecond(evtrig.time);
           std::memcpy(ilog.name, name.data(), name.size());
           ilog.name_len = name.size();
           out->Write(ilog);
       }
   }
