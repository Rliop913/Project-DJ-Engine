#include "MixMachine.hpp"

#undef HWY_TARGET_INCLUDE
#define HWY_TARGET_INCLUDE "MixMachine-inl.h"
#include "hwy/foreach_target.h"
#include <hwy/highway.h>

#ifndef HN_NAMESPACE
#define HN_NAMESPACE
namespace hn = hwy::HWY_NAMESPACE;
#endif

HWY_BEFORE_NAMESPACE();

namespace HWY_NAMESPACE{
void 
INTEGRATE_PCM_SIMD(
    SIMD_FLOAT& tempVec, 
    std::mutex& renderLock,
    std::vector<float>& rendered_out,
    MUSIC_CTR *& MC)
{
    const hn::ScalableTag<float> hwyFTag;
    auto laneSize = hn::Lanes(hwyFTag);
    auto times = tempVec.size() / laneSize;
    auto remained = tempVec.size() % laneSize;

    auto Tptr = tempVec.data();
    {
        std::lock_guard<std::mutex> locks(renderLock);
        if(rendered_out.size() < (MC->QDatas.pos.back().Gidx * CHANNEL)){
            rendered_out.resize((MC->QDatas.pos.back().Gidx * CHANNEL));
        }
        auto Rptr = rendered_out.data() + (MC->QDatas.pos.front().Gidx * CHANNEL);

        for(size_t L = 0; L < times; ++L){
            auto Tsimd = hn::Load(hwyFTag, Tptr);
            auto Rsimd = hn::LoadU(hwyFTag, Rptr);
            hn::StoreU(Rsimd + Tsimd, hwyFTag, Rptr);
            Tptr += laneSize;
            Rptr += laneSize;
        }
        for(size_t REM = 0; REM < remained; ++REM){
            (*(Rptr++)) += (*(Tptr++));
        }
    }
}
}


HWY_AFTER_NAMESPACE();