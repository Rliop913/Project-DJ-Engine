#pragma once

#include <cstdint>
#include <windows.h>

#pragma once
#include <cstdint>
#include <time.h>
namespace PDJE_HIGHRES_CLOCK {

class CLOCK {
  private:
    struct timespec ts;
    uint64_t        qpc_freq;
    LARGE_INTEGER   temp_int;

  public:
    CLOCK()
    {

        QueryPerformanceFrequency(&temp_int);
        qpc_freq = static_cast<uint64_t>(temp_int.QuadPart);
    }
    uint64_t
    Get_MicroSecond()
    {
        QueryPerformanceCounter(&temp_int);
        return static_cast<uint64_t>(static_cast<uint64_t>(temp_int.QuadPart) /
                                     qpc_freq) *
                   static_cast<uint64_t>(1000000) +
               static_cast<uint64_t>(static_cast<uint64_t>(temp_int.QuadPart) %
                                     qpc_freq) *
                   static_cast<uint64_t>(1000000) / qpc_freq;
    }
    ~CLOCK() = default;
};
}; // namespace PDJE_HIGHRES_CLOCK
