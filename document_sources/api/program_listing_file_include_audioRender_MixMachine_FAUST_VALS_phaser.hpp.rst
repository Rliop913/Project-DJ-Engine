
.. _program_listing_file_include_audioRender_MixMachine_FAUST_VALS_phaser.hpp:

Program Listing for File phaser.hpp
===================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_audioRender_MixMachine_FAUST_VALS_phaser.hpp>` (``include\audioRender\MixMachine\FAUST_VALS\phaser.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   #include "Faust_interpolate.hpp"
   
   class Phaser_PDJE : public FaustInterpolate {
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
