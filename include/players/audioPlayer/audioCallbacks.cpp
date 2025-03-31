#include "audioCallbacks.hpp"
#include "FrameCalc.hpp"
#include <cstring>



std::optional<float*>
audioEngineDataStruct::getNowfPointer(const unsigned long frameCount)
{
    if((nowCursor + frameCount) > maxCursor){
        return std::nullopt;
    }
    else{
        return pcmDataPoint->data() + (nowCursor * CHANNEL);
    }
}

void
audioEngineDataStruct::CountUp(const unsigned long frameCount)
{
    nowCursor += frameCount;
    consumedFrames += frameCount;
}

void
audioEngineDataStruct::GetAfterManFX(SIMD_FLOAT* pOutput, const unsigned long frameCount)
{
    if(!FXManualPannel.has_value()){
        return;
    }
    auto getres = getNowfPointer(frameCount);
    if(!getres.has_value()){
        return;
    }
    if(FXManualPannel->checkSomethingOn()){
        toFaustStylePCM(faustPcmPP, getres.value(), frameCount);
        FXManualPannel->addFX(faustPcmPP, frameCount);
        toLRStylePCM(faustPcmPP, pOutput->data(), frameCount);
    }
    else{

        memcpy(pOutput->data(), getres.value(), frameCount * CHANNEL * sizeof(float));
    }
}

void
audioEngineDataStruct::Get(SIMD_FLOAT* pOutput, unsigned long frameCount)
{
    auto getres = getNowfPointer(frameCount);
    if(!getres.has_value()){
        return;
    }
    memcpy(pOutput->data(), getres.value(), frameCount * CHANNEL * sizeof(float));
}





void
FullPreRender_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
	auto rendered = reinterpret_cast<audioEngineDataStruct*>(pDevice->pUserData);
    rendered->Get(reinterpret_cast<SIMD_FLOAT*>(pOutput), frameCount);
    rendered->CountUp(frameCount);
}

void
HybridRender_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
    auto rendered = reinterpret_cast<audioEngineDataStruct*>(pDevice->pUserData);
    rendered->GetAfterManFX(reinterpret_cast<SIMD_FLOAT*>(pOutput), frameCount);
    rendered->CountUp(frameCount);
}

void
FullManualRender_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
    auto Data = reinterpret_cast<audioEngineDataStruct*>(pDevice->pUserData);
    Data->MusCtrPannel->GetPCMFrames(reinterpret_cast<SIMD_FLOAT*>(pOutput), frameCount);
    
    
}