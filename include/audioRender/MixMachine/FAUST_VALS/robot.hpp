#pragma once
#include "Faust_interpolate.hpp"

class Robot_PDJE: public FaustInterpolate {
public:
    int robotFreq;

    template<typename Duck>
    void copyDatas(Duck& dest){
        dest.robotFreq = robotFreq;
    }

    template<typename Duck>
    void copySetting(Duck& dest){
        copyDatas(dest);
        copyInterpolates(dest);
    }
};