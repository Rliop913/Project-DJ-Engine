
.. _program_listing_file_include_core_audioRender_ManualMix_ManualFausts_compressorMan.hpp:

Program Listing for File compressorMan.hpp
==========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_core_audioRender_ManualMix_ManualFausts_compressorMan.hpp>` (``include/core/audioRender/ManualMix/ManualFausts/compressorMan.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   
   #pragma once
   #include "DeckData.hpp"
   
   class CompressorMan {
     public:
       float strength;  
       int   threshDB;  
       int   attackMS;  
       int   releaseMS; 
       int   kneeDB;    
   
       ARGSETTER
       makeArgSetter()
       {
           return { { "Strength",
                      [this](double value) {
                          this->strength = static_cast<float>(value);
                      } },
                    { "ThreshDB",
                      [this](double value) {
                          this->threshDB = static_cast<int>(value);
                      } },
                    { "AttackMS",
                      [this](double value) {
                          this->attackMS = static_cast<int>(value);
                      } },
                    { "ReleaseMS",
                      [this](double value) {
                          this->releaseMS = static_cast<int>(value);
                      } },
                    { "KneeDB", [this](double value) {
                         this->kneeDB = static_cast<int>(value);
                     } } };
       }
   };
