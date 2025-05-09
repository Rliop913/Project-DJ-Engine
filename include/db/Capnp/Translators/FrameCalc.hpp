#pragma once

#include <cmath>
#include <algorithm>

#include "CapnpBinary.hpp"





// #include <hwy/highway.h>
#include <hwy/aligned_allocator.h>


// namespace hn = hwy::HWY_NAMESPACE;


using SIMD_FLOAT    = std::vector<float, hwy::AlignedAllocator<float>>;


#define CHANNEL 2
#define SAMPLERATE 48000
#define DSAMPLERATE 48000.0
#define DMINUTE 60.0

/**
 * @brief Macro for calculates bar-beat approx position
 * 
 */
#define APPRX(TYPE, BAR, BEAT, SEP)\
static_cast<TYPE>(BAR) +\
(\
    static_cast<TYPE>(BEAT) / \
    static_cast<TYPE>(SEP)\
)

/**
 * @brief namespace for calculate frames.
 * 
 */
namespace FrameCalc{
    /**
     * @brief count frame between two bar-beat position.
     * 
     * @param Sbar start bar
     * @param Sbeat start beat
     * @param Sseparate start bar-beat's separate value
     * @param Ebar end bar
     * @param Ebeat end beat
     * @param Eseparate end bar-beat's separate value
     * @param bpm applied bpm.
     * @return unsigned long the calculated Frame Range
     */
    extern unsigned long CountFrame(
        unsigned long Sbar,
        unsigned long Sbeat,
        unsigned long Sseparate,
        unsigned long Ebar,
        unsigned long Ebeat,
        unsigned long Eseparate,
        double bpm
    );
};

/**
 * @brief the smallest unit of bpm class
 * 
 */
struct BpmFragment{
    unsigned long bar;
    unsigned long beat;
    unsigned long separate;
    unsigned long long frame_to_here = 0;
    double bpm;
};

/**
 * @brief contains BpmFragments. and calculate with fragments.
 * 
 */
struct BpmStruct{
    /// @brief the fragment data.
    std::vector<BpmFragment> fragments;

    /// @brief Sort the fragments Ascending by the bar-beat approx 
    void sortFragment();
    /**
     * @brief fills the value "frame_to_here" in BpmFragments
     * 
     * @param StartPos the offset(Frame position)
     * @return true 
     * @return false 
     */
    bool calcFrame(unsigned long long StartPos = 0);
    /**
     * @brief Get the Affected BpmFragment
     * 
     * @param searchFrag the BpmFragment for search affected position. you should fill bar-beat before use.
     * @return const BpmFragment& the affected BpmFragment
     */
    const BpmFragment& getAffected(const BpmFragment& searchFrag) const;
    /**
     * @brief Get the Affected BpmFragment
     * 
     * @param searchFrame the Frame position to search affected BpmFragment
     * @return const BpmFragment& the affected BpmFragment
     */
    const BpmFragment& getAffected(const unsigned long long searchFrame) const;

    /**
     * @brief Get the Affected BpmFragment list with range
     * 
     * @param searchStartFrame start Frame position
     * @param searchEndFrame end Frame position
     * @return const std::vector<const BpmFragment*> the affected BpmFragment list.
     */
    const std::vector<const BpmFragment*> 
    getAffectedList(
        const unsigned long long searchStartFrame,
        const unsigned long long searchEndFrame
    ) const;
};

/**
 * @brief processed mixing data struct.
 * 
 */
struct MixStruct{
    unsigned long long frame_in;
    unsigned long long frame_out;
    /// @brief capnp Reader Pointer
    MBData::Reader RP;
};