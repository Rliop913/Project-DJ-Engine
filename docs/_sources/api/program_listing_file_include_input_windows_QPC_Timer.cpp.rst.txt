
.. _program_listing_file_include_input_windows_QPC_Timer.cpp:

Program Listing for File QPC_Timer.cpp
======================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_input_windows_QPC_Timer.cpp>` (``include/input/windows/QPC_Timer.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "QPC_Timer.hpp"
   
   
   QPC_Timer::QPC_Timer()
   {
       QueryPerformanceFrequency(&temp_int);
       qpc_freq = static_cast<uint64_t>(temp_int.QuadPart);
   }
   
   
   uint64_t
   QPC_Timer::now()
   {
       QueryPerformanceCounter(&temp_int);
       return static_cast<uint64_t>(temp_int.QuadPart);
   }
   
   
   double
   QPC_Timer::to_second(uint64_t tick)
   {
       return 
           static_cast<double>(tick / qpc_freq) + 
               static_cast<double>(tick % qpc_freq) / static_cast<double>(qpc_freq);
   }
   
   
   double
   QPC_Timer::to_ms(uint64_t tick)
   {
       return 
           (static_cast<double>(tick / qpc_freq) * 1000.0) + 
               (static_cast<double>(tick % qpc_freq) * 1000.0) / static_cast<double>(qpc_freq);
   }
   
   uint64_t
   QPC_Timer::to_micro(uint64_t tick)
   {
       return 
       static_cast<uint64_t>(tick / qpc_freq) * static_cast<uint64_t>(1000000) +
       static_cast<uint64_t>(tick % qpc_freq) * static_cast<uint64_t>(1000000) / qpc_freq;
   }
