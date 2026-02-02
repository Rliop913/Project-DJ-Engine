#pragma once

#include "PDJE_EXPORT_SETTER.hpp"
#include "ipc_shared_memory.hpp"
#include <atomic>
#include <memory_resource>
#include <random>
#include <vector>

namespace fs = std::filesystem;

template <typename T> class Atomic_Double_Buffer {
  private:
    std::vector<T>   first;
    std::vector<T>   second;
    std::atomic_flag lock      = ATOMIC_FLAG_INIT;
    bool             acc_first = true;

  public:
    Atomic_Double_Buffer(const size_t reserve_size)
    {
        first.reserve(reserve_size);
        second.reserve(reserve_size);
        lock.clear(std::memory_order_relaxed);
    }
    ~Atomic_Double_Buffer() = default;

    void
    Write(const T &data)
    {
        while (lock.test_and_set(std::memory_order_acquire)) {
        }
        // locked
        if (acc_first) {

            first.push_back(data);

        } else {
            second.push_back(data);
        }
        lock.clear(std::memory_order_release); // unlock
    }
    std::vector<T> *
    Get()
    {
        while (lock.test_and_set(std::memory_order_acquire)) {
        }

        if (acc_first) {
            second.clear();
        } else {
            first.clear();
        }
        acc_first = !acc_first;
        lock.clear(std::memory_order_release);
        if (acc_first) {
            return &second;
        } else {
            return &first;
        }
    }
};
