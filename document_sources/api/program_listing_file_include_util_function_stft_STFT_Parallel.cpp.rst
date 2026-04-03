
.. _program_listing_file_include_util_function_stft_STFT_Parallel.cpp:

Program Listing for File STFT_Parallel.cpp
==========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_util_function_stft_STFT_Parallel.cpp>` (``include\util\function\stft\STFT_Parallel.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "STFT_Parallel.hpp"
   
   #include "OpenclBackend.hpp"
   #include "SerialBackend.hpp"
   
   #include <exception>
   #include <memory>
   #include <utility>
   #include <vector>
   
   namespace PDJE_PARALLEL {
   
   STFT::STFT() : serial_backend(std::make_unique<SERIAL_STFT>())
   {
       backendinfo.LoadBackend();
       backend_now = backendinfo.PrintBackendType();
   
       if (backend_now != BACKEND_T::OPENCL) {
           return;
       }
   
       try {
           opencl_backend = std::make_unique<OPENCL_STFT>();
       } catch (const std::exception &) {
           opencl_backend.reset();
           backend_now = BACKEND_T::SERIAL;
       }
   }
   
   STFT::~STFT() = default;
   
   StftResult
   STFT::calculate(std::vector<float> &PCMdata,
                   const WINDOW_LIST   target_window,
                   const int           windowSizeEXP,
                   const float         overlapRatio,
                   POST_PROCESS        post_process)
   {
       if (PCMdata.empty() || overlapRatio < 0.0f || overlapRatio >= 1.0f ||
           windowSizeEXP < 6 || windowSizeEXP >= 31) {
           return {};
       }
   
       const auto gargs = GenArgs(PCMdata, windowSizeEXP, overlapRatio);
   
       if (backend_now == BACKEND_T::OPENCL && opencl_backend) {
           try {
               auto result = opencl_backend->Execute(
                   PCMdata,
                   target_window,
                   post_process,
                   static_cast<unsigned int>(windowSizeEXP),
                   gargs);
               if (!result.first.empty() || !result.second.empty()) {
                   return result;
               }
           } catch (const std::exception &) {
           }
   
           opencl_backend.reset();
           backend_now = BACKEND_T::SERIAL;
       }
   
       if (!serial_backend) {
           return {};
       }
   
       return serial_backend->Execute(PCMdata,
                                      target_window,
                                      post_process,
                                      static_cast<unsigned int>(windowSizeEXP),
                                      gargs);
   }
   
   void
   STFT::SetBackendForTesting(const BACKEND_T                 backend_type,
                              std::unique_ptr<IStftBackend> backend)
   {
       switch (backend_type) {
       case BACKEND_T::OPENCL:
           opencl_backend = std::move(backend);
           backend_now    = BACKEND_T::OPENCL;
           break;
       case BACKEND_T::SERIAL:
           serial_backend = std::move(backend);
           backend_now    = BACKEND_T::SERIAL;
           break;
       default:
           break;
       }
   }
   
   } // namespace PDJE_PARALLEL
