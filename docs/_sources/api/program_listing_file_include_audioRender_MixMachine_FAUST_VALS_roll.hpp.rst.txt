
.. _program_listing_file_include_audioRender_MixMachine_FAUST_VALS_roll.hpp:

Program Listing for File roll.hpp
=================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_audioRender_MixMachine_FAUST_VALS_roll.hpp>` (``include/audioRender/MixMachine/FAUST_VALS/roll.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   #include "Faust_interpolate.hpp"
   
   class Roll_PDJE : public FaustInterpolate {
     public:
       float RollBpm;
       int   RollSwitch;
   
       template <typename Duck>
       void
       copyDatas(Duck &dest)
       {
           dest.RollBpm    = RollBpm;
           dest.RollSwitch = RollSwitch;
       }
   
       template <typename Duck>
       void
       copySetting(Duck &dest)
       {
           copyDatas(dest);
           copyInterpolates(dest);
       }
   };
