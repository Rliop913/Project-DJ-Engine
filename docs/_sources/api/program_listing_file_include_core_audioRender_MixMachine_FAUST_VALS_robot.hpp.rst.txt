
.. _program_listing_file_include_core_audioRender_MixMachine_FAUST_VALS_robot.hpp:

Program Listing for File robot.hpp
==================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_core_audioRender_MixMachine_FAUST_VALS_robot.hpp>` (``include/core/audioRender/MixMachine/FAUST_VALS/robot.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   #include "Faust_interpolate.hpp"
   
   class Robot_PDJE : public FaustInterpolate {
     public:
       int robotFreq;
   
       template <typename Duck>
       void
       copyDatas(Duck &dest)
       {
           dest.robotFreq = robotFreq;
       }
   
       template <typename Duck>
       void
       copySetting(Duck &dest)
       {
           copyDatas(dest);
           copyInterpolates(dest);
       }
   };
