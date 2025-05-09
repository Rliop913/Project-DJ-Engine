
#include "MusicControlPannel.hpp"

#undef HWY_TARGET_INCLUDE
#define HWY_TARGET_INCLUDE "MusicControlPannel-inl.h"
#include "hwy/foreach_target.h"
#include <hwy/highway.h>

#ifndef HN_NAMESPACE
#define HN_NAMESPACE
namespace hn = hwy::HWY_NAMESPACE;
#endif


HWY_BEFORE_NAMESPACE();


namespace HWY_NAMESPACE {

bool
GetPCMFramesSIMD(
    SIMD_FLOAT& tempFrames,
    std::vector<float>& L,
    std::vector<float>& R,
    float** FaustStyle,
    LOADS& deck,
    float* array, 
    const unsigned long FrameSize)
{
    const unsigned long long RAWFrameSize = FrameSize * CHANNEL;
    
    tempFrames.resize(RAWFrameSize);
    L.resize(FrameSize);
    R.resize(FrameSize);
    FaustStyle[0] = L.data();
    FaustStyle[1] = R.data();
    const hn::ScalableTag<float> hwyFTag;
    auto laneSize = hn::Lanes(hwyFTag);
    auto times = RAWFrameSize / laneSize;
    auto remained = RAWFrameSize % laneSize;

    for(auto& i : deck){
        if(i.second.play){
            
            if(ma_decoder_read_pcm_frames(&i.second.dec, tempFrames.data(), FrameSize, NULL) != MA_SUCCESS){
                return false;
            }
            toFaustStylePCM(FaustStyle, tempFrames.data(), FrameSize);
            i.second.fxP->addFX(FaustStyle, FrameSize);
            toLRStylePCM(FaustStyle, tempFrames.data(), FrameSize);
            
            float* opoint = array;
            float* tpoint = tempFrames.data();
            
            for(size_t j = 0; j < times; ++j){
                auto simdtemp = hn::Load(hwyFTag, tpoint);
                auto simdorigin = hn::LoadU(hwyFTag, opoint);
                auto res = simdtemp + simdorigin;
                hn::StoreU(res, hwyFTag, opoint);
                opoint += laneSize;
                tpoint += laneSize;
            }
            
            for(size_t j=0; j<remained; ++j){
                (*(opoint++)) += (*(tpoint++));
            }
        }
    }
    return true;
}
}



HWY_AFTER_NAMESPACE();