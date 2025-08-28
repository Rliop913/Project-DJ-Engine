#include "MixMachine.hpp"

template <>
bool
MixMachine::TypeWorks<TypeEnum::TRANCE, FaustEffects>(MixStruct    &ms,
                                                      FaustEffects &data,
                                                      SIMD_FLOAT   *Vec)
{
    if (!InterpolateInit(data.tranceData, Vec, ms)) {
        return false;
    }

    EightPointValues thirds(ms.RP.getThird().cStr());
    data.tranceData.back().bps  = thirds.vals[0] / 60.0;
    data.tranceData.back().gain = thirds.vals[1];

    return true;
}