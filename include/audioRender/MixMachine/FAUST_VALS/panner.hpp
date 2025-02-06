#pragma once
#include "Faust_interpolate.hpp"

class Panner_PDJE: public FaustInterpolate{
protected:
    float bps;
    float PGain;
};