
.. _program_listing_file_include_global_PDJE_ATOMIC_EVENT.hpp:

Program Listing for File PDJE_ATOMIC_EVENT.hpp
==============================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_global_PDJE_ATOMIC_EVENT.hpp>` (``include/global/PDJE_ATOMIC_EVENT.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include "PDJE_EXPORT_SETTER.hpp"
   #include <atomic>
   #include <thread>
   class PDJE_API ATOMIC_EVENT {
     private:
       std::atomic<bool> flag{ false };
   
     public:
       void
       wait()
       {
           while (!flag.load(std::memory_order_acquire)) {
               flag.wait(false, std::memory_order_acquire);
           }
           flag.store(false, std::memory_order_release);
       }
       void
       signal()
       {
           flag.store(true, std::memory_order_release);
           flag.notify_one();
       }
   };
