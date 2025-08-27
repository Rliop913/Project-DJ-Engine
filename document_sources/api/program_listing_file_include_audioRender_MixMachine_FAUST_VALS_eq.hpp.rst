
.. _program_listing_file_include_audioRender_MixMachine_FAUST_VALS_eq.hpp:

Program Listing for File eq.hpp
===============================

|exhale_lsh| :ref:`Return to documentation for file <file_include_audioRender_MixMachine_FAUST_VALS_eq.hpp>` (``include\audioRender\MixMachine\FAUST_VALS\eq.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   #include "Faust_interpolate.hpp"
   
   class EQ_PDJE: public FaustInterpolate {
   public:
       int EQSelect;
   
       template<typename Duck>
       void copyDatas(Duck& dest){
           dest.EQSelect = EQSelect;
       }
   
       template<typename Duck>
       void copySetting(Duck& dest){
           copyDatas(dest);
           copyInterpolates(dest);
       }
   };
