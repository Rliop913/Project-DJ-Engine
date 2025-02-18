#include "MixMachine.hpp"

template<>
bool
MixMachine::TypeWorks<TypeEnum::FILTER, FaustEffects>
(MixStruct& ms, FaustEffects& data, std::vector<float>* Vec)
{
    if(!InterpolateInit(data.filterData, Vec, ms)){
        return false;
    }
    // data.filterData.emplace_back(Vec, ms.frame_in, ms.frame_out);

    // TRY(
    //     data.filterData.back().selectInterpolator =
    //     std::stoi(ms.RP.getFirst().cStr());
    // )
    // if(data.filterData.back().selectInterpolator == InterpolateType::FLAT){
    //     TRY(
    //         data.filterData.back().vZero =
    //         std::stof(ms.RP.getSecond().cStr());
    //     )
    // }
    // else{
    //     EightPointValues EPV(ms.RP.getSecond().cStr());
    //     data.filterData.back().v1 = EPV.vals[0];
    //     data.filterData.back().v2 = EPV.vals[1];
    //     data.filterData.back().v3 = EPV.vals[2];
    //     data.filterData.back().v4 = EPV.vals[3];
    //     data.filterData.back().v5 = EPV.vals[4];
    //     data.filterData.back().v6 = EPV.vals[5];
    //     data.filterData.back().v7 = EPV.vals[6];
    //     data.filterData.back().v8 = EPV.vals[7];
    // }
    switch (ms.RP.getDetails())
    {
    case DetailEnum::HIGH:
        data.filterData.back().HLswitch = 0;
        break;
    case DetailEnum::LOW:
        data.filterData.back().HLswitch = 1;
        break;
    default:
        break;
    }
    return true;
    // data.filterData.back().frames = ms.frame_out = ms.frame_in;
    // data.filterData.back().timerActive = 0;
}