
.. _program_listing_file_include_audioRender_MixMachine_FAUST_VALS_trance.hpp:

Program Listing for File trance.hpp
===================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_audioRender_MixMachine_FAUST_VALS_trance.hpp>` (``include\audioRender\MixMachine\FAUST_VALS\trance.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   #include "Faust_interpolate.hpp"
   
   class Trance_PDJE : public FaustInterpolate{
   public:
       float bps;
       float gain;
   
       template<typename Duck>
       void copyDatas(Duck& dest){
           dest.bps = bps;
           dest.gain = gain;
       }
   
       template<typename Duck>
       void copySetting(Duck& dest){
           copyDatas(dest);
           copyInterpolates(dest);
       }
   };
