
.. _program_listing_file_include_util_function_FunctionContext.hpp:

Program Listing for File FunctionContext.hpp
============================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_util_function_FunctionContext.hpp>` (``include\util\function\FunctionContext.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   namespace PDJE_UTIL::function {
   
   class CacheContext {
     public:
       CacheContext() = default;
       CacheContext(const CacheContext &) = delete;
       CacheContext &operator=(const CacheContext &) = delete;
       CacheContext(CacheContext &&) noexcept = default;
       CacheContext &operator=(CacheContext &&) noexcept = default;
       ~CacheContext() = default;
   };
   
   struct EvalOptions {
       CacheContext *cache = nullptr;
   };
   
   } // namespace PDJE_UTIL::function
