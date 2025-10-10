#pragma once

#include <optional>

#include "MusicControlPanel.hpp"
#include "PDJE_EXPORT_SETTER.hpp"
#include <miniaudio.h>

/**
 * @brief the core Data
 *
 */
struct PDJE_API audioEngineDataStruct {
    float                           *faustPcmPP[2];
    std::optional<FXControlPanel>    FXManualPanel;
    std::optional<MusicControlPanel> MusCtrPanel;
    std::vector<float>              *pcmDataPoint;
    unsigned long long               nowCursor      = 0;
    unsigned long long               maxCursor      = 0;
    unsigned long long               consumedFrames = 0;

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
