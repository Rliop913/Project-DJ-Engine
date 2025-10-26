#pragma once

#include <algorithm>
#include <cmath>

#include "CapnpBinary.hpp"

#include "PDJE_EXPORT_SETTER.hpp"

// #include <hwy/highway.h>
#include <hwy/aligned_allocator.h>

// namespace hn = hwy::HWY_NAMESPACE;

using SIMD_FLOAT = std::vector<float, hwy::AlignedAllocator<float>>;

#define CHANNEL 2
#define SAMPLERATE 48000
#define DSAMPLERATE 48000.0
#define DMINUTE 60.0

/**
 * @brief Macro for calculates beat-subBeat approx position
 *
 */
#define APPRX(TYPE, BEAT, SUBBEAT, SEP)                                        \
    static_cast<TYPE>(BEAT) +                                                  \
        (static_cast<TYPE>(SUBBEAT) / static_cast<TYPE>(SEP))

/**
 * @brief namespace for calculate frames.
 *
 */
namespace FrameCalc {
/**
 * @brief count frame between two beat-subBeat position.
 *
 * @param Sbeat start beat
 * @param SsubBeat start subBeat
 * @param Sseparate start beat-subBeat's separate value
 * @param Ebeat end beat
 * @param EsubBeat end subBeat
 * @param Eseparate end beat-subBeat's separate value
 * @param bpm applied bpm.
 * @return unsigned long the calculated Frame Range
 */
extern uint64_t
CountFrame(uint64_t Sbeat,
           uint64_t SsubBeat,
           uint64_t Sseparate,
           uint64_t Ebeat,
           uint64_t EsubBeat,
           uint64_t Eseparate,
           double        bpm);
}; // namespace FrameCalc

/**
 * @brief the smallest unit of bpm class
 *
 */
struct PDJE_API BpmFragment {
    uint64_t      beat=0;
    uint64_t      subBeat=0;
    uint64_t      separate=0;
    uint64_t frame_to_here = 0;
    double             bpm=0;
};

/**
 * @brief contains BpmFragments. and calculate with fragments.
 *
 */
struct PDJE_API BpmStruct {
    /// @brief the fragment data.
    std::vector<BpmFragment> fragments;

    /// @brief Sort the fragments Ascending by the beat-beat approx
    void
    sortFragment();
    /**
     * @brief fills the value "frame_to_here" in BpmFragments
     *
     * @param StartPos the offset(Frame position)
     * @return true
     * @return false
     */
    bool
    calcFrame(unsigned long long StartPos = 0);
    /**
     * @brief Get the Affected BpmFragment
     *
     * @param searchFrag the BpmFragment for search affected position. you
     * should fill beat-beat before use.
     * @return const BpmFragment& the affected BpmFragment
     */
    const BpmFragment &
    getAffected(const BpmFragment &searchFrag) const;
    /**
     * @brief Get the Affected BpmFragment
     *
     * @param searchFrame the Frame position to search affected BpmFragment
     * @return const BpmFragment& the affected BpmFragment
     */
    const BpmFragment &
    getAffected(const unsigned long long searchFrame) const;

    /**
     * @brief Get the Affected BpmFragment list with range
     *
     * @param searchStartFrame start Frame position
     * @param searchEndFrame end Frame position
     * @return const std::vector<const BpmFragment*> the affected BpmFragment
     * list.
     */
    const std::vector<const BpmFragment *>
    getAffectedList(const unsigned long long searchStartFrame,
                    const unsigned long long searchEndFrame) const;
};

/**
 * @brief processed mixing data struct.
 *
 */
struct PDJE_API MixStruct {
    unsigned long long frame_in;
    unsigned long long frame_out;
    /// @brief capnp Reader Pointer
    MBData::Reader RP;
};