#pragma once

#include <optional>

#include <miniaudio.h>
#include "ManualMix.hpp"



struct audioEngineDataStruct{
    float* faustPcmPP[2];
    std::optional<FXControlPannel> FXManualPannel;
    std::vector<float>* pcmDataPoint;
    unsigned long long nowCursor = 0;
    unsigned long long maxCursor = 0;
    unsigned long long consumedFrames = 0;
    
    inline std::optional<float*>
    getNowfPointer(unsigned long frameCount);

    inline void CountUp(unsigned long frameCount);

    void GetAfterManFX(float* pOutput, unsigned long frameCount);

    void Get(float* pOutput, unsigned long frameCount);
 
};

