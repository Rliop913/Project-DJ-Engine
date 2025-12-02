#pragma once

#include "FrameCalc.hpp"
#include "PreLoadedMusic.hpp"

#include <chrono>
#include <map>

#include "ManualMix.hpp"
#include "PDJE_EXPORT_SETTER.hpp"
#include "PredictMusic.hpp"
#include "SoundTouch.h"
#include "dbRoot.hpp"
#include "fileNameSanitizer.hpp"
#include <array>
#include <filesystem>
namespace fs = std::filesystem;
// #undef HWY_TARGET_INCLUDE
// #define HWY_TARGET_INCLUDE "MusicControlPanel-inl.h"
// #include "hwy/foreach_target.h"
// #include <hwy/highway.h>

using LOADED_LIST          = std::vector<std::string>;
constexpr int PREDICT_SIZE = 10;
/**
 * @brief Music handling data structure for manual mode
 *
 */
struct MusicOnDeck {
    bool                                       play = false;
    PreLoadedMusic                             loaded;
    std::optional<PredictBuffer<PREDICT_SIZE>> pb;
    FXControlPanel                            *fxP;
    uint64_t                                   frameSZ = 0;
    std::optional<soundtouch::SoundTouch>      st;
    std::optional<std::thread>                 worker;
    std::atomic<bool>                          flag = true;
    MusicOnDeck() : fxP(new FXControlPanel(48000))
    {

        st.emplace();
        st->setChannels(CHANNEL);
        st->setSampleRate(SAMPLERATE);
        st->setSetting(0, 1);
        st->setSetting(2, 0);
        st->setTempo(1.0);
    };
    void
    Init(const uint64_t frameSize)
    {
        frameSZ = frameSize;
        pb.emplace();
        worker.emplace([this]() { predict_loop(); });
    }
    void
    predict_loop()
    {
        SIMD_FLOAT pcmBuffer(frameSZ);
        PREDICT    pd;
        pd.predict_fragment.resize(frameSZ * CHANNEL);
        std::chrono::milliseconds sleepTime(((frameSZ * 1000) / SAMPLERATE) *
                                            (PREDICT_SIZE / 2));
        while (flag) {
            if (!pb->IsFull()) {
                pd.io_ratio     = st->getInputOutputSampleRatio();
                pd.start_cursor = loaded.cursor;
                pd.used_frames  = static_cast<uint64_t>(
                    std::ceil(static_cast<double>(frameSZ) / pd.io_ratio));
                loaded.getRange(pd.used_frames, pcmBuffer);
                st->putSamples(pcmBuffer.data(), pd.used_frames);
                st->receiveSamples(pd.predict_fragment.data(), frameSZ);
                pb->Fill(pd);
            } else {
                std::this_thread::sleep_for(sleepTime);
            }
        }
    }
    bool
    join()
    {
        if (worker) {
            if (worker->joinable()) {
                flag = false;
                worker->join();
                return true;
            }
        }
        return false;
    }
    ~MusicOnDeck()
    {
        join();
        delete fxP;
    }
};

using LOADS = std::map<std::string, MusicOnDeck>;

/**
 * @brief Music handler for manual mode
 *
 */
class PDJE_API MusicControlPanel {
  private:
    LOADS              deck;
    unsigned long      fsize;
    uint64_t           bufferSZ;
    std::vector<float> L;
    std::vector<float> R;
    float             *FaustStyle[2];
    SIMD_FLOAT         tempFrames;

  public:
    /**
     * @brief loads music to the deck. doesn't play music
     *
     * @param Mus Searched music
     * @return int, miniaudio Error code.
     */
    bool
    LoadMusic(litedb &ROOTDB, const musdata &Mus);

    /**
     * @brief Change playback position of the music
     *
     * @param title the music title
     * @param newPos the new playback position of the music
     * @return true
     * @return false
     */
    bool
    CueMusic(const UNSANITIZED &title, const unsigned long long newPos);

    /**
     * @brief turn on, off the music
     *
     * @param title the music title
     * @param onOff True is on, False is off
     * @return true
     * @return false
     */
    bool
    SetMusic(const UNSANITIZED &title, const bool onOff);

    /**
     * @brief get music list on the deck
     *
     * @return LOADED_LIST
     */
    LOADED_LIST
    GetLoadedMusicList();

    /**
     * @brief unload music from deck. used to prevent memory leaks.
     *
     * @param title the target music title
     * @return true
     * @return false
     */
    bool
    UnloadMusic(const UNSANITIZED &title);

    /**
     * @brief gets decoded pcm frames
     *
     * @param array pcm array output
     * @param FrameSize amount of frames
     * @return true
     * @return false
     */
    bool
    GetPCMFrames(float *array, const unsigned long FrameSize);

    /**
     * @brief gets FX handler
     *
     * @param title the title of the music
     * @return FXControlPanel*, the handler pointer
     */
    FXControlPanel *
    getFXHandle(const UNSANITIZED &title);

    /**
     * @brief changes music's bpm
     *
     * @param title the title of the music
     * @param targetBpm the target bpm of the music
     * @param originBpm the origin bpm of the music
     * @return true
     * @return false
     */
    bool
    ChangeBpm(const UNSANITIZED &title,
              const double       targetBpm,
              const double       originBpm);

    MusicControlPanel(const unsigned long FrameSize, const uint64_t BufferSize)
        : fsize(FrameSize), bufferSZ(BufferSize)
    {
    }
    ~MusicControlPanel();
};
