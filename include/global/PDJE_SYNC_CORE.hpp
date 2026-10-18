#pragma once
#include "PDJE_EXPORT_SETTER.hpp"
#include <cstdint>
struct PDJE_API audioSyncData {
    uint64_t consumed_frames = 0;
    uint64_t pre_calculated_unused_frames = 0;
    uint64_t microsecond = 0;
};