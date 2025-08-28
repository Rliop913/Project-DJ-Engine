#include "MixMachine.hpp"

template <>
bool
MixMachine::TypeWorks<TypeEnum::FLANGER, FaustEffects>(MixStruct    &ms,
                                                       FaustEffects &data,
                                                       SIMD_FLOAT   *Vec)
{
    if (!InterpolateInit(data.flangerData, Vec, ms)) {
        return false;
    }

    TRY(data.flangerData.back().bps =
            std::stof(ms.RP.getThird().cStr()) / 60.0;)

    return true;
}