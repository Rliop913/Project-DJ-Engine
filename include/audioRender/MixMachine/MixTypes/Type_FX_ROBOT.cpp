#include "MixMachine.hpp"


template<>
bool
MixMachine::TypeWorks<TypeEnum::ROBOT, FaustEffects>
(MixStruct& ms, FaustEffects& data, std::vector<float>* Vec)
{
    if(!InterpolateInit(data.robotData, Vec, ms)){
        return false;
    }
    TRY(
        data.robotData.back().robotFreq = std::stof(ms.RP.getThird().cStr());
    )
    
    return true;
}