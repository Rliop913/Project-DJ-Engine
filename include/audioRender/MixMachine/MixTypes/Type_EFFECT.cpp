#include "MixMachine.hpp"

#define TRY(CODE)\
try\
{\
    CODE\
}\
catch(...)\
{\
    return false;\
}


template<>
bool
MixMachine::TypeWorks<TypeEnum::COMPRESSOR, FaustEffects>
(MixStruct& ms, FaustEffects& data, std::vector<float>* Vec)
{
    data.compressorData.emplace_back(Vec, ms.frame_in, ms.frame_out);
    TRY(
        data.compressorData.back().strength =
        std::stof(ms.RP.getFirst().cStr());
        )
    
    EightPointValues tk(ms.RP.getSecond().cStr());
    EightPointValues ar(ms.RP.getThird().cStr());
    data.compressorData.back().threshDB = tk.vals[0];
    data.compressorData.back().kneeDB = tk.vals[1];
    data.compressorData.back().attackMS = ar.vals[0];
    data.compressorData.back().releaseMS = ar.vals[1];
    return true;
}

template<>
bool
MixMachine::TypeWorks<TypeEnum::EQ, FaustEffects>
(MixStruct& ms, FaustEffects& data, std::vector<float>* Vec)
{
    data.eqData.emplace_back(Vec, ms.frame_in, ms.frame_out);
    
    TRY(
        data.eqData.back().selectInterpolator =
        std::stoi(ms.RP.getFirst().cStr());
    )
    if(data.eqData.back().selectInterpolator == 0){
        TRY(
            data.eqData.back().vZero = 
            std::stof(ms.RP.getSecond().cStr());
        )
    }
    else{
        EightPointValues EPV(ms.RP.getSecond().cStr());
        data.eqData.back().v1 = EPV.vals[0];
        data.eqData.back().v2 = EPV.vals[1];
        data.eqData.back().v3 = EPV.vals[2];
        data.eqData.back().v4 = EPV.vals[3];
        data.eqData.back().v5 = EPV.vals[4];
        data.eqData.back().v6 = EPV.vals[5];
        data.eqData.back().v7 = EPV.vals[6];
        data.eqData.back().v8 = EPV.vals[7];
    }
    switch (ms.RP.getDetails())
    {
    case DetailEnum::HIGH:
        data.eqData.back().EQSelect = 0;
        break;
    case DetailEnum::MID:
        data.eqData.back().EQSelect = 1;
        break;
    case DetailEnum::LOW:
        data.eqData.back().EQSelect = 2;
        break;
    default:
        break;
    }
    data.eqData.back().frames = ms.frame_out - ms.frame_in;
    data.eqData.back().timerActive = 0;//active
    data.eq.managingClass.instanceClear();//set timer 0
    return true; //need to refactor
}