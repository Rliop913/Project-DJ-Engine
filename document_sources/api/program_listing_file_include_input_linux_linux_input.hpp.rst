
.. _program_listing_file_include_input_linux_LINUX_INPUT.hpp:

Program Listing for File LINUX_INPUT.hpp
========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_input_linux_LINUX_INPUT.hpp>` (``include/input/linux/LINUX_INPUT.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   #include "PDJE_INPUT.hpp"
   #include <libevdev-1.0/libevdev/libevdev.h>
   
   class LinuxEVDEV {
     public:
       void
       getList();
   
       void
       init();
   };
