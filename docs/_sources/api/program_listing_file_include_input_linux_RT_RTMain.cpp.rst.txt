
.. _program_listing_file_include_input_linux_RT_RTMain.cpp:

Program Listing for File RTMain.cpp
===================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_input_linux_RT_RTMain.cpp>` (``include/input/linux/RT/RTMain.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "RTSocket.hpp"
   #include <exception>
   #include <iostream>
   #include <memory>
   int
   main(int argc, char **argv)
   {
       if (argc < 2) {
           return -1;
       }
       std::cout << "client on" << std::endl;
       std::unique_ptr<RTSocket> rs;
       RTEvent                   rtev;
       try {
           rs = std::make_unique<RTSocket>(argv[1], &rtev);
       } catch (const std::exception &e) {
           std::cerr << e.what() << std::endl;
           std::cout << "from RT" << std::endl;
           return -2;
       }
   
       std::cout << "in communication" << std::endl;
       rs->Communication();
       std::cout << "trig loop" << std::endl;
       rtev.Trig();
       std::cout << "end client" << std::endl;
       // run
       return 0;
   }
