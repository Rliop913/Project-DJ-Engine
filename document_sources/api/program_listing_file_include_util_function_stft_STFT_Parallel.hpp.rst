
.. _program_listing_file_include_util_function_stft_STFT_Parallel.hpp:

Program Listing for File STFT_Parallel.hpp
==========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_util_function_stft_STFT_Parallel.hpp>` (``include\util\function\stft\STFT_Parallel.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include "PDJE_Parallel_Runtime_Loader.hpp"
   #include "Parallel_Args.hpp"
   #include "STFT_args.hpp"
   
   #include <memory>
   #include <utility>
   #include <vector>
   
   namespace PDJE_PARALLEL {
   
   class IStftBackend;
   class OPENCL_STFT;
   class SERIAL_STFT;
   
   inline int
   toQuot(const unsigned int fullSize,
          const float        overlapRatio,
          const int          windowSize)
   {
       if (overlapRatio == 0.0f) {
           return static_cast<int>(fullSize / static_cast<unsigned int>(windowSize)) +
                  1;
       }
   
       const float stepSize = static_cast<float>(windowSize) * (1.0f - overlapRatio);
       return static_cast<int>(static_cast<float>(fullSize) / stepSize) + 1;
   }
   
   enum WINDOW_LIST {
       BLACKMAN,
       BLACKMAN_HARRIS,
       BLACKMAN_NUTTALL,
       HANNING,
       NUTTALL,
       FLATTOP,
       GAUSSIAN,
       HAMMING,
       NONE
   };
   
   struct POST_PROCESS {
       bool to_bin            = false;
       bool toPower           = false;
       bool mel_scale         = false;
       bool to_db             = false;
       bool normalize_min_max = false;
       bool to_rgb            = false;
   
       void
       check_values()
       {
           if (to_rgb) {
               normalize_min_max = true;
               mel_scale         = true;
           }
           if (mel_scale) {
               to_bin   = true;
               toPower  = true;
           }
       }
   
       bool
       Chainable_BIN_POWER() const
       {
           return to_bin && toPower;
       }
   
       bool
       Chainable_MEL_DB() const
       {
           return mel_scale && to_db;
       }
   };
   
   using StftResult = std::pair<std::vector<float>, std::vector<float>>;
   
   class IStftBackend {
     public:
       virtual ~IStftBackend() = default;
   
       virtual StftResult
       Execute(std::vector<float> &PCMdata,
               WINDOW_LIST         target_window,
               POST_PROCESS        post_process,
               unsigned int        windowSizeEXP,
               const StftArgs     &gargs) = 0;
   };
   
   class STFT {
     private:
       StftArgs
       GenArgs(const std::vector<float> &inputVec,
               int                       windowSizeEXP,
               float                     overlapRatio);
   
       std::unique_ptr<IStftBackend> serial_backend;
       std::unique_ptr<IStftBackend> opencl_backend;
   
     public:
       Backend   backendinfo;
       BACKEND_T backend_now = BACKEND_T::SERIAL;
   
       STFT();
   
       void
       SetBackendForTesting(BACKEND_T backend_type,
                            std::unique_ptr<IStftBackend> backend);
   
       StftResult
       calculate(std::vector<float> &PCMdata,
                 WINDOW_LIST         target_window = WINDOW_LIST::HANNING,
                 int                 windowSizeEXP = 10,
                 float               overlapRatio  = 0.5f,
                 POST_PROCESS        post_process  = POST_PROCESS());
   
       ~STFT();
   };
   
   } // namespace PDJE_PARALLEL
