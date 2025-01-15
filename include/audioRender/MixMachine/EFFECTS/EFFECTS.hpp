#pragma once

#include "DeckData.hpp"

using EFFECT_VALUE = double;
using EFFECT_VALUE_INTERPOLATE = double;
using ON_OFF = bool;
using EFFECT_BPM = double;
using FEEDBACK = double;
using POWER = double;
using MIN_FREQUENCY = double;
using GAIN = double;
struct Filter{
    std::vector<Values<EFFECT_VALUE, EFFECT_VALUE_INTERPOLATE, ON_OFF>> High;
    std::vector<Values<EFFECT_VALUE, EFFECT_VALUE_INTERPOLATE, ON_OFF>> Low;
};


struct EQ{
    std::vector<Values<EFFECT_VALUE,EFFECT_VALUE_INTERPOLATE,ON_OFF>> High;
    std::vector<Values<EFFECT_VALUE,EFFECT_VALUE_INTERPOLATE,ON_OFF>> Mid;
    std::vector<Values<EFFECT_VALUE,EFFECT_VALUE_INTERPOLATE,ON_OFF>> Low;
};

struct Distortion{
    std::vector<Values<EFFECT_VALUE,EFFECT_VALUE_INTERPOLATE,ON_OFF>> Single;
};


struct Echo{
    std::vector<Values<EFFECT_BPM, FEEDBACK, POWER>> Single;
};

struct LFO_Filter{
    std::vector<Values<EFFECT_BPM, MIN_FREQUENCY, POWER>> Single;
};

struct Flanger{
    std::vector<Values<EFFECT_BPM, GAIN, POWER>> Single;
};

struct Phaser{
    std::vector<Values<EFFECT_BPM, GAIN, POWER>> Single;
};

struct Trance{
    std::vector<Values<EFFECT_BPM, GAIN, POWER>> Single;
};

struct Panner{
    std::vector<Values<EFFECT_BPM, GAIN, POWER>> Single;
};

struct Roll{
    std::vector<Values<EFFECT_BPM, POWER, ON_OFF>> Single;
};

struct EFFECTS{
    Filter filters;
    EQ eq;
    Distortion distortions;
    Echo echos;
    LFO_Filter lfo_filters;
    Flanger flangers;
    Phaser phasers;
    Trance trances;
    Panner panners;
    Roll rolls;
};
