
.. _program_listing_file_include_audioRender_MixMachine_FAUST_VALS_compressor.hpp:

Program Listing for File compressor.hpp
=======================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_audioRender_MixMachine_FAUST_VALS_compressor.hpp>` (``include/audioRender/MixMachine/FAUST_VALS/compressor.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   #include "Faust_interpolate.hpp"
   
   class Compressor_PDJE {
     public:
       float strength;
       int   threshDB;
       int   attackMS;
       int   releaseMS;
       int   kneeDB;
   
       template <typename Duck>
       void
       copyDatas(Duck &dest)
       {
           dest.strength  = strength;
           dest.threshDB  = threshDB;
           dest.attackMS  = attackMS;
           dest.releaseMS = releaseMS;
           dest.kneeDB    = kneeDB;
       }
   
       template <typename Duck>
       void
       copySetting(Duck &dest)
       {
           copyDatas(dest);
       }
   };
