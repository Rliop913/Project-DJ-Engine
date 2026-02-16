
.. _program_listing_file_include_core_audioRender_MixMachine_FAUST_VALS_filter.hpp:

Program Listing for File filter.hpp
===================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_core_audioRender_MixMachine_FAUST_VALS_filter.hpp>` (``include/core/audioRender/MixMachine/FAUST_VALS/filter.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   #include "Faust_interpolate.hpp"
   
   class Filter_PDJE : public FaustInterpolate {
     public:
       int HLswitch;
   
       template <typename Duck>
       void
       copyDatas(Duck &dest)
       {
           dest.HLswitch = HLswitch;
       }
   
       template <typename Duck>
       void
       copySetting(Duck &dest)
       {
           copyDatas(dest);
           copyInterpolates(dest);
       }
   };
