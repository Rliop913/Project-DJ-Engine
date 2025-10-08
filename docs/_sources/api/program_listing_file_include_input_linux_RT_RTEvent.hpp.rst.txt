
.. _program_listing_file_include_input_linux_RT_RTEvent.hpp:

Program Listing for File RTEvent.hpp
====================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_input_linux_RT_RTEvent.hpp>` (``include/input/linux/RT/RTEvent.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   #include <filesystem>
   #include <libevdev/libevdev.h>
   #include <unordered_map>
   namespace fs = std::filesystem;
   class RTEvent {
     private:
       std::unordered_map<int, libevdev *> events;
       void
       DrainEvents(const int epFD, int FD, libevdev *evdev);
       void
       use_event(const input_event &evtrig);
   
     public:
       void
       Reset();
       int
       Add(const fs::path &target);
       void
       Trig();
       RTEvent() = default;
       ~RTEvent();
   };
