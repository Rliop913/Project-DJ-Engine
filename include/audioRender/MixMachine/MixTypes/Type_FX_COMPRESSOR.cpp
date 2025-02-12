#include "MixMachine.hpp"


template<>
bool
MixMachine::TypeWorks<TypeEnum::COMPRESSOR, FaustEffects>
(MixStruct& ms, FaustEffects& data, std::vector<float>* Vec)
{
    data.compressorData.emplace_back(Vec, ms.frame_in, ms.frame_out);
    TRY(
        data.compressorData.back().strength =
        std::stof(ms.RP.getFirst().cStr());
        )
    
    EightPointValues tk(ms.RP.getSecond().cStr());
    EightPointValues ar(ms.RP.getThird().cStr());
    data.compressorData.back().threshDB = tk.vals[0];
    data.compressorData.back().kneeDB = tk.vals[1];
    data.compressorData.back().attackMS = ar.vals[0];
    data.compressorData.back().releaseMS = ar.vals[1];
    return true;
}
