#pragma once

#include <miniaudio.h>
#include "MixMachine.hpp"
#include "ManualMix.hpp"
#include "audioRender.hpp"
#include "audioCallbacks.hpp"
class audioPlayer{
private:
    ma_device player;
    audioRender renderer;
    std::vector<float> LFaust;
    std::vector<float> RFaust;
    audioEngineDataStruct engineDatas;
public:
    bool Activate();
    bool Deactivate();
    void ChangeCursorPos(unsigned long long pos);
    unsigned long long GetConsumedFrames();
    FXControlPannel* GetFXControlPannel();

    //use pre-rendered audio, use manual or not
    audioPlayer(litedb& db, trackdata& td, unsigned int frameBufferSize, bool hasManual = false);
    //completely manual play
    audioPlayer(unsigned int frameBufferSize);

    ~audioPlayer();
};