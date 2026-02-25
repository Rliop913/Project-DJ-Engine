#pragma once

#include "PDJE_EXPORT_SETTER.hpp"

#include <cstdint>
#include <vector>

#define CHANNEL 2
#define SAMPLERATE 48000
#define DSAMPLERATE 48000.0
#define DMINUTE 60.0

/**
 * @brief Macro for calculates beat-subBeat approx position
 */
#define APPRX(TYPE, BEAT, SUBBEAT, SEP)                                        \
    static_cast<TYPE>(BEAT) +                                                  \
        (static_cast<TYPE>(SUBBEAT) / static_cast<TYPE>(SEP))

namespace FrameCalc {
extern uint64_t
CountFrame(uint64_t Sbeat,
           uint64_t SsubBeat,
           uint64_t Sseparate,
           uint64_t Ebeat,
           uint64_t EsubBeat,
           uint64_t Eseparate,
           double   bpm);
}; // namespace FrameCalc

struct PDJE_API BpmFragment {
    uint64_t beat          = 0;
    uint64_t subBeat       = 0;
    uint64_t separate      = 0;
    uint64_t frame_to_here = 0;
    double   bpm           = 0;
};

struct PDJE_API BpmStruct {
    std::vector<BpmFragment> fragments;

    void
    sortFragment();
    bool
    calcFrame(unsigned long long StartPos = 0);
    const BpmFragment &
    getAffected(const BpmFragment &searchFrag) const;
    const BpmFragment &
    getAffected(const unsigned long long searchFrame) const;
    const std::vector<const BpmFragment *>
    getAffectedList(const unsigned long long searchStartFrame,
                    const unsigned long long searchEndFrame) const;
};
