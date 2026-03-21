
.. _program_listing_file_include_tests_INPUT_TESTS_evdevtest.cpp:

Program Listing for File evdevtest.cpp
======================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_tests_INPUT_TESTS_evdevtest.cpp>` (``include/tests/INPUT_TESTS/evdevtest.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "LINUX_INPUT.hpp"
   #include <chrono>
   #include <fcntl.h>
   #include <iostream>
   #include <linux/futex.h>
   #include <sys/mman.h>
   #include <sys/syscall.h>
   #include <unistd.h>
   
   int
   main(int argc, char *argv[])
   {
       std::string posRoot = "/dev/input/event";
       int         evdevEventFD;
       libevdev   *dev = nullptr;
       for (int i = 14; i > 0; --i) {
           std::string pos = posRoot + std::to_string(i);
           (evdevEventFD)  = open(pos.c_str(), O_RDONLY); // | O_NONBLOCK);
           std::cout << pos << std::endl;
           if (libevdev_new_from_fd((evdevEventFD), &dev) < 0) {
               std::cout << "init failed" << std::endl;
               close((evdevEventFD));
               continue;
           }
           if (libevdev_has_event_code(dev, EV_REL, REL_X) > 0) {
               break;
           } else {
               std::cout << "not a mouse" << std::endl;
               libevdev_free(dev);
               dev = nullptr;
               close(evdevEventFD);
           }
       }
       input_event ev;
   
       while (true) {
           int rc = libevdev_next_event(dev, LIBEVDEV_READ_FLAG_BLOCKING, &ev);
           if (rc == LIBEVDEV_READ_STATUS_SUCCESS) {
               auto code  = ev.code;
               auto time  = ev.time;
               auto type  = ev.type;
               auto value = ev.value;
               std::cout << "evdev IO Result: " << std::endl;
               std::cout << "code: " << code << std::endl;
               std::cout << "time: " << time.tv_usec << std::endl;
               std::cout << "type: " << type << std::endl;
               std::cout << "value: " << value << std::endl;
               std::cout << "WAKEUP CALL" << std::endl;
           } else {
   
               std::cout << "Failed" << rc << std::endl;
           }
       }
       libevdev_free(dev);
   
       close(evdevEventFD);
       return 0;
   }
