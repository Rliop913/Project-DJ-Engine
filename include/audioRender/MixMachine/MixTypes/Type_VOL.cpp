#include "MixMachine.hpp"


template<>
bool
MixMachine::TypeWorks<TypeEnum::VOL, FaustEffects>
(MixStruct& ms, FaustEffects& data, std::vector<float>* Vec)
{
    if(!InterpolateInit(data.volData, Vec, ms)){
        return false;
    }
    
    return true;
}