
.. _program_listing_file_include_audioRender_ManualMix_ManualFausts_robotMan.hpp:

Program Listing for File robotMan.hpp
=====================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_audioRender_ManualMix_ManualFausts_robotMan.hpp>` (``include/audioRender/ManualMix/ManualFausts/robotMan.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   #include "DeckData.hpp"
   
   class RobotMan{
   public:
       int robotFreq;
       float RobotDryWet;
       ARGSETTER
       makeArgSetter(){
           return {
               {"RobotFreq", [this](double value) {
                    this->robotFreq = static_cast<int>(value);
               }},
               {"RobotDryWet", [this](double value) {
                    this->RobotDryWet = static_cast<float>(value);
               }}
           };
       }
   };
