#include "MixMachine.hpp"


template<>
bool
MixMachine::TypeWorks<TypeEnum::ROLL, FaustEffects>
(MixStruct& ms, FaustEffects& data, SIMD_FLOAT* Vec)
{
    if(!InterpolateInit(data.rollData, Vec, ms)){
        return false;
    }

    TRY(
        data.rollData.back().RollBpm = std::stof(ms.RP.getThird().cStr());
    )
    
    return true;
}