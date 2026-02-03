
.. _program_listing_file_include_global_PDJE_SYNC_CORE.hpp:

Program Listing for File PDJE_SYNC_CORE.hpp
===========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_global_PDJE_SYNC_CORE.hpp>` (``include/global/PDJE_SYNC_CORE.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   #include "PDJE_EXPORT_SETTER.hpp"
   #include <cstdint>
   struct PDJE_API audioSyncData {
       unsigned long long consumed_frames = 0;
       uint64_t           microsecond     = 0;
   };
