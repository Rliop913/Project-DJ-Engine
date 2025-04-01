#pragma once
#include "Faust_interpolate.hpp"

class OcsFilter_PDJE: public FaustInterpolate{
public:
    int ocsFilterHighLowSW;
    int middleFreq;
    int rangeFreqHalf;
    float bps;

    template<typename Duck>
    void copyDatas(Duck& dest){
        dest.ocsFilterHighLowSW = ocsFilterHighLowSW;
        dest.middleFreq = middleFreq;
        dest.rangeFreqHalf = rangeFreqHalf;
        dest.bps = bps;
    }

    template<typename Duck>
    void copySetting(Duck& dest){
        copyDatas(dest);
        copyInterpolates(dest);
    }
};