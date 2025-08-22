
.. _program_listing_file_include_interface_PDJE_Core_DataLine.hpp:

Program Listing for File PDJE_Core_DataLine.hpp
===============================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_interface_PDJE_Core_DataLine.hpp>` (``include/interface/PDJE_Core_DataLine.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include "PDJE_EXPORT_SETTER.hpp"
   
   #include "MusicControlPannel.hpp"
   struct PDJE_API PDJE_CORE_DATA_LINE{
       unsigned long long* used_frame = nullptr;
       unsigned long long* nowCursor = nullptr;
       unsigned long long* maxCursor = nullptr;
       float* preRenderedData = nullptr;
   
       FXControlPannel* fx = nullptr;
       MusicControlPannel* musp = nullptr;
   };
