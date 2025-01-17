#pragma once

#include "FAUST_FILTERS.hpp"
#include "MixTranslator.hpp"
#include "dbRoot.hpp"

class FX_Filter{
private:
    litedb db;
    FilterFAUST FF;
    std::vector<MixStruct>& operator<=(std::vector<MixStruct>& data)
    {
        for(auto i : data){
            if(i.RP.getType() == TypeEnum::LOAD){
                musdata mm;
                mm.title = i.RP.getFirst();
                mm.composer = i.RP.getSecond();
                mm.bpm = std::stod(i.RP.getThird().cStr());
                auto res = db << mm;
                auto samplePos = std::stoul(res.value()[0].firstBar);
                auto local_start = i.frame_in;
                
            }
        }
        
        for(auto i : data){
            i.frame_in;
        }
    }
    float* operator<<(float* pcmFrames)
    {
        FF.compute()
    }
    
};