
.. _program_listing_file_include_tests_CapnpTest.cpp:

Program Listing for File CapnpTest.cpp
======================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_tests_CapnpTest.cpp>` (``include/tests/CapnpTest.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   
   #include <hwy/highway.h>
   #include "MUSIC_CTR.hpp"
   #include <iostream>
   int main()
   {
       Ingredients ing;
       PlayPosition spp;
       spp.Gidx = 0;
       spp.Lidx = 123;
       spp.status = MIXSTATE::PLAY;
       PlayPosition Epp;
       Epp.Gidx = 4800000;
       Epp.Lidx = Epp.Gidx + spp.Lidx;
       Epp.status = MIXSTATE::END;
       ing.pos.push_back(spp);
       ing.pos.push_back(Epp);
   
       PlayPosition Cpp;
       Cpp.Gidx = 480000;
       Cpp.Lidx = 123;
       Cpp.status = MIXSTATE::PLAY;
       ing.pos.push_back(Cpp);
   
       BpmStruct Gb;
       BpmStruct Lb;
       {
           Gb.fragments.emplace_back();
           Gb.fragments.back().bar = 0;
           Gb.fragments.back().beat = 0;
           Gb.fragments.back().bpm = 60;
           Gb.fragments.back().separate = 4;
           Gb.fragments.emplace_back();
           Gb.fragments.back().bar = 4;
           Gb.fragments.back().beat = 0;
           Gb.fragments.back().bpm = 120;
           Gb.fragments.back().separate = 4;
           Gb.fragments.emplace_back();
           Gb.fragments.back().bar = 8;
           Gb.fragments.back().beat = 0;
           Gb.fragments.back().bpm = 60;
           Gb.fragments.back().separate = 4;
       }
       {
           Lb.fragments.emplace_back();
           Lb.fragments.back().bar = 0;
           Lb.fragments.back().beat = 0;
           Lb.fragments.back().bpm = 120;
           Lb.fragments.emplace_back();
           Lb.fragments.back().bar = 4;
           Lb.fragments.back().beat = 0;
           Lb.fragments.back().bpm = 60;
           Lb.fragments.emplace_back();
           Lb.fragments.back().bar = 8;
           Lb.fragments.back().beat = 0;
           Lb.fragments.back().bpm = 120;
       }
       
       Lb.sortFragment();
       Gb.sortFragment();
       Lb.calcFrame(spp.Lidx);
       Gb.calcFrame();
       ing.Ready(Gb, Lb);
   
       if(Lb.calcFrame() && Gb.calcFrame()){
   
           // auto res = ing.FirstStage(Gb, Lb);
           int temp = 0;
           // for(auto i : res){
           //     std::cout<< "Global BPM: " << i.GlobalBpm << std::endl;
           //     std::cout<< "Local BPM: " << i.LocalBpm << std::endl;
           //     std::cout<< "Global Position: " << i.Gpos << std::endl;
           //     std::cout<< "-------------------->" << temp++ << std::endl;
           // }
           return 0;
       }
       return -1;
   }
