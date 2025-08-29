
.. _program_listing_file_include_tests_highway_Test.cpp:

Program Listing for File highway_Test.cpp
=========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_tests_highway_Test.cpp>` (``include\tests\highway_Test.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include <hwy/highway.h>
   // #include <hwy/aligned_allocator.h>
   #include <chrono>
   #include <vector>
   
   #include <iostream>
   // HWY_BEFORE_NAMESPACE();
   // namespace HWY_NAMESPACE {
   
   namespace hn = hwy::HWY_NAMESPACE;
   #define SIZE_V 16 * 10000000
   
   #include <thread>
   int
   main()
   {
       std::vector<float> normal(SIZE_V);
       // std::vector<float, hwy::AlignedAllocator<float>> hwy_vec(SIZE_V);
       std::vector<float> hwy_vec(SIZE_V);
       auto               normal_time = clock();
       for (int i = 0; i < normal.size(); ++i) {
           normal[i] += 10;
       }
       auto                         normal_result = clock() - normal_time;
       const hn::ScalableTag<float> d;
   
       auto laneSz   = hn::Lanes(d);
       auto times    = hwy_vec.size() / laneSz;
       auto remained = hwy_vec.size() % laneSz;
   
       auto optr = hwy_vec.data();
   
       auto Va = hn::Set(d, 10.0f);
       // std::vector<std::thread> ThrdP;
       // for(int i=0; i<times; ++i){
       //     ThrdP.emplace_back([optr, laneSz, Va, d](const int idx){
       //         auto dptr = optr + (idx * laneSz);
       //         auto added = hn::Add(hn::Load(d, dptr), Va);
       //         hn::Store(added, d, dptr);
       //     }, i);
       // }
       auto        half = times / 2;
       std::thread thr([half, d, optr, Va, laneSz]() {
           auto dptr = optr;
           for (unsigned int i = 0; i < half; ++i) {
               // auto dptr = optr + i * laneSz;
               // auto hwyV = hn::Load(d, optr);
               auto added = hn::Add(hn::Load(d, dptr), Va);
               hn::Store(added, d, dptr);
               dptr += laneSz;
           }
       });
       auto        ddptr = optr;
       ddptr += (laneSz * half);
       auto hwy_time = clock();
       for (unsigned int i = half; i < times; ++i) {
           // auto dptr = optr + i * laneSz;
           // auto hwyV = hn::Load(d, optr);
           auto added = hn::Add(hn::Load(d, ddptr), Va);
           hn::Store(added, d, ddptr);
           ddptr += laneSz;
       }
       thr.join();
       // for(auto& i : ThrdP){
       //     i.join();
       // }
       auto hwy_result = clock() - hwy_time;
       std::cout << laneSz << "normal: " << normal_result
                 << " hwy time: " << hwy_result << std::endl;
   
       return 0;
   }
   // }
   // HWY_AFTER_NAMESPACE();
