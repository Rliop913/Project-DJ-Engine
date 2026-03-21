
.. _program_listing_file_include_global_PDJE_CAbi_Core_Private.hpp:

Program Listing for File PDJE_CAbi_Core_Private.hpp
===================================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_global_PDJE_CAbi_Core_Private.hpp>` (``include\global\PDJE_CAbi_Core_Private.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #ifdef __cplusplus
   
   struct PDJE_EngineHandleV1 {
       void               *engine       = nullptr;
       unsigned long long *now_cursor    = nullptr;
       unsigned long long *max_cursor    = nullptr;
       float              *pre_rendered = nullptr;
       void               *sync_data    = nullptr;
   };
   
   namespace PDJE_CABI {
   
   inline const PDJE_EngineHandleV1 *
   BorrowCoreDataLine(const PDJE_EngineHandleV1 *engine) noexcept
   {
       return engine;
   }
   
   } // namespace PDJE_CABI
   
   #endif
