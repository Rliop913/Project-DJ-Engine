#pragma once

#include <cmath>

#include "MixBinary.hpp"
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

struct BpmStruct{
    unsigned long bar;
    unsigned long beat;
    unsigned long separate;
    unsigned long frame_to_here = 0;
    double bpm;
};

struct MixStruct{
    unsigned long frame_in;
    unsigned long frame_out;
    MBData::Reader RP;
};