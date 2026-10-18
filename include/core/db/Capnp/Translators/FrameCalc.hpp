#pragma once
#include "FrameCalcCore.hpp"

#include "CapnpBinary.hpp"

// #include <hwy/highway.h>
#include <hwy/aligned_allocator.h>
#include <cstdint>
#include <vector>

// namespace hn = hwy::HWY_NAMESPACE;

using SIMD_FLOAT = std::vector<float, hwy::AlignedAllocator<float>>;

/**
 * @brief processed mixing data struct.
 *
 */
struct MixStruct {
    uint64_t frame_in;
    uint64_t frame_out;
    /// @brief capnp Reader Pointer
    MBData::Reader RP;
};
