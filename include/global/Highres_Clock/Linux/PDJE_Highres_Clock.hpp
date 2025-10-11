#pragma once
#include <cstdint>
#include <time.h>
namespace PDJE_HIGHRES_CLOCK {

class CLOCK {
  private:
    struct timespec ts;

  public:
    uint64_t
    Get_MicroSecond()
    {
        clock_gettime(CLOCK_MONOTONIC_RAW, &ts);

        return (uint64_t)ts.tv_sec * 1000000ull + ts.tv_nsec / 1000;
    }
};
}; // namespace PDJE_HIGHRES_CLOCK