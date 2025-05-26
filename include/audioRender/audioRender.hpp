#pragma once

#include "MixMachine.hpp"
#include "ManualMix.hpp"
/**
 * @brief Top-level class for audio rendering
 * 
 * \dot
 * digraph audioRender{
 *      audioRender -> MixMachine;
 *      MixMachine -> MixTranslator;
 *      MixTranslator -> CapReader;
 *      CapReader -> MixBinaryCapnpData;
 * }
 * \enddot
 * 
 */
class audioRender{
public:
    std::optional<std::vector<float>> rendered_frames;

    /**
     * @brief Loads Track Data
     * 
     * @param db RootDB
     * @param td trackdata
     * @return true 
     * @return false 
     */
    bool LoadTrack(litedb& db, trackdata& td);
    bool LoadTrackFromMixData(litedb& db, BIN& mixData);
    audioRender() = default;
    ~audioRender() = default;

};