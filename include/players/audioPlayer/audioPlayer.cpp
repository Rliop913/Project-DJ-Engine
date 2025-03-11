#include "audioPlayer.hpp"


extern void FullPreRender_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);

extern void HybridRender_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);
extern void FullManualRender_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);

audioPlayer::audioPlayer(litedb& db, trackdata& td, unsigned int frameBufferSize, bool hasManual)
{
    ma_device_config conf = ma_device_config_init(ma_device_type_playback);
    conf.playback.format = ma_format_f32;
    conf.playback.channels = 2;
    conf.sampleRate = 48000;
    conf.periodSizeInFrames = frameBufferSize;
    conf.performanceProfile = ma_performance_profile_low_latency;
    LFaust.resize(frameBufferSize);
    RFaust.resize(frameBufferSize);
    engineDatas.faustPcmPP[0] = LFaust.data();
    engineDatas.faustPcmPP[1] = RFaust.data();
    if(hasManual){
        conf.dataCallback = HybridRender_callback;
        engineDatas.FXManualPannel.emplace(48000);
    }
    else{
        conf.dataCallback = FullPreRender_callback;
    }
    
    if(!renderer.LoadTrack(db, td)){
        throw "Failed to load track data";
    }
    engineDatas.pcmDataPoint = &renderer.rendered_frames.value();
    engineDatas.maxCursor = renderer.rendered_frames->size();
    conf.pUserData = reinterpret_cast<void*>(&engineDatas);
    if(ma_device_init(NULL, &conf, &player) != MA_SUCCESS){
        throw "Failed to init player";
    }
}

audioPlayer::audioPlayer(unsigned int frameBufferSize)
{
    ma_device_config conf = ma_device_config_init(ma_device_type_playback);
    conf.playback.format = ma_format_f32;
    conf.playback.channels = 2;
    conf.sampleRate = 48000;
    conf.periodSizeInFrames = frameBufferSize;
    conf.performanceProfile = ma_performance_profile_low_latency;
    conf.dataCallback = FullManualRender_callback;
    LFaust.resize(frameBufferSize);
    RFaust.resize(frameBufferSize);
    engineDatas.faustPcmPP[0] = LFaust.data();
    engineDatas.faustPcmPP[1] = RFaust.data();
    engineDatas.FXManualPannel.emplace(48000);
    
    conf.pUserData = reinterpret_cast<void*>(&engineDatas);
    
    if(ma_device_init(NULL, &conf, &player) != MA_SUCCESS){
        throw "Failed to init player";
    }
    
}

bool
audioPlayer::Activate()
{
    return ma_device_start(&player) == MA_SUCCESS;
}

bool
audioPlayer::Deactivate()
{
    return ma_device_stop(&player) == MA_SUCCESS;
}


audioPlayer::~audioPlayer()
{
    ma_device_uninit(&player);
    
}

void
audioPlayer::ChangeCursorPos(unsigned long long pos)
{
    engineDatas.nowCursor = pos;
}

unsigned long long
audioPlayer::GetConsumedFrames()
{
    return  engineDatas.consumedFrames;
}

FXControlPannel*
audioPlayer::GetFXControlPannel()
{
    if(!engineDatas.FXManualPannel.has_value()){
        engineDatas.FXManualPannel.emplace(48000);
    }
    return &engineDatas.FXManualPannel.value();
}
