
.. _program_listing_file_include_global_DataLines_PDJE_Core_DataLine.hpp:

Program Listing for File PDJE_Core_DataLine.hpp
===============================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_global_DataLines_PDJE_Core_DataLine.hpp>` (``include/global/DataLines/PDJE_Core_DataLine.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include "PDJE_EXPORT_SETTER.hpp"
   #include "PDJE_SYNC_CORE.hpp"
   #include <atomic>
   #include <cstdint>
   // #include "MusicControlPanel.hpp"
   struct PDJE_API PDJE_CORE_DATA_LINE {
       unsigned long long         *nowCursor       = nullptr;
       unsigned long long         *maxCursor       = nullptr;
       float                      *preRenderedData = nullptr;
       std::atomic<audioSyncData> *syncD           = nullptr;
   };
