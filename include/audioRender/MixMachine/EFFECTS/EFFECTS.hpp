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

class Pipelines{
public:
    float* operator<<(float* pcmFrames);
    std::vector<MBData::Reader>& operator<=(std::vector<MBData::Reader>& data);
};


struct Filter{
    std::vector<Values<EFFECT_VALUE, EFFECT_VALUE_INTERPOLATE, ON_OFF>> High;
    std::vector<Values<EFFECT_VALUE, EFFECT_VALUE_INTERPOLATE, ON_OFF>> Low;
};

class FilterVal{
protected:
    int InterpolateSW = 1;// 1 if off
    float HighFreq = 100;
    float LowFreq = 23000;

    int LowFilterSW = 1;
    int HighFilterSW = 1;

    int InterpolateSamples;
public:
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
