#pragma once

#include <optional>

#include <SoundTouch.h>

#include "DeckData.hpp"
#include "Decoder.hpp"
#include "MixTranslator.hpp"
#include "MusicTranslator.hpp"
#include "dbRoot.hpp"

constexpr FRAME_POS BPM_WINDOWS_SIZE = SAMPLERATE / 1000;

using GLOBAL_POS = FRAME_POS;
using LOCAL_POS  = FRAME_POS;

#define GET_BIGGER(A, B) (A < B ? B : A)

enum MIXSTATE {
    PLAY,
    PAUSE,
    END,
    BPMCHANGE,
};

/**
 * @brief Data structure for managing global/local playback positions
 *
 */
struct PDJE_API PlayPosition {
    GLOBAL_POS Gidx;
    LOCAL_POS  Lidx;
    double     OriginBPM = -1;
    double     TargetBPM = -1;
    MIXSTATE   status    = MIXSTATE::PLAY;
};

/**
 * @brief A structure that sorts mixing data, computes global/local playback
 * positions, and finalizes BPM flow control calculations.
 *
 */
struct PDJE_API Ingredients {
  private:
    /**
     * @brief sort the mix objects
     *
     */
    void
    SORT();

    /**
     * @brief calculates global position
     *
     * @param Gbpm Global bpm applied position object
     * @param Global Global bpm object
     */
    void
    FillGlobal(std::vector<PlayPosition> &Gbpm, const BpmStruct &Global);
    /**
     * @brief calculates local position
     *
     * @param Lbpm Local bpm applied position object
     * @param Local Local bpm object
     */
    void
    FillLocal(std::vector<PlayPosition> &Lbpm, const BpmStruct &Local);
    /**
     * @brief Returns overlapping positions based on the Global index
     *
     * @param gidx global index
     * @return std::vector<PlayPosition>::iterator
     */
    std::vector<PlayPosition>::iterator
    GetSameGidx(GLOBAL_POS gidx);

  public:
    /**
     * @brief Prepares finalized data. All computations are completed, and the
     * data is made ready for use.
     *
     * @param Global global bpm object
     * @param Local local bpm object
     */
    void
    Ready(const BpmStruct &Global, const BpmStruct &Local);

    std::vector<PlayPosition> pos;
};
/**
 * @brief music controller object for mixing
 *
 */
class PDJE_API MUSIC_CTR {
  private:
    std::optional<soundtouch::SoundTouch> st;
    std::optional<Decoder>                D;
    CapReader<MusicBinaryCapnpData>       capnpMus;
    MusicTranslator                       Mus;
    std::vector<float>                    timeStretchBuffer;
    /**
     * @brief check initialized
     *
     * @return true
     * @return false
     */
    bool
    checkUsable();

    /**
     * @brief apply time stretch
     *
     * @param Frame amount of frames
     * @param masterPTR pcm frames
     * @return true
     * @return false
     */
    bool
    TimeStretch(const FRAME_POS Frame, float *&masterPTR);
    /**
     * @brief Internally uses TimeStretch() and ChangeBPM(). Implements the
     * Facade pattern.
     *
     * @param targetBpm
     * @param originBpm
     * @param LocalIDX local position
     * @param RenderAmount amount of frames
     * @param masterPTR PCM frames
     * @return true
     * @return false
     */
    bool
    Render(const double     targetBpm,
           const double     originBpm,
           const LOCAL_POS  LocalIDX,
           const GLOBAL_POS RenderAmount,
           float          *&masterPTR);
    /**
     * @brief changes bpm. this sets the soundtouch object.
     *
     * @param targetbpm
     * @param originbpm
     */
    void
    ChangeBpm(double targetbpm, double originbpm);

  public:
    Ingredients QDatas;
    MUSIC_CTR();
    ~MUSIC_CTR() = default;
    /**
     * @brief load music from database
     *
     * @param RP capnp reader object
     * @param db RootDB
     * @param FrameIn the music's first beat position
     * @return true
     * @return false
     */
    bool
    setLOAD(MBData::Reader &RP, litedb &db, FRAME_POS FrameIn);
    // bool AddFrameCut(FRAME_POS cutIn, FRAME_POS cutOut);

    // std::optional<double> originBpm;
    std::optional<std::string> songPath;
    /**
     * @brief Facade pattern: renders all music using mixing data into a single
     * PCM array.
     *
     * @param bpms BPM object
     * @param PCMS PCM array pointer
     * @param dbRoot RootDB
     * @return std::optional<SIMD_FLOAT*>
     */
    std::optional<SIMD_FLOAT *>
    Execute(const BPM &bpms, SIMD_FLOAT *PCMS, litedb &db);

    /**
     * @brief send soundtouch, decoder object to other class
     *
     * @param stp soundtouch obj to fill
     * @param dp decoder obj to fill
     * @return true
     * @return false
     */
    bool
    SendData(soundtouch::SoundTouch *&stp, Decoder *&dp);
};
