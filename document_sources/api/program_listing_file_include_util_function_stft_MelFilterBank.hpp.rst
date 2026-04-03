
.. _program_listing_file_include_util_function_stft_MelFilterBank.hpp:

Program Listing for File MelFilterBank.hpp
==========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_util_function_stft_MelFilterBank.hpp>` (``include\util\function\stft\MelFilterBank.hpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #pragma once
   
   #include <cmath>
   #include <limits>
   #include <vector>
   
   namespace PDJE_PARALLEL {
   
   enum class MelFormula {
       HTK = 0,
       Slaney
   };
   
   enum class MelNorm {
       None = 0,
       Peak,
       Slaney
   };
   
   namespace detail {
   
   constexpr float kSlaneyFSp = 200.0f / 3.0f;
   constexpr float kSlaneyMinLogHz = 1000.0f;
   constexpr float kSlaneyMinLogMel = kSlaneyMinLogHz / kSlaneyFSp;
   constexpr float kSlaneyLogStep = 0.06875177742094912f;
   
   static inline float
   HtkHzToMel(const float hz)
   {
       return 2595.0f * std::log10(1.0f + (hz / 700.0f));
   }
   
   static inline float
   HtkMelToHz(const float mel)
   {
       return 700.0f * (std::pow(10.0f, mel / 2595.0f) - 1.0f);
   }
   
   static inline float
   SlaneyHzToMel(const float hz)
   {
       if (hz < kSlaneyMinLogHz) {
           return hz / kSlaneyFSp;
       }
   
       return kSlaneyMinLogMel +
              (std::log(hz / kSlaneyMinLogHz) / kSlaneyLogStep);
   }
   
   static inline float
   SlaneyMelToHz(const float mel)
   {
       if (mel < kSlaneyMinLogMel) {
           return mel * kSlaneyFSp;
       }
   
       return kSlaneyMinLogHz *
              std::exp(kSlaneyLogStep * (mel - kSlaneyMinLogMel));
   }
   
   static inline float
   HzToMel(const float hz, const MelFormula mel_formula)
   {
       switch (mel_formula) {
       case MelFormula::HTK:
           return HtkHzToMel(hz);
       case MelFormula::Slaney:
           return SlaneyHzToMel(hz);
       default:
           return 0.0f;
       }
   }
   
   static inline float
   MelToHz(const float mel, const MelFormula mel_formula)
   {
       switch (mel_formula) {
       case MelFormula::HTK:
           return HtkMelToHz(mel);
       case MelFormula::Slaney:
           return SlaneyMelToHz(mel);
       default:
           return 0.0f;
       }
   }
   
   static inline void
   ApplyNorm(float       *weights,
             const int    freq_bins,
             const float  left_hz,
             const float  right_hz,
             const MelNorm norm)
   {
       if (weights == nullptr || freq_bins <= 0 || norm == MelNorm::None) {
           return;
       }
   
       if (norm == MelNorm::Peak) {
           float row_peak = 0.0f;
           for (int bin_idx = 0; bin_idx < freq_bins; ++bin_idx) {
               if (weights[bin_idx] > row_peak) {
                   row_peak = weights[bin_idx];
               }
           }
   
           if (row_peak <= 0.0f) {
               return;
           }
   
           const float inv_row_peak = 1.0f / row_peak;
           for (int bin_idx = 0; bin_idx < freq_bins; ++bin_idx) {
               weights[bin_idx] *= inv_row_peak;
           }
           return;
       }
   
       if (norm == MelNorm::Slaney) {
           const float band_width = right_hz - left_hz;
           if (band_width <= 0.0f) {
               return;
           }
   
           const float enorm = 2.0f / band_width;
           for (int bin_idx = 0; bin_idx < freq_bins; ++bin_idx) {
               weights[bin_idx] *= enorm;
           }
       }
   }
   
   } // namespace detail
   
   static inline bool
   CheckMelVals(const int        sample_rate,
                const int        n_fft,
                const int        n_mels,
                const float      f_min,
                const float      f_max,
                const MelFormula mel_formula,
                const MelNorm    norm)
   {
       if (sample_rate <= 0 || n_fft <= 0 || n_mels <= 0 || f_min < 0.0f) {
           return false;
       }
   
       if (n_mels > (std::numeric_limits<int>::max() - 2)) {
           return false;
       }
   
       const float nyquist = static_cast<float>(sample_rate) * 0.5f;
       if (f_max <= f_min || f_max > nyquist) {
           return false;
       }
   
       const int freq_bins = (n_fft / 2) + 1;
       if (freq_bins <= 0) {
           return false;
       }
   
       const std::size_t mel_count = static_cast<std::size_t>(n_mels);
       const std::size_t bin_count = static_cast<std::size_t>(freq_bins);
       if (mel_count > (std::numeric_limits<std::size_t>::max() / bin_count)) {
           return false;
       }
   
       switch (mel_formula) {
       case MelFormula::HTK:
       case MelFormula::Slaney:
           break;
       default:
           return false;
       }
   
       switch (norm) {
       case MelNorm::None:
       case MelNorm::Peak:
       case MelNorm::Slaney:
           return true;
       default:
           return false;
       }
   }
   
   static inline std::vector<float>
   GenMelFilterBank(const int   sample_rate,
                    const int   n_fft,
                    const int   n_mels,
                    const float f_min = 0.0f,
                    float       f_max = -1.0f,
                    MelFormula  mel_formula = MelFormula::HTK,
                    MelNorm     norm = MelNorm::None)
   {
       const float nyquist = static_cast<float>(sample_rate) * 0.5f;
       if (f_max < 0.0f) {
           f_max = nyquist;
       }
   
       if (!CheckMelVals(sample_rate, n_fft, n_mels, f_min, f_max, mel_formula, norm)) {
           return {};
       }
   
       const int freq_bins = (n_fft / 2) + 1;
       if (freq_bins <= 0) {
           return {};
       }
   
       std::vector<float> filter_bank(
           static_cast<std::size_t>(n_mels) * static_cast<std::size_t>(freq_bins),
           0.0f);
   
       const float mel_min = detail::HzToMel(f_min, mel_formula);
       const float mel_max = detail::HzToMel(f_max, mel_formula);
       const float mel_step =
           (mel_max - mel_min) / static_cast<float>(n_mels + 1);
   
       std::vector<float> hz_points(static_cast<std::size_t>(n_mels) + 2u, 0.0f);
       for (int point_idx = 0; point_idx < n_mels + 2; ++point_idx) {
           const float mel_value =
               mel_min + (static_cast<float>(point_idx) * mel_step);
           hz_points[static_cast<std::size_t>(point_idx)] =
               detail::MelToHz(mel_value, mel_formula);
       }
   
       const float fft_scale =
           static_cast<float>(sample_rate) / static_cast<float>(n_fft);
   
       for (int mel_idx = 0; mel_idx < n_mels; ++mel_idx) {
           const float left_hz =
               hz_points[static_cast<std::size_t>(mel_idx)];
           const float center_hz =
               hz_points[static_cast<std::size_t>(mel_idx) + 1u];
           const float right_hz =
               hz_points[static_cast<std::size_t>(mel_idx) + 2u];
   
           const float left_width = center_hz - left_hz;
           const float right_width = right_hz - center_hz;
   
           for (int bin_idx = 0; bin_idx < freq_bins; ++bin_idx) {
               const float bin_hz = static_cast<float>(bin_idx) * fft_scale;
               float       weight = 0.0f;
   
               if (bin_hz >= left_hz && bin_hz <= center_hz && left_width > 0.0f) {
                   weight = (bin_hz - left_hz) / left_width;
               } else if (bin_hz >= center_hz && bin_hz <= right_hz &&
                          right_width > 0.0f) {
                   weight = (right_hz - bin_hz) / right_width;
               }
   
               filter_bank[static_cast<std::size_t>(mel_idx) *
                               static_cast<std::size_t>(freq_bins) +
                           static_cast<std::size_t>(bin_idx)] = weight;
           }
   
           detail::ApplyNorm(
               filter_bank.data() +
                   (static_cast<std::size_t>(mel_idx) *
                    static_cast<std::size_t>(freq_bins)),
               freq_bins,
               left_hz,
               right_hz,
               norm);
       }
   
       return filter_bank;
   }
   
   } // namespace PDJE_PARALLEL
