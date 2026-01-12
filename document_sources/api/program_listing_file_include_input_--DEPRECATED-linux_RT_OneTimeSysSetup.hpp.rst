
.. _program_listing_file_include_input_--DEPRECATED-linux_RT_OneTimeSysSetup.hpp:

Program Listing for File OneTimeSysSetup.hpp
============================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_input_--DEPRECATED-linux_RT_OneTimeSysSetup.hpp>` (``include\input\--DEPRECATED-linux\RT\OneTimeSysSetup.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   #include "Common_Features.hpp"
   #include <numa.h>
   #include <numaif.h>
   #include <sched.h>
   #include <sys/mman.h>
   class OneTimeSysSetup {
     private:
       int
       CoreValid(int core_number);
       void
       FixCPU(int core_number = 2);
       void
       MLock();
   
     public:
       OneTimeSysSetup()
       {
           FixCPU();
           MLock();
       }
       ~OneTimeSysSetup()
       {
           munlockall();
       }
   };
