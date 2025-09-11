#include "MixMachine.hpp"

template <>
bool
MixMachine::TypeWorks<TypeEnum::PANNER, FaustEffects>(MixStruct    &ms,
                                                      FaustEffects &data,
                                                      SIMD_FLOAT   *Vec)
{
    if (!InterpolateInit(data.pannerData, Vec, ms)) {
        return false;
    }

    EightPointValues thirds(ms.RP.getThird().cStr());
    data.pannerData.back().bps   = thirds.vals[0] / 60.0;
    data.pannerData.back().PGain = thirds.vals[1];

    return true;
}