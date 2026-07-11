#pragma once

#include "util/function/image/detail/WaveformWebpInternal.hpp"
#include "util/function/image/detail/WaveformWebpSupport.hpp"

#include <algorithm>
#include <atomic>
#include <exception>
#include <mutex>
#include <thread>
#include <utility>
#include <vector>

namespace PDJE_UTIL::function::image::detail {

class WaveformWorkerRunner {
  public:
    explicit WaveformWorkerRunner(std::size_t requested_workers)
        : requested_workers_(requested_workers)
    {
    }

    template <class ProcessorFactory>
    WaveformWebpBatch Run(WaveformEncodePlan &plan,
                          ProcessorFactory &&create_processor) const
    {
        if (plan.jobs.empty()) return std::move(plan.batch);

        const auto worker_count = ResolveWorkerCount(
            requested_workers_, plan.jobs.size());
        std::atomic<std::size_t> next_job { 0 };
        std::atomic<bool> stop { false };
        std::mutex error_mutex;
        std::exception_ptr first_error;

        const auto record_error = [&](std::exception_ptr error) {
            std::lock_guard lock(error_mutex);
            if (!first_error) first_error = std::move(error);
            stop.store(true, std::memory_order_release);
        };

        const auto worker = [&] {
            try {
                auto processor = create_processor();
                while (!stop.load(std::memory_order_acquire)) {
                    const auto index = next_job.fetch_add(1);
                    if (index >= plan.jobs.size()) return;
                    const auto &job = plan.jobs[index];
                    if (job.samples == nullptr || job.output_slot == nullptr) {
                        throw support::job_error(job,
                            "job is missing sample or output storage");
                    }
                    processor.Process(job, *job.output_slot);
                }
            } catch (...) {
                record_error(std::current_exception());
            }
        };

        std::vector<std::thread> workers;
        workers.reserve(worker_count);
        try {
            for (std::size_t index = 0; index < worker_count; ++index) {
                workers.emplace_back(worker);
            }
        } catch (...) {
            stop.store(true, std::memory_order_release);
            for (auto &thread : workers) if (thread.joinable()) thread.join();
            throw;
        }
        for (auto &thread : workers) if (thread.joinable()) thread.join();
        if (first_error) std::rethrow_exception(first_error);
        return std::move(plan.batch);
    }

  private:
    static std::size_t ResolveWorkerCount(std::size_t requested,
                                          std::size_t jobs) noexcept
    {
        auto count = requested == 0
            ? static_cast<std::size_t>(std::thread::hardware_concurrency())
            : requested;
        if (count == 0) count = 1;
        return std::min(count, jobs);
    }

    std::size_t requested_workers_ = 0;
};

} // namespace PDJE_UTIL::function::image::detail
