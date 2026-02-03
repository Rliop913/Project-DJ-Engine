
.. _program_listing_file_include_global_Highres_Clock_Windows_PDJE_Highres_Clock.hpp:

Program Listing for File PDJE_Highres_Clock.hpp
===============================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_global_Highres_Clock_Windows_PDJE_Highres_Clock.hpp>` (``include/global/Highres_Clock/Windows/PDJE_Highres_Clock.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include "PDJE_EXPORT_SETTER.hpp"
   #include <cstdint>
   #include <windows.h>
   
   #pragma once
   #include <cstdint>
   #include <time.h>
   namespace PDJE_HIGHRES_CLOCK {
   
   class PDJE_API CLOCK {
     private:
       struct timespec ts;
       uint64_t        qpc_freq;
       LARGE_INTEGER   temp_int;
   
     public:
       CLOCK()
       {
   
           QueryPerformanceFrequency(&temp_int);
           qpc_freq = static_cast<uint64_t>(temp_int.QuadPart);
       }
       uint64_t
       Get_MicroSecond()
       {
           QueryPerformanceCounter(&temp_int);
           return static_cast<uint64_t>(static_cast<uint64_t>(temp_int.QuadPart) /
                                        qpc_freq) *
                      static_cast<uint64_t>(1000000) +
                  static_cast<uint64_t>(static_cast<uint64_t>(temp_int.QuadPart) %
                                        qpc_freq) *
                      static_cast<uint64_t>(1000000) / qpc_freq;
       }
       ~CLOCK() = default;
   };
   }; // namespace PDJE_HIGHRES_CLOCK
