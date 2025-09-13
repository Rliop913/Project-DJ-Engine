#pragma once

#include "ManualMix.hpp"
#include "MixMachine.hpp"
#include "PDJE_Core_DataLine.hpp"
#include "PDJE_EXPORT_SETTER.hpp"
#include "audioCallbacks.hpp"
#include "audioRender.hpp"
#include <miniaudio.h>
/**
 * @brief The music handler class
 * this is the music handler class.
 * you can play/stop music, controlling fx, attach other music in realtime
 * manually or getting music's playing position and consumed frames.
 */
class PDJE_API audioPlayer {
  private:
    ma_device  player;
    ma_context ctxt;

    /// the audio renderer object.
    audioRender renderer;

    /// the separated pcm frame for the faust. the left side.
    std::vector<float> LFaust;

    /// the separated pcm frame for the faust. the right side.
    std::vector<float> RFaust;

    /// the core datas.
    audioEngineDataStruct engineDatas;

    /// @brief the default initializer.
    /// @param frameBufferSize
    /// @return
    ma_device_config
    DefaultInit(const unsigned int frameBufferSize);

    /// @brief the context initializer.
    void
    ContextInit();

  public:
    /// the handler status. check when something wrong.
    /// do not change this manually.
    std::string STATUS = "OK";

    /// the status getter for binded languages.
    const std::string
    GetStatus()
    {
        return STATUS;
    }

    /// Plays music.
    bool
    Activate();

    /// Stops music
    bool
    Deactivate();

    /// change the prerendered music's cursor(the playing position).
    void
    ChangeCursorPos(unsigned long long pos);

    /// @brief gets consumed frames.
    /// with this, you can get how many frames are consumed and music's playback
    /// time. to get playback time, divide the returned value with 48000. The
    /// result is Seconds.
    /// @return consumed frames
    unsigned long long
    GetConsumedFrames();
    /**
     * @brief fx controller getter
     * this returns the fx controller. with this, you can control the fx in
     * realtime manually.
     * @param title the music to control. "__PDJE__MAIN__" means the prerendered
     * music.
     * @return FXControlPanel* but the "title" doesn't exists, it returns
     * nullptr.
     */
    FXControlPanel *
    GetFXControlPanel(const UNSANITIZED &title = "__PDJE__MAIN__");

    /**
     * @brief music controller getter
     * this returns the music controller. with this, you can load music, stop
     * music in realtime manually.
     * @return MusicControlPanel* if something wrong, it returns nullptr.
     */
    MusicControlPanel *
    GetMusicControlPanel();
    /**
     * @brief audio handler constructor. specified for pre-render mode.
     *
     * @param db litedb(Root database) object
     * @param td track_data
     * @param frameBufferSize the buffer size.
     * @param hasManual enable manual features. if don't, you can't use FX
     * controller, Music controller.
     */
    audioPlayer(litedb            &db,
                trackdata         &td,
                const unsigned int frameBufferSize,
                const bool         hasManual = false);
    /**
     * @brief audio handler constructor. specified for manual render mode.
     *
     * @param frameBufferSize the buffer size.
     */
    audioPlayer(const unsigned int frameBufferSize);

    PDJE_CORE_DATA_LINE
    PullOutDataLine();

    ~audioPlayer();
};