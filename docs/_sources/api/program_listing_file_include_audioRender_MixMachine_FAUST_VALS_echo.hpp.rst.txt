
.. _program_listing_file_include_audioRender_MixMachine_FAUST_VALS_echo.hpp:

Program Listing for File echo.hpp
=================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_audioRender_MixMachine_FAUST_VALS_echo.hpp>` (``include/audioRender/MixMachine/FAUST_VALS/echo.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   #include "Faust_interpolate.hpp"
   
   class Echo_PDJE : public FaustInterpolate {
     public:
       float EchoBps;
       float EchoFeedback;
   
       template <typename Duck>
       void
       copyDatas(Duck &dest)
       {
           dest.EchoBps      = EchoBps;
           dest.EchoFeedback = EchoFeedback;
       }
   
       template <typename Duck>
       void
       copySetting(Duck &dest)
       {
           copyDatas(dest);
           copyInterpolates(dest);
       }
   };
