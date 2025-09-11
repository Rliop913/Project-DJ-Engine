
.. _program_listing_file_core_include_audioRender_MixMachine_FAUST_VALS_flanger.hpp:

Program Listing for File flanger.hpp
====================================

|exhale_lsh| :ref:`Return to documentation for file <file_core_include_audioRender_MixMachine_FAUST_VALS_flanger.hpp>` (``core_include\audioRender\MixMachine\FAUST_VALS\flanger.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include "Faust_interpolate.hpp"
   
   class Flanger_PDJE : public FaustInterpolate {
     public:
       float bps;
   
       template <typename Duck>
       void
       copyDatas(Duck &dest)
       {
           dest.bps = bps;
       }
   
       template <typename Duck>
       void
       copySetting(Duck &dest)
       {
           copyDatas(dest);
           copyInterpolates(dest);
       }
   };
