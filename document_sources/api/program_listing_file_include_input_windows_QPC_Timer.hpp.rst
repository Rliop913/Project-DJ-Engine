
.. _program_listing_file_include_input_windows_QPC_Timer.hpp:

Program Listing for File QPC_Timer.hpp
======================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_input_windows_QPC_Timer.hpp>` (``include/input/windows/QPC_Timer.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include <windows.h>
   #include <cstdint>
   
   class QPC_Timer {
   private:
       uint64_t qpc_freq;
       LARGE_INTEGER temp_int;
   
   
   public:
       uint64_t now();
       double to_second(uint64_t tick);
       double to_ms(uint64_t tick);
       uint64_t to_micro(uint64_t tick);
       QPC_Timer();
       ~QPC_Timer() = default;
   };
   
