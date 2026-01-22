#pragma once
#include <cstdint>
#include <ctime>
#include <sys/time.h>
#include <time.h>
namespace PDJE_HIGHRES_CLOCK {

class CLOCK {
  private:
    struct timespec ts;

  public:
    CLOCK()  = default;
    ~CLOCK() = default;
    uint64_t
    Get_MicroSecond() noexcept
    {
        clock_gettime(CLOCK_MONOTONIC, &ts);
        return (uint64_t)ts.tv_sec * 1000000ull +
               (uint64_t)ts.tv_nsec / 1000ull;
    }
    static inline uint64_t
    ConvertToMicroSecond(const timeval &linux_time) noexcept
    {
        return (uint64_t)linux_time.tv_sec * 1000000ull +
               (uint64_t)linux_time.tv_usec;
    }
};
}; // namespace PDJE_HIGHRES_CLOCK