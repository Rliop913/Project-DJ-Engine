#include "MixMachine.hpp"


template<>
bool
MixMachine::TypeWorks<TypeEnum::PHASER, FaustEffects>
(MixStruct& ms, FaustEffects& data, std::vector<float>* Vec)
{
    if(!InterpolateInit(data.phaserData, Vec, ms)){
        return false;
    }
    TRY(
        data.phaserData.back().bps = std::stof(ms.RP.getThird().cStr()) / 60.0;
    )
    return true;
}