#include "audioCallbacks.hpp"
#include "FrameCalc.hpp"
#include "PDJE_Benchmark.hpp"
#include "audio_OS_impls.hpp"
#include <atomic>
#include <cstring>

std::optional<float *>
audioEngineDataStruct::getNowfPointer(const unsigned long frameCount)
{
    if ((nowCursor + frameCount) > maxCursor) {
        return std::nullopt;
    } else {
        return pcmDataPoint->data() + (nowCursor * CHANNEL);
    }
}

void
audioEngineDataStruct::CountUp(const unsigned long frameCount)
{
    nowCursor += frameCount;
    cacheSync = syncData.load(std::memory_order_acquire);
    cacheSync.consumed_frames += frameCount;
    cacheSync.microsecond                  = highres_clock.Get_MicroSecond();
    cacheSync.pre_calculated_unused_frames = get_unused_frames(backend_ptr);
    syncData.store(cacheSync, std::memory_order_release);
}

void
audioEngineDataStruct::GetAfterManFX(float              *pOutput,
                                     const unsigned long frameCount)
{
    if (!FXManualPanel.has_value()) {
        return;
    }
    auto getres = getNowfPointer(frameCount);
    if (!getres.has_value()) {
        return;
    }
    if (FXManualPanel->checkSomethingOn()) {
        toFaustStylePCM(faustPcmPP, getres.value(), frameCount);
        FXManualPanel->addFX(faustPcmPP, frameCount);
        toLRStylePCM(faustPcmPP, pOutput, frameCount);
    } else {

        memcpy(pOutput, getres.value(), frameCount * CHANNEL * sizeof(float));
    }
}

void
audioEngineDataStruct::Get(float *pOutput, unsigned long frameCount)
{
    auto getres = getNowfPointer(frameCount);
    if (!getres.has_value()) {
        return;
    }
    memcpy(pOutput, getres.value(), frameCount * CHANNEL * sizeof(float));
}

void
FullPreRender_callback(ma_device  *pDevice,
                       void       *pOutput,
                       const void *pInput,
                       ma_uint32   frameCount)
{
    WBCH("fullprerender callback calc start")
    auto rendered =
        reinterpret_cast<audioEngineDataStruct *>(pDevice->pUserData);
    rendered->CountUp(frameCount);
    rendered->Get(reinterpret_cast<float *>(pOutput), frameCount);
    WBCH("fullprerender callback calc end")
}

void
HybridRender_callback(ma_device  *pDevice,
                      void       *pOutput,
                      const void *pInput,
                      ma_uint32   frameCount)
{
    WBCH("hybridrender callback calc start")
    auto rendered =
        reinterpret_cast<audioEngineDataStruct *>(pDevice->pUserData);
    rendered->CountUp(frameCount);
    rendered->GetAfterManFX(reinterpret_cast<float *>(pOutput), frameCount);
    rendered->MusCtrPanel->GetPCMFrames(reinterpret_cast<float *>(pOutput),
                                        frameCount);
    WBCH("hybridrender callback calc end")
}

void
FullManualRender_callback(ma_device  *pDevice,
                          void       *pOutput,
                          const void *pInput,
                          ma_uint32   frameCount)
{
    WBCH("full manual render callback calc start")
    auto Data = reinterpret_cast<audioEngineDataStruct *>(pDevice->pUserData);
    Data->CountUp(frameCount);
    Data->MusCtrPanel->GetPCMFrames(reinterpret_cast<float *>(pOutput),
                                    frameCount);
    WBCH("full manual render callback calc end")
}