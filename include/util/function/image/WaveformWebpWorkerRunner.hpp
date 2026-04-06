#pragma once

#include "util/common/Result.hpp"
#include "util/function/image/WaveformWebpInternal.hpp"
#include "util/function/image/WaveformWebpSupport.hpp"

#include <algorithm>
#include <atomic>
#include <cstddef>
#include <exception>
#include <mutex>
#include <string>
#include <thread>
#include <utility>
#include <vector>

namespace PDJE_UTIL::function::image::detail {

class WaveformWorkerRunner {
  public:
    explicit WaveformWorkerRunner(std::size_t requested_worker_count)
        : requested_worker_count_(requested_worker_count)
    {
    }

    template <class ProcessorFactory>
    common::Result<WaveformWebpBatch>
    Run(WaveformEncodePlan &plan, ProcessorFactory &&create_processor) const
    {
        if (plan.jobs.empty()) {
            return common::Result<WaveformWebpBatch>::success(std::move(plan.batch));
        }

        const std::size_t resolved_worker_count =
            ResolveWorkerThreadCount(requested_worker_count_, plan.jobs.size());
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
            try {
                auto processor = create_processor();

                while (true) {
                    if (stop_requested.load(std::memory_order_acquire)) {
                        return;
                    }

                    const std::size_t job_index =
                        next_job_index.fetch_add(1, std::memory_order_relaxed);
                    if (job_index >= plan.jobs.size()) {
                        return;
                    }

                    const auto &job = plan.jobs[job_index];
                    if (job.samples == nullptr || job.output_slot == nullptr) {
                        record_error(support::wrap_job_status(
                            { common::StatusCode::internal_error,
                              "Waveform job was missing required sample or "
                              "output pointers." },
                            job));
                        return;
                    }

                    auto job_result = processor.Process(job, *(job.output_slot));
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

        return common::Result<WaveformWebpBatch>::success(std::move(plan.batch));
    }

  private:
    static std::size_t
    ResolveWorkerThreadCount(std::size_t requested,
                             std::size_t job_count) noexcept
    {
        std::size_t resolved = requested;
        if (resolved == 0) {
            resolved =
                static_cast<std::size_t>(std::thread::hardware_concurrency());
        }

        if (resolved == 0) {
            resolved = 1;
        }

        if (job_count == 0) {
            return 1;
        }

        return std::min(resolved, job_count);
    }

    std::size_t requested_worker_count_ = 0;
};

} // namespace PDJE_UTIL::function::image::detail
