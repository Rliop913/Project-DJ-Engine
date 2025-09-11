#include "MixMachine.hpp"

template <>
bool
MixMachine::TypeWorks<TypeEnum::OSC_FILTER, FaustEffects>(MixStruct    &ms,
                                                          FaustEffects &data,
                                                          SIMD_FLOAT   *Vec)
{
    if (!InterpolateInit(data.ocsFilterData, Vec, ms)) {
        return false;
    }

    switch (ms.RP.getDetails()) {
    case DetailEnum::HIGH:
        data.ocsFilterData.back().ocsFilterHighLowSW = 0;
        break;

    case DetailEnum::LOW:
        data.ocsFilterData.back().ocsFilterHighLowSW = 1;
        break;
    default:
        break;
    }

    EightPointValues thirds(ms.RP.getThird().cStr());
    data.ocsFilterData.back().bps           = thirds.vals[0] / 60.0;
    data.ocsFilterData.back().middleFreq    = thirds.vals[1];
    data.ocsFilterData.back().rangeFreqHalf = thirds.vals[2];

    return true;
}