
.. _program_listing_file_include_util_common_BackendLoader_PDJE_Parallel_Runtime_Loader.cpp:

Program Listing for File PDJE_Parallel_Runtime_Loader.cpp
=========================================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_util_common_BackendLoader_PDJE_Parallel_Runtime_Loader.cpp>` (``include\util\common\BackendLoader\PDJE_Parallel_Runtime_Loader.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "PDJE_Parallel_Runtime_Loader.hpp"
   #include "OpenCL_Loader.hpp"
   
   namespace PDJE_PARALLEL {
   
   Backend::Backend() : backend_now(BACKEND_T::SERIAL)
   {
   }
   
   Backend::~Backend() = default;
   
   BACKEND_T
   Backend::PrintBackendType()
   {
       return backend_now;
   }
   
   bool
   Backend::LoadBackend()
   {
       backend_now =
           EnsureOpenCLRuntimeLoaded() ? BACKEND_T::OPENCL : BACKEND_T::SERIAL;
       return true;
   }
   
   }; // namespace PDJE_PARALLEL
