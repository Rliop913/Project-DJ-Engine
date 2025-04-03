#pragma once

#include <miniaudio.h>
#include "MixMachine.hpp"
#include "ManualMix.hpp"
#include "audioRender.hpp"
#include "audioCallbacks.hpp"

class audioPlayer{
private:
    ma_device player;
    ma_context ctxt;
    audioRender renderer;
    std::vector<float> LFaust;
    std::vector<float> RFaust;
    audioEngineDataStruct engineDatas;
    ma_device_config DefaultInit(const unsigned int frameBufferSize);
    void ContextInit();
public:
    bool Activate();
    bool Deactivate();
    void ChangeCursorPos(unsigned long long pos);
    unsigned long long GetConsumedFrames();
    FXControlPannel* GetFXControlPannel(const std::string& title = "__PDJE__MAIN__");
    MusicControlPannel* GetMusicControlPannel();
    //use pre-rendered audio, use manual or not
    audioPlayer(litedb& db, trackdata& td, const unsigned int frameBufferSize, const bool hasManual = false);
    //completely manual play
    audioPlayer(const unsigned int frameBufferSize);

    ~audioPlayer();
};