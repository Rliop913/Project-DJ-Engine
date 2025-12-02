#pragma once
#include "Decoder.hpp"
#include "FrameCalc.hpp"

struct PDJE_API PreLoadedMusic {

    SIMD_FLOAT           music;
    ma_uint64            fullSize;
    uint64_t             cursor;
    SIMD_FLOAT::iterator p;
    PreLoadedMusic()  = default;
    ~PreLoadedMusic() = default;

    bool
    init(litedb &db, const SANITIZED_ORNOT &KeyOrPath);
    /**
     * @brief changes the playback position
     *
     * @param Pos new position
     * @return true
     * @return false
     */
    bool
    changePos(FRAME_POS Pos);

    /**
     * @brief Get the playback position
     *
     * @param pos position to be returned
     * @return true
     * @return false
     */
    bool
    getPos(FRAME_POS &pos);

    /**
     * @brief check the music's range and return decoded pcm frames
     *
     * @param numFrames amount of frames
     * @param buffer pcm frames to be returned
     * @return true
     * @return false
     */
    bool
    getRange(FRAME_POS numFrames, SIMD_FLOAT &buffer);
};
