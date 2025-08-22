
.. _program_listing_file_include_audioRender_MixMachine_DeckData.hpp:

Program Listing for File DeckData.hpp
=====================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_audioRender_MixMachine_DeckData.hpp>` (``include\audioRender\MixMachine\DeckData.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   
   #pragma once
   
   #include <vector>
   #include <string>
   #include <functional>
   #include "PDJE_EXPORT_SETTER.hpp"
   
   using STR = std::string;
   
   using SETTER = std::function<void(double)>;
   
   using ARGSETTER = std::unordered_map<std::string, SETTER>;
   
   struct Position{
       unsigned long frame_in;  
       unsigned long frame_out; 
   };
   
   class Meta{
   public:
       void declare(std::string, std::string){
           ;//nothing. this is for faust
       }
   };
   
   class UI{
   public:
       void openVerticalBox(std::string){
           ;//nothing. this is for faust
       }
       void closeBox(){
           ;//nothing. this is for faust
       }
   };
