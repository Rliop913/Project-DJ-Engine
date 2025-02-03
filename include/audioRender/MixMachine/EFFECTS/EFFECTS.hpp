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

class FilterVal{
protected:
    int InterpolateSW = 1;// 1 if off
    float HighFreq = 100;
    float LowFreq = 23000;

    int LowFilterSW = 1;
    int HighFilterSW = 1;

    int InterpolateSamples;
public:
    std::optional<unsigned long> StartPos;
    
    void Interpolate(){
        InterpolateSW = 0;
    }
    void NoInterpolate(){
        InterpolateSW = 1;
    }
    void HighOn(){
        HighFilterSW = 0;
    }
    void HighOff(){
        HighFilterSW = 1;
    }
    void LowOn(){
        LowFilterSW = 0;
    }
    void LowOff(){
        LowFilterSW = 1;
    }
    void setHighFreq(float freq){
        HighFreq = freq;
    }
    void setLowFreq(float freq){
        LowFreq = freq;
    }
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

class RollVal{
protected:
    int RollSwitch = 1;
    float RollBpm = 60;
    float RollPower = 1.0;
public:
    void on(){
        RollSwitch = 0;
    }
    void off(){
        RollSwitch = 1;
    }
    void setBpm(float bpm){
        RollBpm = bpm;
    }
    void setpow(float pow){
        pow = pow > 1.0 ? 1.0 : pow;
        pow = pow < 0.0 ? 0.0 : pow;
        RollPower = pow;
    }
};


// struct EFFECTS{
//     Filter filters;
//     EQ eq;
//     Distortion distortions;
//     Echo echos;
//     LFO_Filter lfo_filters;
//     Flanger flangers;
//     Phaser phasers;
//     Trance trances;
//     Panner panners;
//     Roll rolls;
// };
