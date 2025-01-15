#pragma once

#include "DeckData.hpp"

using EFFECT_VALUE = double;
using EFFECT_VALUE_INTERPOLATE = double;
using ON_OFF = bool;

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

using EFFECT_BPM = double;
using FEEDBACK = double;
using POWER = double;
using MIN_FREQUENCY = double;
struct Echo{
    std::vector<Values<EFFECT_BPM, FEEDBACK, POWER>> Single;
};

struct LFO_Filter{
    std::vector<Values<EFFECT_BPM, MIN_FREQUENCY, POWER>> Single;
};

struct flanger