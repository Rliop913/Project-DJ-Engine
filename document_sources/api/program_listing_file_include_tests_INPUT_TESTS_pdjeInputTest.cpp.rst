
.. _program_listing_file_include_tests_INPUT_TESTS_pdjeInputTest.cpp:

Program Listing for File pdjeInputTest.cpp
==========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_tests_INPUT_TESTS_pdjeInputTest.cpp>` (``include/tests/INPUT_TESTS/pdjeInputTest.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "Input_State.hpp"
   #include "PDJE_Input.hpp"
   #include "PDJE_Input_Device_Data.hpp"
   #include <iostream>
   #include <thread>
   #include <vector>
   
   int
   main()
   {
       PDJE_Input pip;
       if (!pip.Init()) {
           std::cerr << "Init failed\n";
           return 1;
       }
   
       auto     devs = pip.GetDevs();
       DEV_LIST set_targets;
       for (const auto &i : devs) {
           std::cout << "name: " << i.Name << std::endl;
           switch (i.Type) {
           case PDJE_Dev_Type::MOUSE:
               std::cout << "type: mouse" << std::endl;
               set_targets.push_back(i);
               break;
           case PDJE_Dev_Type::KEYBOARD:
               std::cout << "type: keyboard" << std::endl;
               set_targets.push_back(i);
               break;
           case PDJE_Dev_Type::UNKNOWN:
               std::cout << "type: unknown" << std::endl;
               break;
           default:
               break;
           }
           std::cout << "dev path: " << i.device_specific_id << std::endl;
       }
   
       if (!pip.Config(set_targets, std::vector<libremidi::input_port>())) {
           std::cerr << "Config failed\n";
           pip.Kill();
           return 2;
       }
   
       auto dline = pip.PullOutDataLine();
       if (!pip.Run()) {
           std::cerr << "Run failed\n";
           pip.Kill();
           return 3;
       }
       if (!dline.input_arena) {
           std::cerr << "Input arena is null\n";
           pip.Kill();
           return 4;
       }
   
       int         times = 100;
       std::thread watcher([&]() {
           while (true) {
               try {
                   dline.input_arena->Receive();
   
                   const auto got = dline.input_arena->datas;
                   for (const auto &idx : got) {
                       std::cout << "time: " << idx.microSecond << std::endl;
                       std::cout << "id: " << idx.id << std::endl;
                       std::cout << "name: " << idx.name << std::endl;
   
                       if (idx.type == PDJE_Dev_Type::KEYBOARD) {
                           std::cout << "keyNumber: "
                                     << static_cast<int>(idx.event.keyboard.k)
                                     << std::endl;
                           std::cout << "pressed" << idx.event.keyboard.pressed
                                     << std::endl;
                       } else if (idx.type == PDJE_Dev_Type::MOUSE) {
                           std::cout << "keyNumber: "
                                     << static_cast<int>(idx.event.mouse.axis_type)
                                     << std::endl;
                           std::cout << "pressed" << idx.event.mouse.x << ", "
                                     << idx.event.mouse.y << std::endl;
                       }
   
                       times--;
                       if (times < 0) {
                           return;
                       }
                   }
               } catch (const std::exception &e) {
                   std::cout << e.what() << std::endl;
               }
           }
       });
   
       watcher.join();
       pip.Kill();
       return 0;
   }
