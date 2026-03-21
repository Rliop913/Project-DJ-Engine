
.. _program_listing_file_include_util_function_image_WaveformWebp.cpp:

Program Listing for File WaveformWebp.cpp
=========================================

|exhale_lsh| :ref:`Return to documentation for file <file_include_util_function_image_WaveformWebp.cpp>` (``include/util/function/image/WaveformWebp.cpp``)

.. |exhale_lsh| unicode:: U+021B0 .. UPWARDS ARROW WITH TIP LEFTWARDS

.. code-block:: cpp

   #include "util/function/image/WaveformWebp.hpp"
   
   #include "util/function/image/WebpWriter.hpp"
   #include "WaveformWebpHighway-inl.h"
   
   #include <algorithm>
   #include <atomic>
   #include <cmath>
   #include <cstddef>
   #include <cstdint>
   #include <limits>
   #include <mutex>
   #include <span>
   #include <string>
   #include <thread>
   #include <utility>
   #include <vector>
   
   HWY_EXPORT(ComputeWaveformColumnExtremaSIMD);
   
   namespace PDJE_UTIL::function::image {
   namespace detail {
   
   struct WaveformJob {
       const float      *samples = nullptr;
       std::size_t       sample_count = 0;
       std::size_t       channel_index = 0;
       std::size_t       image_index = 0;
       EncodedWebpBytes *output_slot = nullptr;
   };
   
   struct WorkerScratch {
       std::vector<float>        column_mins;
       std::vector<float>        column_maxs;
       std::vector<std::uint8_t> rgba;
   };
   
   inline common::Result<void>
   validate_waveform_args(const EncodeWaveformWebpArgs &args)
   {
       if (args.pcm.data() == nullptr) {
           return common::Result<void>::failure(
               { common::StatusCode::invalid_argument,
                 "EncodeWaveformWebpArgs.pcm must reference valid PCM data." });
       }
   
       if (args.pcm.empty()) {
           return common::Result<void>::failure(
               { common::StatusCode::invalid_argument,
                 "EncodeWaveformWebpArgs.pcm must not be empty." });
       }
   
       if (args.channel_count == 0) {
           return common::Result<void>::failure(
               { common::StatusCode::invalid_argument,
                 "EncodeWaveformWebpArgs.channel_count must be greater than zero." });
       }
   
       if (args.y_pixels == 0) {
           return common::Result<void>::failure(
               { common::StatusCode::invalid_argument,
                 "EncodeWaveformWebpArgs.y_pixels must be greater than zero." });
       }
   
       if (args.pcm_per_pixel == 0) {
           return common::Result<void>::failure(
               { common::StatusCode::invalid_argument,
                 "EncodeWaveformWebpArgs.pcm_per_pixel must be greater than zero." });
       }
   
       if (args.x_pixels_per_image == 0) {
           return common::Result<void>::failure(
               { common::StatusCode::invalid_argument,
                 "EncodeWaveformWebpArgs.x_pixels_per_image must be greater than zero." });
       }
   
       if (args.compression_level < -1 || args.compression_level > 9) {
           return common::Result<void>::failure(
               { common::StatusCode::invalid_argument,
                 "EncodeWaveformWebpArgs.compression_level must be between -1 and 9." });
       }
   
       return common::Result<void>::success();
   }
   
   inline bool
   checked_multiply_waveform(std::size_t lhs,
                             std::size_t rhs,
                             std::size_t &result) noexcept
   {
       if (lhs != 0 && rhs > (std::numeric_limits<std::size_t>::max() / lhs)) {
           return false;
       }
   
       result = lhs * rhs;
       return true;
   }
   
   inline bool
   checked_add_waveform(std::size_t lhs,
                        std::size_t rhs,
                        std::size_t &result) noexcept
   {
       if (rhs > (std::numeric_limits<std::size_t>::max() - lhs)) {
           return false;
       }
   
       result = lhs + rhs;
       return true;
   }
   
   inline common::Result<std::size_t>
   compute_chunk_sample_count(const EncodeWaveformWebpArgs &args)
   {
       std::size_t chunk_sample_count = 0;
       if (!checked_multiply_waveform(
               args.pcm_per_pixel, args.x_pixels_per_image, chunk_sample_count)) {
           return common::Result<std::size_t>::failure(
               { common::StatusCode::invalid_argument,
                 "Waveform chunk sample count overflows size_t." });
       }
   
       return common::Result<std::size_t>::success(chunk_sample_count);
   }
   
   inline float
   clamp_pcm_sample(float value) noexcept
   {
       return std::clamp(value, -1.0f, 1.0f);
   }
   
   inline std::size_t
   map_sample_to_row_floor(float value, std::size_t y_pixels) noexcept
   {
       if (y_pixels <= 1) {
           return 0;
       }
   
       const double clamped = static_cast<double>(clamp_pcm_sample(value));
       const double normalized_row =
           ((1.0 - clamped) * 0.5) * static_cast<double>(y_pixels - 1);
       const auto row = static_cast<std::size_t>(std::floor(normalized_row));
       return std::min(row, y_pixels - 1);
   }
   
   inline std::size_t
   map_sample_to_row_ceil(float value, std::size_t y_pixels) noexcept
   {
       if (y_pixels <= 1) {
           return 0;
       }
   
       const double clamped = static_cast<double>(clamp_pcm_sample(value));
       const double normalized_row =
           ((1.0 - clamped) * 0.5) * static_cast<double>(y_pixels - 1);
       const auto row = static_cast<std::size_t>(std::ceil(normalized_row));
       return std::min(row, y_pixels - 1);
   }
   
   inline common::Result<std::vector<std::vector<float>>>
   split_channels(const EncodeWaveformWebpArgs &args)
   {
       std::vector<float> padded_pcm(args.pcm.begin(), args.pcm.end());
       const std::size_t  remainder = padded_pcm.size() % args.channel_count;
       if (remainder != 0) {
           const std::size_t missing = args.channel_count - remainder;
           padded_pcm.resize(padded_pcm.size() + missing, 0.0f);
       }
   
       const std::size_t frame_count = padded_pcm.size() / args.channel_count;
       std::vector<std::vector<float>> channels(
           args.channel_count,
           std::vector<float>(frame_count, 0.0f));
   
       for (std::size_t frame_index = 0; frame_index < frame_count; ++frame_index) {
           const std::size_t frame_offset = frame_index * args.channel_count;
           for (std::size_t channel_index = 0; channel_index < args.channel_count;
                ++channel_index) {
               channels[channel_index][frame_index] =
                   padded_pcm[frame_offset + channel_index];
           }
       }
   
       return common::Result<std::vector<std::vector<float>>>::success(
           std::move(channels));
   }
   
   inline common::Status
   wrap_job_status(const common::Status &status, const WaveformJob &job)
   {
       return { status.code,
                "Waveform job channel " + std::to_string(job.channel_index) +
                    " image " + std::to_string(job.image_index) + ": " +
                    status.message };
   }
   
   inline std::size_t
   resolve_worker_thread_count(std::size_t requested, std::size_t job_count) noexcept
   {
       std::size_t resolved = requested;
       if (resolved == 0) {
           resolved = static_cast<std::size_t>(std::thread::hardware_concurrency());
       }
   
       if (resolved == 0) {
           resolved = 1;
       }
   
       if (job_count == 0) {
           return 1;
       }
   
       return std::min(resolved, job_count);
   }
   
   inline common::Result<void>
   compute_column_extrema_dispatch(std::span<const float> chunk_pcm,
                                   std::size_t            pcm_per_pixel,
                                   std::size_t            x_pixels_per_image,
                                   std::span<float>       mins_out,
                                   std::span<float>       maxs_out)
   {
       std::size_t expected_sample_count = 0;
       if (!checked_multiply_waveform(
               pcm_per_pixel, x_pixels_per_image, expected_sample_count)) {
           return common::Result<void>::failure(
               { common::StatusCode::invalid_argument,
                 "Waveform chunk sample count overflows size_t." });
       }
   
       if (chunk_pcm.size() != expected_sample_count) {
           return common::Result<void>::failure(
               { common::StatusCode::invalid_argument,
                 "Waveform chunk span size does not match pcm_per_pixel * x_pixels_per_image." });
       }
   
       if (mins_out.size() != x_pixels_per_image || maxs_out.size() != x_pixels_per_image) {
           return common::Result<void>::failure(
               { common::StatusCode::invalid_argument,
                 "Waveform extrema buffers must match x_pixels_per_image." });
       }
   
       HWY_DYNAMIC_DISPATCH(ComputeWaveformColumnExtremaSIMD)(chunk_pcm.data(),
                                                              pcm_per_pixel,
                                                              x_pixels_per_image,
                                                              mins_out.data(),
                                                              maxs_out.data());
       return common::Result<void>::success();
   }
   
   inline common::Result<void>
   encode_waveform_job(const WaveformJob         &job,
                       const EncodeWaveformWebpArgs &args,
                       std::size_t                row_stride,
                       WorkerScratch             &scratch,
                       function::EvalOptions      options)
   {
       if (job.samples == nullptr || job.output_slot == nullptr) {
           return common::Result<void>::failure(
               { common::StatusCode::internal_error,
                 "Waveform job was missing required sample or output pointers." });
       }
   
       auto extrema = compute_column_extrema_dispatch(
           std::span<const float>(job.samples, job.sample_count),
           args.pcm_per_pixel,
           args.x_pixels_per_image,
           std::span<float>(scratch.column_mins.data(), scratch.column_mins.size()),
           std::span<float>(scratch.column_maxs.data(), scratch.column_maxs.size()));
       if (!extrema.ok()) {
           return common::Result<void>::failure(
               wrap_job_status(extrema.status(), job));
       }
   
       std::fill(scratch.rgba.begin(), scratch.rgba.end(), std::uint8_t { 0 });
   
       for (std::size_t x = 0; x < args.x_pixels_per_image; ++x) {
           std::size_t top_row =
               map_sample_to_row_floor(scratch.column_maxs[x], args.y_pixels);
           std::size_t bottom_row =
               map_sample_to_row_ceil(scratch.column_mins[x], args.y_pixels);
           if (top_row > bottom_row) {
               std::swap(top_row, bottom_row);
           }
   
           for (std::size_t y = top_row; y <= bottom_row; ++y) {
               const std::size_t pixel_offset = (y * row_stride) + (x * 4);
               scratch.rgba[pixel_offset + 0] = 255;
               scratch.rgba[pixel_offset + 1] = 255;
               scratch.rgba[pixel_offset + 2] = 255;
               scratch.rgba[pixel_offset + 3] = 255;
           }
       }
   
       auto encoded = encode_webp(
           { .image =
                 {
                     .pixels = scratch.rgba,
                     .width = args.x_pixels_per_image,
                     .height = args.y_pixels,
                     .stride = row_stride,
                     .pixel_format = RasterPixelFormat::rgba8,
                 },
             .compression_level = args.compression_level },
           options);
       if (!encoded.ok()) {
           return common::Result<void>::failure(
               wrap_job_status(encoded.status(), job));
       }
   
       *(job.output_slot) = std::move(encoded).value();
       return common::Result<void>::success();
   }
   
   } // namespace detail
   
   common::Result<WaveformWebpBatch>
   encode_waveform_webps(const EncodeWaveformWebpArgs &args,
                         function::EvalOptions         options)
   {
       const auto validation = detail::validate_waveform_args(args);
       if (!validation.ok()) {
           return common::Result<WaveformWebpBatch>::failure(validation.status());
       }
   
       const auto split = detail::split_channels(args);
       if (!split.ok()) {
           return common::Result<WaveformWebpBatch>::failure(split.status());
       }
   
       const auto chunk_sample_count_result = detail::compute_chunk_sample_count(args);
       if (!chunk_sample_count_result.ok()) {
           return common::Result<WaveformWebpBatch>::failure(
               chunk_sample_count_result.status());
       }
       const std::size_t chunk_sample_count = chunk_sample_count_result.value();
   
       std::size_t row_stride = 0;
       std::size_t image_byte_count = 0;
       if (!detail::checked_multiply_waveform(
               args.x_pixels_per_image, std::size_t { 4 }, row_stride) ||
           !detail::checked_multiply_waveform(
               row_stride, args.y_pixels, image_byte_count)) {
           return common::Result<WaveformWebpBatch>::failure(
               { common::StatusCode::invalid_argument,
                 "Waveform image size overflows size_t." });
       }
   
       auto channels = std::move(split).value();
   
       WaveformWebpBatch            batch(channels.size());
       std::vector<detail::WaveformJob> jobs;
       std::size_t                  total_job_count = 0;
   
       for (std::size_t channel_index = 0; channel_index < channels.size();
            ++channel_index) {
           auto &channel_pcm = channels[channel_index];
           const std::size_t chunk_remainder = channel_pcm.size() % chunk_sample_count;
           if (chunk_remainder != 0) {
               const std::size_t missing = chunk_sample_count - chunk_remainder;
               channel_pcm.resize(channel_pcm.size() + missing, 0.0f);
           }
   
           const std::size_t image_count = channel_pcm.size() / chunk_sample_count;
           batch[channel_index].resize(image_count);
   
           std::size_t next_total_job_count = 0;
           if (!detail::checked_add_waveform(total_job_count, image_count, next_total_job_count)) {
               return common::Result<WaveformWebpBatch>::failure(
                   { common::StatusCode::invalid_argument,
                     "Waveform job count overflows size_t." });
           }
           total_job_count = next_total_job_count;
       }
   
       jobs.reserve(total_job_count);
   
       for (std::size_t channel_index = 0; channel_index < channels.size();
            ++channel_index) {
           auto &channel_pcm = channels[channel_index];
           for (std::size_t image_index = 0;
                image_index < batch[channel_index].size();
                ++image_index) {
               const std::size_t sample_offset = image_index * chunk_sample_count;
               jobs.push_back({ .samples = channel_pcm.data() + sample_offset,
                                .sample_count = chunk_sample_count,
                                .channel_index = channel_index,
                                .image_index = image_index,
                                .output_slot = &batch[channel_index][image_index] });
           }
       }
   
       if (jobs.empty()) {
           return common::Result<WaveformWebpBatch>::success(std::move(batch));
       }
   
       const std::size_t resolved_worker_count =
           detail::resolve_worker_thread_count(args.worker_thread_count, jobs.size());
       std::atomic<std::size_t> next_job_index { 0 };
       std::atomic<bool>        stop_requested { false };
       std::mutex               error_mutex;
       common::Status           first_error = {};
   
       auto record_error = [&](common::Status status) {
           std::lock_guard<std::mutex> lock(error_mutex);
           if (first_error.ok()) {
               first_error = std::move(status);
           }
           stop_requested.store(true, std::memory_order_release);
       };
   
       auto worker_fn = [&]() {
           detail::WorkerScratch scratch {
               .column_mins = std::vector<float>(args.x_pixels_per_image, 0.0f),
               .column_maxs = std::vector<float>(args.x_pixels_per_image, 0.0f),
               .rgba = std::vector<std::uint8_t>(image_byte_count, 0),
           };
   
           try {
               while (true) {
                   if (stop_requested.load(std::memory_order_acquire)) {
                       return;
                   }
   
                   const std::size_t job_index =
                       next_job_index.fetch_add(1, std::memory_order_relaxed);
                   if (job_index >= jobs.size()) {
                       return;
                   }
   
                   auto job_result = detail::encode_waveform_job(
                       jobs[job_index], args, row_stride, scratch, options);
                   if (!job_result.ok()) {
                       record_error(job_result.status());
                       return;
                   }
               }
           } catch (const std::exception &e) {
               record_error(
                   { common::StatusCode::internal_error,
                     std::string("Waveform worker failed: ") + e.what() });
           } catch (...) {
               record_error(
                   { common::StatusCode::internal_error,
                     "Waveform worker failed with an unknown exception." });
           }
       };
   
       std::vector<std::thread> workers;
       workers.reserve(resolved_worker_count);
   
       try {
           for (std::size_t i = 0; i < resolved_worker_count; ++i) {
               workers.emplace_back(worker_fn);
           }
       } catch (const std::exception &e) {
           stop_requested.store(true, std::memory_order_release);
           for (auto &worker : workers) {
               if (worker.joinable()) {
                   worker.join();
               }
           }
           return common::Result<WaveformWebpBatch>::failure(
               { common::StatusCode::internal_error,
                 std::string("Failed to launch waveform worker: ") + e.what() });
       }
   
       for (auto &worker : workers) {
           if (worker.joinable()) {
               worker.join();
           }
       }
   
       if (!first_error.ok()) {
           return common::Result<WaveformWebpBatch>::failure(first_error);
       }
   
       return common::Result<WaveformWebpBatch>::success(std::move(batch));
   }
   
   } // namespace PDJE_UTIL::function::image
