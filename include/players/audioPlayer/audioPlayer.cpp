#include "audioPlayer.hpp"


extern void FullPreRender_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);

extern void HybridRender_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);
extern void FullManualRender_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);


#include "MusicControlPannel.hpp"


void* 
MaSIMDAlloc(size_t sz, void *pUserData)
{
    pUserData = new SIMD_FLOAT;
    reinterpret_cast<SIMD_FLOAT*>(pUserData)->resize(sz / sizeof(float));
    return reinterpret_cast<SIMD_FLOAT*>(pUserData)->data();
}

void 
MaSIMDFree(void* p, void* pUserData)
{
    auto SIMDV = reinterpret_cast<SIMD_FLOAT*>(pUserData);
    delete SIMDV;
}

void* 
MaSIMDReAlloc(void *p, size_t sz, void *pUserData)
{
    auto SIMDV = reinterpret_cast<SIMD_FLOAT*>(pUserData);
    SIMDV->resize(sz / sizeof(float));
    return SIMDV;
}

void
audioPlayer::ContextInit()
{
    auto conf = ma_context_config_init();
    ma_context_init(NULL, 0, &conf, &ctxt);
    ctxt.threadPriority = ma_thread_priority_high;
    ma_allocation_callbacks mac;
    mac.onFree = MaSIMDFree;
    mac.onMalloc = MaSIMDAlloc;
    mac.onRealloc = MaSIMDReAlloc;
    ctxt.allocationCallbacks = mac;
}


ma_device_config
audioPlayer::DefaultInit(const unsigned int frameBufferSize)
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
    conf.pUserData = reinterpret_cast<void*>(&engineDatas);
    ContextInit();
    return conf;
}

audioPlayer::audioPlayer(litedb& db, trackdata& td, const unsigned int frameBufferSize, const bool hasManual)
{
    auto conf = DefaultInit(frameBufferSize);
    if(hasManual){
        conf.dataCallback = HybridRender_callback;
        engineDatas.FXManualPannel.emplace(SAMPLERATE);
        engineDatas.MusCtrPannel.emplace(SAMPLERATE);
    }
    else{
        conf.dataCallback = FullPreRender_callback;
    }
    
    if(!renderer.LoadTrack(db, td)){
        throw "Failed to load track data";
    }
    engineDatas.pcmDataPoint = &renderer.rendered_frames.value();
    engineDatas.maxCursor = renderer.rendered_frames->size();
    
    if(ma_device_init(&ctxt, &conf, &player) != MA_SUCCESS){
        throw "Failed to init player";
    }
    
}

audioPlayer::audioPlayer(const unsigned int frameBufferSize)
{
    ma_device_config conf = DefaultInit(frameBufferSize);
    
    conf.dataCallback = FullManualRender_callback;
    engineDatas.FXManualPannel.emplace(SAMPLERATE);
    engineDatas.MusCtrPannel.emplace(SAMPLERATE);
    

    if(ma_device_init(&ctxt, &conf, &player) != MA_SUCCESS){
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
    ma_context_uninit(&ctxt);
    
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
audioPlayer::GetFXControlPannel(const std::string& title)
{
    if(title == "__PDJE__MAIN__"){
        EXCEPTION_DIVE_TO_MAIN:
        if(!engineDatas.FXManualPannel.has_value()){
            engineDatas.FXManualPannel.emplace(48000);
        }
        return &engineDatas.FXManualPannel.value();
    }
    else{
        if(engineDatas.MusCtrPannel.has_value()){
            return engineDatas.MusCtrPannel->getFXHandle(title);
        }
        else{
            goto EXCEPTION_DIVE_TO_MAIN;
        }
    }
}
