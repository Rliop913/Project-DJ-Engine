#include "QPC_Timer.hpp"


QPC_Timer::QPC_Timer()
{
    QueryPerformanceFrequency(&temp_int);
    qpc_freq = static_cast<uint64_t>(temp_int.QuadPart);
}


uint64_t
QPC_Timer::now()
{
    QueryPerformanceCounter(&temp_int);
    return static_cast<uint64_t>(temp_int.QuadPart);
}


double
QPC_Timer::to_second(uint64_t tick)
{
    return 
        static_cast<double>(tick / qpc_freq) + 
            static_cast<double>(tick % qpc_freq) / static_cast<double>(qpc_freq);
}


double
QPC_Timer::to_ms(uint64_t tick)
{
    return 
        (static_cast<double>(tick / qpc_freq) * 1000.0) + 
            (static_cast<double>(tick % qpc_freq) * 1000.0) / static_cast<double>(qpc_freq);
}