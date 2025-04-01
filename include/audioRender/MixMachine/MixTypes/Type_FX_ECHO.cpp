#include "MixMachine.hpp"

template<>
bool
MixMachine::TypeWorks<TypeEnum::ECHO, FaustEffects>
(MixStruct& ms, FaustEffects& data, SIMD_FLOAT* Vec)
{
    if(!InterpolateInit(data.echoData, Vec, ms)){
        return false;
    }

    EightPointValues thirds(ms.RP.getThird().cStr());
    data.echoData.back().EchoBps = thirds.vals[0] / 60.0;
    data.echoData.back().EchoFeedback = thirds.vals[1];
    
    return true;
}