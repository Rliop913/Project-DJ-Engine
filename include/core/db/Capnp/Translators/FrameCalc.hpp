#pragma once
#include "FrameCalcCore.hpp"

#include "CapnpBinary.hpp"

// #include <hwy/highway.h>
#include <hwy/aligned_allocator.h>
#include <vector>

// namespace hn = hwy::HWY_NAMESPACE;

using SIMD_FLOAT = std::vector<float, hwy::AlignedAllocator<float>>;

/**
 * @brief processed mixing data struct.
 *
 */
struct MixStruct {
    unsigned long long frame_in;
    unsigned long long frame_out;
    /// @brief capnp Reader Pointer
    MBData::Reader RP;
};
