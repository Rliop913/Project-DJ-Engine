
.. _program_listing_file_include_global_Highres_Clock_Linux_PDJE_Highres_Clock.hpp:

Program Listing for File PDJE_Highres_Clock.hpp
===============================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_global_Highres_Clock_Linux_PDJE_Highres_Clock.hpp>` (``include\global\Highres_Clock\Linux\PDJE_Highres_Clock.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   #include <cstdint>
   #include <time.h>
   namespace PDJE_HIGHRES_CLOCK {
   
   class CLOCK {
     private:
       struct timespec ts;
   
     public:
       CLOCK()  = default;
       ~CLOCK() = default;
       uint64_t
       Get_MicroSecond()
       {
           clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
           return (uint64_t)ts.tv_sec * 1000000ull + ts.tv_nsec / 1000;
       }
   };
   }; // namespace PDJE_HIGHRES_CLOCK
