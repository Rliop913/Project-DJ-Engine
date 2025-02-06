#pragma once

#include "DeckData.hpp"

#include "FAUST_COMPRESSOR.hpp"
#include "FAUST_DISTORTION.hpp"
#include "FAUST_ECHO.hpp"
#include "FAUST_EQ.hpp"
#include "FAUST_FILTERS.hpp"
#include "FAUST_FLANGER.hpp"
#include "FAUST_OCS_FILTER.hpp"
#include "FAUST_PANNER.hpp"
#include "FAUST_PHASER.hpp"
#include "FAUST_ROLL.hpp"
#include "FAUST_TRANCE.hpp"
#include "FAUST_VOL.hpp"


using EFFECT_VALUE = double;
using EFFECT_VALUE_INTERPOLATE = double;
using ON_OFF = bool;
using EFFECT_BPM = double;
using FEEDBACK = double;
using POWER = double;
using MIN_FREQUENCY = double;
using GAIN = double;

struct FX_TIME_STAMP{
    unsigned long Frame;
    double Val;

};

struct EFFECT_JOB{
    FX_TIME_STAMP in;
    FX_TIME_STAMP out;
    bool NO_OUT = true;
};

struct PCM_MIX_RANGE{
    unsigned long pos;
    unsigned long range;
    unsigned long IdxPos() const {return pos * CHANNEL;}
    unsigned long IdxRange() const {return range * CHANNEL;}
    unsigned long Size() const {return range * CHANNEL * sizeof(float);}
};

class FaustCaster{
protected:
    void ToFaust(float** faustData, std::vector<float>& origin, const PCM_MIX_RANGE& pcm);
    void ToDefault(std::vector<float>& origin, float** faustData, const PCM_MIX_RANGE& pcm);
    void clearFaustData(float** faustData);
public:
    void FaustCompute();
};

// struct Filter{
//     std::vector<Values<EFFECT_VALUE, EFFECT_VALUE_INTERPOLATE, ON_OFF>> High;
//     std::vector<Values<EFFECT_VALUE, EFFECT_VALUE_INTERPOLATE, ON_OFF>> Low;
// };
