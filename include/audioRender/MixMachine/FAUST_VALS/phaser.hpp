#pragma once
#include "Faust_interpolate.hpp"

class Phaser_PDJE : public FaustInterpolate{
public:
    float bps;
    
    template<typename Duck>
    void copyDatas(Duck& dest){
        dest.bps = bps;
    }

    template<typename Duck>
    void copySetting(Duck& dest){
        copyDatas(dest);
        copyInterpolates(dest);
    }
};