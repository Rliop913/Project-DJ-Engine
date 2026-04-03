
.. _program_listing_file_include_util_common_BackendLoader_PDJE_Parallel_Runtime_Loader.hpp:

Program Listing for File PDJE_Parallel_Runtime_Loader.hpp
=========================================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_util_common_BackendLoader_PDJE_Parallel_Runtime_Loader.hpp>` (``include\util\common\BackendLoader\PDJE_Parallel_Runtime_Loader.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   #include "Parallel_Args.hpp"
   
   namespace PDJE_PARALLEL {
   
   class Backend {
     private:
       BACKEND_T backend_now;
   
     public:
       BACKEND_T
       PrintBackendType();
   
       bool
       LoadBackend();
   
       Backend();
       ~Backend();
   };
   }; // namespace PDJE_PARALLEL
