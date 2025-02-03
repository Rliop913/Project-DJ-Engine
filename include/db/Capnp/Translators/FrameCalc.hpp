#pragma once

#include <cmath>
#include <algorithm>

#include "CapnpBinary.hpp"
#define CHANNEL 2
#define SAMPLERATE 48000
#define DSAMPLERATE 48000.0
#define DMINUTE 60.0

#define APPRX(TYPE, BAR, BEAT, SEP)\
static_cast<TYPE>(BAR) +\
(\
    static_cast<TYPE>(BEAT) / \
    static_cast<TYPE>(SEP)\
)

namespace FrameCalc{
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

struct BpmFragment{
    unsigned long bar;
    unsigned long beat;
    unsigned long separate;
    unsigned long long frame_to_here = 0;
    double bpm;
};

struct BpmStruct{

    std::vector<BpmFragment> fragments;
    void sortFragment();
    bool calcFrame(unsigned long long StartPos = 0);
    const BpmFragment& getAffected(const BpmFragment& searchFrag) const;
    const BpmFragment& getAffected(const unsigned long long searchFrame) const;
    const std::vector<const BpmFragment*> 
    getAffectedList(
        const unsigned long long searchStartFrame,
        const unsigned long long searchEndFrame
    ) const;
};

struct MixStruct{
    unsigned long long frame_in;
    unsigned long long frame_out;
    MBData::Reader RP;
};