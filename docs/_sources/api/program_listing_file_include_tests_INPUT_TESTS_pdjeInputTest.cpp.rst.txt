
.. _program_listing_file_include_tests_INPUT_TESTS_pdjeInputTest.cpp:

Program Listing for File pdjeInputTest.cpp
==========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_tests_INPUT_TESTS_pdjeInputTest.cpp>` (``include/tests/INPUT_TESTS/pdjeInputTest.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "Input_State.hpp"
   #include "PDJE_Input.hpp"
   #include "PDJE_Input_Device_Data.hpp"
   #include <thread>
   // #include "linux/linux_input.hpp"
   #include <iostream>
   // #include <unistd.h>
   int
   main()
   {
       PDJE_Input pip;
       pip.Init();
       auto     devs = pip.GetDevs();
       DEV_LIST set_targets;
       for (auto i : devs) {
           std::cout << "name: " << i.Name << std::endl;
           switch (i.Type) {
           case PDJE_Dev_Type::MOUSE:
               std::cout << "type: mouse" << std::endl;
               break;
           case PDJE_Dev_Type::KEYBOARD:
               std::cout << "type: keyboard" << std::endl;
               set_targets.push_back(i);
               break;
           case PDJE_Dev_Type::HID:
               std::cout << "type: hid" << std::endl;
               break;
           case PDJE_Dev_Type::UNKNOWN:
               std::cout << "type: unknown" << std::endl;
               break;
           default:
               break;
           }
   
           std::cout << "dev path: " << i.device_specific_id << std::endl;
       }
   
       pip.Config(set_targets);
       // pip.NEXT();
       auto dline = pip.PullOutDataLine();
       pip.Run();
       // pip.NEXT();
       int         times = 100;
       std::thread watcher([&]() {
           while (true) {
               auto got = dline.input_arena->Get();
   
               for (const auto &i : *got) {
                   auto name = dline.id_name_conv->find(i.id);
                   if (name != dline.id_name_conv->end()) {
   
                       std::cout << "name: " << name->second << std::endl;
                       std::cout << "time: " << i.microSecond << std::endl;
                       if (i.type == PDJE_Dev_Type::KEYBOARD) {
   
                           std::cout << "keyNumber: "
                                     << static_cast<int>(i.event.keyboard.k)
                                     << std::endl;
                           std::cout << "pressed" << i.event.keyboard.pressed
                                     << std::endl;
                       }
   
                       times--;
                       if (times < 0) {
                           return;
                       }
                   }
               }
           }
       });
   
       watcher.join();
       pip.Kill();
   
       // OS_Input linux_oi;
       // linux_oi.SocketOpen("./PDJE_MODULE_INPUT_RTMAIN");
       // auto     devs = linux_oi.getDevices();
       // DEV_LIST toSet;
       // for (const auto &i : devs) {
       //     std::cout << i.Name << ", " << i.Type << std::endl;
       //     if (i.Type == "MOUSE") {
       //         toSet.push_back(i);
       //     }
       // }
       // std::cout << linux_oi.setDevices(toSet) << std::endl;
       // sleep(1);
       // linux_oi.EndSocketTransmission();
       // sleep(1);
       // std::cout << "end server" << std::endl;
       return 0;
   }
