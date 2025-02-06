#pragma once
#include "Faust_interpolate.hpp"

class OcsFilter_PDJE: public FaustInterpolate{
protected:
    int ocsFilterHighLowSW;
    int middleFreq;
    int rangeFreqHalf;
    float bps;
};