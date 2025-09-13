
.. _program_listing_file_include_core_audioRender_MixMachine_FAUST_VALS_ocsFilter.hpp:

Program Listing for File ocsFilter.hpp
======================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_core_audioRender_MixMachine_FAUST_VALS_ocsFilter.hpp>` (``include/core/audioRender/MixMachine/FAUST_VALS/ocsFilter.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   #include "Faust_interpolate.hpp"
   
   class OcsFilter_PDJE : public FaustInterpolate {
     public:
       int   ocsFilterHighLowSW;
       int   middleFreq;
       int   rangeFreqHalf;
       float bps;
   
       template <typename Duck>
       void
       copyDatas(Duck &dest)
       {
           dest.ocsFilterHighLowSW = ocsFilterHighLowSW;
           dest.middleFreq         = middleFreq;
           dest.rangeFreqHalf      = rangeFreqHalf;
           dest.bps                = bps;
       }
   
       template <typename Duck>
       void
       copySetting(Duck &dest)
       {
           copyDatas(dest);
           copyInterpolates(dest);
       }
   };
