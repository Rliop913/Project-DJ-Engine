#pragma once
#include "Faust_interpolate.hpp"

class Filter_PDJE: public FaustInterpolate{
public:
    int LowFilterSW;
    int HighFilterSW;
    
    template<typename Duck>
    void copyDatas(Duck& dest){
        dest.LowFilterSW = LowFilterSW;
        dest.HighFilterSW = HighFilterSW;
    }

    template<typename Duck>
    void copySetting(Duck& dest){
        copyDatas(dest);
        copyInterpolates(dest);
    }
};