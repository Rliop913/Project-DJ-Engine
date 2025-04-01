#pragma once
#include "DeckData.hpp"




class FaustInterpolate{
public:
    int selectInterpolator;
    float v1;
    float v2;
    float v3;
    float v4;
    float v5;
    float v6;
    float v7;
    float v8;
    float vZero;
    int frames;
    int timerActive;
    
    template<typename Duck>
    void copyInterpolates(Duck& dest){
        dest.selectInterpolator = selectInterpolator;
        dest.v1 = v1;
        dest.v2 = v2;
        dest.v3 = v3;
        dest.v4 = v4;
        dest.v5 = v5;
        dest.v6 = v6;
        dest.v7 = v7;
        dest.v8 = v8;
        dest.vZero = vZero;
        dest.frames = frames;
        dest.timerActive = timerActive;
    }

    template<typename Duck>
    void copySetting(Duck& dest){
        copyInterpolates(dest);
    }
};