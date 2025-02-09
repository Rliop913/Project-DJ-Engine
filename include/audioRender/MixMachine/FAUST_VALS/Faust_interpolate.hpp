#pragma once
#include "DeckData.hpp"




class FaustInterpolate{
public:
    int selectInterpolator;
    int v1;
    int v2;
    int v3;
    int v4;
    int v5;
    int v6;
    int v7;
    int v8;
    int vZero;
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