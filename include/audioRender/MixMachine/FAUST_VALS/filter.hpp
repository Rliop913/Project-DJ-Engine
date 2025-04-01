#pragma once
#include "Faust_interpolate.hpp"

class Filter_PDJE: public FaustInterpolate{
public:
    int HLswitch;
    
    template<typename Duck>
    void copyDatas(Duck& dest){
        dest.HLswitch = HLswitch;
    }

    template<typename Duck>
    void copySetting(Duck& dest){
        copyDatas(dest);
        copyInterpolates(dest);
    }
};