#pragma once

#include "MusicControlPanel.hpp"
#include "PDJE_EXPORT_SETTER.hpp"
#include "PDJE_Highres_Clock.hpp"
#include "PDJE_SYNC_CORE.hpp"
#include <atomic>
#include <cstdint>
#include <miniaudio.h>
#include <optional>

/**
 * @brief the core Data
 *
 */
struct PDJE_API audioEngineDataStruct {
    float                           *faustPcmPP[2];
    std::optional<FXControlPanel>    FXManualPanel;
    std::optional<MusicControlPanel> MusCtrPanel;
    std::vector<float>              *pcmDataPoint;
    unsigned long long               nowCursor = 0;
    unsigned long long               maxCursor = 0;
    std::atomic<audioSyncData>       syncData =
        audioSyncData{ .consumed_frames = 0, .microsecond = 0 };
    audioSyncData             cacheSync;
    PDJE_HIGHRES_CLOCK::CLOCK highres_clock;
    /**
     * @brief Get Current playback point
     *
     * @param frameCount Amount of frames to be returned
     * @return std::optional<float*> the pointer, return nullopt if exceed range
     */
    inline std::optional<float *>
    getNowfPointer(const unsigned long frameCount);

    /**
     * @brief count up function.
     *
     * @param frameCount Amount of frames
     */
    inline void
    CountUp(const unsigned long frameCount);

    /**
     * @brief Return FX processed PCM frames
     *
     * @param pOutput the pcm output. this should be memory allocated.(size:
     * frameCount * CHANNEL * sizeof(float))
     * @param frameCount Amount of frames
     */
    void
    GetAfterManFX(float *pOutput, unsigned long frameCount);

    /**
     * @brief Return PCM frames
     *
     * @param pOutput the pcm output. this should be memory allocated.(size:
     * frameCount * CHANNEL * sizeof(float))
     * @param frameCount Amount of frames
     */
    void
    Get(float *pOutput, unsigned long frameCount);
};
