#include "MixMachine.hpp"

template <>
bool
MixMachine::TypeWorks<TypeEnum::FILTER, FaustEffects>(MixStruct    &ms,
                                                      FaustEffects &data,
                                                      SIMD_FLOAT   *Vec)
{
    if (!InterpolateInit(data.filterData, Vec, ms)) {
        return false;
    }

    switch (ms.RP.getDetails()) {
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
}