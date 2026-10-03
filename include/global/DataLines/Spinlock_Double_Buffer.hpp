#pragma once

#include <atomic>
#include <cstddef>
#include <mutex>
#include <vector>

template <typename T> class Spinlock_Double_Buffer {
  private:
    // BasicLockable adapter: preserve the existing spinlock and memory orders.
    struct SpinLock {
        std::atomic_flag &flag;

        void
        lock() noexcept
        {
            while (flag.test_and_set(std::memory_order_acquire)) {
            }
        }

        void
        unlock() noexcept
        {
            flag.clear(std::memory_order_release);
        }
    };
    std::vector<T>   first;
    std::vector<T>   second;
    std::atomic_flag lock      = ATOMIC_FLAG_INIT;
    bool             acc_first = true;

  public:
    explicit Spinlock_Double_Buffer(const std::size_t reserve_size)
    {
        first.reserve(reserve_size);
        second.reserve(reserve_size);
        lock.clear(std::memory_order_relaxed);
    }
    ~Spinlock_Double_Buffer() = default;

    void
    Write(const T &data)
    {
        SpinLock                  spin_lock{ lock };
        std::lock_guard<SpinLock> guard(spin_lock);
        if (acc_first) {
            first.push_back(data);
        } else {
            second.push_back(data);
        }
    }

    std::vector<T> *
    Get()
    {
        SpinLock                  spin_lock{ lock };
        std::lock_guard<SpinLock> guard(spin_lock);

        if (acc_first) {
            second.clear();
        } else {
            first.clear();
        }
        acc_first = !acc_first;

        if (acc_first) {
            return &second;
        }
        return &first;
    }
};
