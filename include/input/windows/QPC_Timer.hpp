#pragma once

#include <windows.h>
#include <cstdint>

class QPC_Timer {
private:
    uint64_t qpc_freq;
    LARGE_INTEGER temp_int;


public:
    uint64_t now();
    double to_second(uint64_t tick);
    double to_ms(uint64_t tick);
    uint64_t to_micro(uint64_t tick);
    QPC_Timer();
    ~QPC_Timer() = default;
};

