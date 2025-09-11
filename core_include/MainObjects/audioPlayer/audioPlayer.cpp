#include "audioPlayer.hpp"
#include "PDJE_LOG_SETTER.hpp"

extern void
FullPreRender_callback(ma_device  *pDevice,
                       void       *pOutput,
                       const void *pInput,
                       ma_uint32   frameCount);

extern void
HybridRender_callback(ma_device  *pDevice,
                      void       *pOutput,
                      const void *pInput,
                      ma_uint32   frameCount);
extern void
FullManualRender_callback(ma_device  *pDevice,
                          void       *pOutput,
                          const void *pInput,
                          ma_uint32   frameCount);

#include "MusicControlPanel.hpp"

void
audioPlayer::ContextInit()
{
    auto conf = ma_context_config_init();
    ma_context_init(NULL, 0, &conf, &ctxt);
    ctxt.threadPriority = ma_thread_priority_high;
}

ma_device_config
audioPlayer::DefaultInit(const unsigned int frameBufferSize)
{
    ma_device_config conf   = ma_device_config_init(ma_device_type_playback);
    conf.playback.format    = ma_format_f32;
    conf.playback.channels  = 2;
    conf.sampleRate         = 48000;
    conf.periodSizeInFrames = frameBufferSize;
    conf.performanceProfile = ma_performance_profile_low_latency;
    LFaust.resize(frameBufferSize);
    RFaust.resize(frameBufferSize);
    engineDatas.faustPcmPP[0] = LFaust.data();
    engineDatas.faustPcmPP[1] = RFaust.data();
    conf.pUserData            = reinterpret_cast<void *>(&engineDatas);
    ContextInit();
    return conf;
}

audioPlayer::audioPlayer(litedb            &db,
                         trackdata         &td,
                         const unsigned int frameBufferSize,
                         const bool         hasManual)
{
    auto conf = DefaultInit(frameBufferSize);
    if (hasManual) {
        conf.dataCallback = HybridRender_callback;
        engineDatas.FXManualPanel.emplace(SAMPLERATE);
        engineDatas.MusCtrPanel.emplace(SAMPLERATE);
    } else {
        conf.dataCallback = FullPreRender_callback;
    }

    if (!renderer.LoadTrack(db, td)) {
        critlog("failed to load track. from audioPlayer::audioPlayer(db, td "
                ",fbsize, hasmanual)");
        return;
    }
    engineDatas.pcmDataPoint = &renderer.rendered_frames.value();
    engineDatas.maxCursor    = renderer.rendered_frames->size() / CHANNEL;

    if (ma_device_init(&ctxt, &conf, &player) != MA_SUCCESS) {
        critlog("failed to init device. from audioPlayer::audioPlayer(db, td "
                ",fbsize, hasmanual)");
        return;
    }
}

audioPlayer::audioPlayer(const unsigned int frameBufferSize)
{
    ma_device_config conf = DefaultInit(frameBufferSize);

    conf.dataCallback = FullManualRender_callback;
    engineDatas.FXManualPanel.emplace(SAMPLERATE);
    engineDatas.MusCtrPanel.emplace(SAMPLERATE);

    if (ma_device_init(&ctxt, &conf, &player) != MA_SUCCESS) {
        critlog("failed to init device. from audioPlayer::audioPlayer(fbsize)");
    }
}

bool
audioPlayer::Activate()
{
    bool Res = ma_device_start(&player) == MA_SUCCESS;
    if (!Res) {
        critlog("failed to activate audioPlayer. from audioPlayer Activate");
    }
    return Res;
}

bool
audioPlayer::Deactivate()
{
    bool Res = ma_device_stop(&player) == MA_SUCCESS;
    if (!Res) {
        critlog(
            "failed to deactivate audioPlayer. from audioPlayer Deactivate");
    }
    return Res;
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
    return engineDatas.consumedFrames;
}

FXControlPanel *
audioPlayer::GetFXControlPanel(const UNSANITIZED &title)
{
    if (title == "__PDJE__MAIN__") {
        if (!engineDatas.FXManualPanel.has_value()) {
            engineDatas.FXManualPanel.emplace(48000);
        }
        return &engineDatas.FXManualPanel.value();
    } else {
        if (engineDatas.MusCtrPanel.has_value()) {
            return engineDatas.MusCtrPanel->getFXHandle(title);
        } else {
            critlog("failed to return fx control panel. from audioPlayer "
                    "GetFXControlPanel");
            return nullptr;
        }
    }
}

MusicControlPanel *
audioPlayer::GetMusicControlPanel()
{
    if (engineDatas.MusCtrPanel.has_value()) {
        return &(engineDatas.MusCtrPanel.value());
    } else {
        critlog("failed to return music control panel. from audioPlayer "
                "GetMusicControlPanel");
        return nullptr;
    }
}

PDJE_CORE_DATA_LINE
audioPlayer::PullOutDataLine()
{
    PDJE_CORE_DATA_LINE dline;
    dline.used_frame = &engineDatas.consumedFrames;
    dline.nowCursor  = &engineDatas.nowCursor;
    dline.maxCursor  = &engineDatas.maxCursor;
    if (!engineDatas.pcmDataPoint->empty()) {
        dline.preRenderedData = engineDatas.pcmDataPoint->data();
    }
    if (engineDatas.FXManualPanel.has_value()) {
        dline.fx = &engineDatas.FXManualPanel.value();
    }
    if (engineDatas.MusCtrPanel.has_value()) {
        dline.musp = &engineDatas.MusCtrPanel.value();
    }
    return dline;
}