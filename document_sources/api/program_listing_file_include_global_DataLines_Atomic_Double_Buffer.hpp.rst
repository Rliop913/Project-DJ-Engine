
.. _program_listing_file_include_global_DataLines_Atomic_Double_Buffer.hpp:

Program Listing for File Atomic_Double_Buffer.hpp
=================================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_global_DataLines_Atomic_Double_Buffer.hpp>` (``include/global/DataLines/Atomic_Double_Buffer.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include <atomic>
   #include <cstddef>
   #include <vector>
   
   template <typename T> class Atomic_Double_Buffer {
     private:
       std::vector<T>   first;
       std::vector<T>   second;
       std::atomic_flag lock      = ATOMIC_FLAG_INIT;
       bool             acc_first = true;
   
     public:
       explicit Atomic_Double_Buffer(const std::size_t reserve_size)
       {
           first.reserve(reserve_size);
           second.reserve(reserve_size);
           lock.clear(std::memory_order_relaxed);
       }
       ~Atomic_Double_Buffer() = default;
   
       void
       Write(const T &data)
       {
           while (lock.test_and_set(std::memory_order_acquire)) {
           }
           if (acc_first) {
               first.push_back(data);
           } else {
               second.push_back(data);
           }
           lock.clear(std::memory_order_release);
       }
   
       std::vector<T> *
       Get()
       {
           while (lock.test_and_set(std::memory_order_acquire)) {
           }
   
           if (acc_first) {
               second.clear();
           } else {
               first.clear();
           }
           acc_first = !acc_first;
           lock.clear(std::memory_order_release);
   
           if (acc_first) {
               return &second;
           }
           return &first;
       }
   };
