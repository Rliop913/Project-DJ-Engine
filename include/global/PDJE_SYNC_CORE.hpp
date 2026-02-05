#pragma once
#include "PDJE_EXPORT_SETTER.hpp"
#include <cstdint>
struct PDJE_API audioSyncData {
    unsigned long long consumed_frames = 0;
    unsigned long long pre_calculated_unused_frames = 0;
    uint64_t           microsecond     = 0;
};