#include "MixMachine.hpp"

template <>
bool
MixMachine::TypeWorks<TypeEnum::DISTORTION, FaustEffects>(MixStruct    &ms,
                                                          FaustEffects &data,
                                                          SIMD_FLOAT   *Vec)
{
    if (!InterpolateInit(data.distortionData, Vec, ms)) {
        return false;
    }
    return true;
}