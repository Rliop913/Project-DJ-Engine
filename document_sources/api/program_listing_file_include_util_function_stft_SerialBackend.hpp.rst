
.. _program_listing_file_include_util_function_stft_SerialBackend.hpp:

Program Listing for File SerialBackend.hpp
==========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_util_function_stft_SerialBackend.hpp>` (``include\util\function\stft\SerialBackend.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include "STFT_Parallel.hpp"
   
   #include <cstdint>
   #include <vector>
   
   namespace PDJE_PARALLEL {
   
   class SERIAL_STFT final : public IStftBackend {
     private:
       static constexpr uint32_t kMelBins          = 80;
       static constexpr int      kDefaultSampleRate = 48000;
   
       uint32_t prev_overlap_fullsize           = 0;
       uint32_t prev_overlap_subbuffer_fullsize = 0;
       uint32_t prev_bin_fullsize               = 0;
       uint32_t prev_mel_fullsize               = 0;
       int      prev_fft_size                   = 0;
   
       std::vector<float> real;
       std::vector<float> imag;
       std::vector<float> subreal;
       std::vector<float> subimag;
       std::vector<float> bin_real;
       std::vector<float> bin_imag;
       std::vector<float> mel;
       std::vector<float> rgb;
       std::vector<float> mel_filter_bank;
   
       void
       EnsureMemory(const StftArgs &gargs,
                    const POST_PROCESS &post_process,
                    bool needSubBuffer);
   
       void
       EnsureMelFilterBank(int windowSize);
   
       void
       ApplyWindow(WINDOW_LIST target_window, const StftArgs &gargs);
   
       void
       RunFft(unsigned int windowSizeEXP, const StftArgs &gargs);
   
     public:
       StftResult
       Execute(std::vector<float> &PCMdata,
               WINDOW_LIST         target_window,
               POST_PROCESS        post_process,
               unsigned int        windowSizeEXP,
               const StftArgs     &gargs) override;
   
       ~SERIAL_STFT() override;
   };
   
   } // namespace PDJE_PARALLEL
